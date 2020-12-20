#include <QDebug>
#include <QSqlTableModel>
#include <QSignalMapper>
#include <QSqlRelationalTableModel>
#include <QTableView>
#include <QPushButton>
#include <QDebug>
#include <QUuid>
#include <QLineEdit>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	db(QSqlDatabase::addDatabase("QPSQL"))
{
	ui->setupUi(this);

	db.setHostName("localhost");
	db.setDatabaseName("test");
	db.setUserName("postgres");
	db.setPassword("postgres");

	bool res = connect(ui->readFromButton, &QPushButton::clicked,this,&MainWindow::on_readButton_clicked) &&
	connect(ui->writeToButton, &QPushButton::clicked,this,&MainWindow::on_writeButton_clicked) &&
	connect(ui->addRecordButton, &QPushButton::clicked,this,&MainWindow::on_addrecordButton_clicked);
	if (!res)
	{
		qFatal("connect signals failed");
	}

	ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
	//ui->tableWidget->verticalHeader()->setStretchLastSection(true);
	ui->tableWidget->show();
}

void MainWindow::init()
{
	bool ok = db.open();
	if(!ok)
	{
		qInfo() << "Db error: " << db.lastError().databaseText();
		return;
	}
}

MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::on_readButton_clicked()
{
	qInfo() << "read from bd";

	auto query = db.exec("SELECT user_id, username, level, class.name FROM schoolusers join class on schoolusers.class_id=class.class_id");

	if (query.lastError().isValid())
	{
	   qCritical() << "Query failed:" << query.lastError().text();
	}
	else
	{
		ui->tableWidget->clear();//clearcont?
		ui->tableWidget->setRowCount(query.size());
		ui->tableWidget->setColumnCount(query.record().count() + 1);
		int row = 0;
	   while (query.next())
	   {
		   for (int i = 0; i < query.record().count(); ++i)
		   {

			   auto cell = new QLineEdit(query.value(i).toString());
			   if(i == 0)
			   {
					cell->setReadOnly(true);
			   }
				ui->tableWidget->setCellWidget( row, i, cell );
		   }
		   auto delButton = new QPushButton("X");
		   auto butName = QUuid::createUuid().toString();
		   delButton->setObjectName(butName);

		   connect(delButton, &QPushButton::clicked, [this, butName] { deleteRow(butName);});
		   ui->tableWidget->setCellWidget( row, ui->tableWidget->columnCount()-1, delButton );

		   row++;
	   }
	}
}

void MainWindow::deleteRow(const QString& butName)
{
	int delCol = ui->tableWidget->columnCount() - 1;
	int delRow = 0;
	for (; delRow < ui->tableWidget->rowCount(); ++delRow)
	{
		auto delbut = dynamic_cast<QPushButton*>(ui->tableWidget->cellWidget(delRow,delCol));
		if(delbut->objectName() == butName)
		{
			break;
		}
	}

	if(delRow == ui->tableWidget->rowCount())
	{
		qCritical() << "X Button not found. Delete row failed ";
		return;
	}

	qInfo() << "delete row" << delRow;
	auto cell = dynamic_cast<QLineEdit*>(ui->tableWidget->cellWidget(delRow,0));
	Q_ASSERT(cell);

	if (cell->text() == "auto")
	{
		ui->tableWidget->removeRow(delRow);
		return;
	}

	auto deleteQ = QString("DELETE FROM schoolusers WHERE user_id=%1").arg(cell->text());

	qInfo() << deleteQ;
	auto query = db.exec(deleteQ);
	if (query.lastError().isValid())
	{
	   qCritical() << "Query failed:" << query.lastError().text();
	}
	else
	{
		ui->tableWidget->removeRow(delRow);
	}
}


void MainWindow::on_writeButton_clicked()
{
	if (db.transaction())
	{
		qInfo() << "Start write transaction";
	}

	for (int row = 0; row < ui->tableWidget->rowCount(); ++row)
	{
		bool isChanged = false;
		//QString update = "UPDATE schoolusers SET username='%1', level=%2, class_id=%3 where user_id=%0";
		QString update = "UPDATE schoolusers SET username='%1', level=%2, class_id=(select class_id from class where name='%3') where user_id=%0";
		for (int col = 0; col < ui->tableWidget->columnCount() -1; ++col)
		{
			auto cell = dynamic_cast<QLineEdit*>(ui->tableWidget->cellWidget(row,col));
			Q_ASSERT(cell);

			if(col == 0 && cell->text() == "auto")
			{
				//update = "INSERT INTO schoolusers (username, level, class_id) VALUES ('%1',%2,%3)";
				update = "INSERT INTO schoolusers (username, level, class_id) VALUES ('%1',%2,(select class_id from class where name='%3'))";
			}
			else
			{
				update = update.arg(cell->text());
				if(cell->isModified())//for insert?
				{
					isChanged = true;
					//cell->setModified(false);
				}
			}

		}
		if(isChanged)
		{
			qInfo() << update;
			auto query = db.exec(update);
			if (query.lastError().isValid())
			{
			   qCritical() << "Query failed:" << query.lastError().text();
			   if (db.rollback())
			   {
				   qInfo() << "Rollback write transaction";
			   }
			   return;
			}
		}
	}

	if (db.commit())
	{
		for (int row = 0; row < ui->tableWidget->rowCount(); ++row)
		{
			for (int col = 0; col < ui->tableWidget->columnCount() -1; ++col)
			{
				auto cell = dynamic_cast<QLineEdit*>(ui->tableWidget->cellWidget(row,col));
				Q_ASSERT(cell);
				cell->setModified(false);
			}
		}
		qInfo() << "Commit write transaction";
		on_readButton_clicked();
	}
}

void MainWindow::on_addrecordButton_clicked()
{
	auto row = ui->tableWidget->rowCount();

	ui->tableWidget->insertRow(row);

	for (int col = 0; col < ui->tableWidget->columnCount(); ++col)
	{
		auto cell = new QLineEdit;
		if(col == 0)
		{
			 cell->setReadOnly(true);
			 cell->setText("auto");
		}
		 ui->tableWidget->setCellWidget( row, col, cell );
	}

	auto delButton = new QPushButton("X");
	auto butName = QUuid::createUuid().toString();
	delButton->setObjectName(butName);

	connect(delButton, &QPushButton::clicked, [this, butName] { deleteRow(butName);});
	ui->tableWidget->setCellWidget( row, ui->tableWidget->columnCount()-1, delButton );

	qInfo() << "Insert new row in table widget";
}

