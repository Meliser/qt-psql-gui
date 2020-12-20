#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlQueryModel>
namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = 0);
	void init();
	~MainWindow();

private slots:
	void on_readButton_clicked();
	void deleteRow(const QString& butName);
	void on_writeButton_clicked();
	void on_addrecordButton_clicked();

private:
	Ui::MainWindow *ui;
	QSqlDatabase db;
};

#endif // MAINWINDOW_H
