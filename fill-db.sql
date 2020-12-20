INSERT INTO subjects (name) 
	VALUES ('Математика'),
		('Русский язык'),
		('Английский язык'),
		('Литература'),
		('Физика');

INSERT INTO class (name) 
	VALUES ('9A'),
		('8B'),
		('1A'),
		('3E'),
		('9B');

INSERT INTO rooms (name,access_level) 
	VALUES ('Кабинет 15',5),
		('Столовая',4),
		('Лаборатория',3),
		('Спортивный зал',4),
		('Администрация',1);

INSERT INTO weekdays (weekday) 
	VALUES ('Понедельник'),
		('Вторник'),
		('Среда'),
		('Четверг'),
		('Пятница'),
		('Суббота'),
		('Воскресенье');

INSERT INTO schoolUsers (class_id,username,level) 
	VALUES (1,'Арсений Вавин',5),
		(2,'Копытин Егор',3),
		(4,'Хрусталев Дмитрий',5),
		(3,'Осчапко Артур',4),
		(5,'Пономарев Игорь',3),
		(2,'Сидельнова Мария',5);

INSERT INTO attendance (user_id,class_id,time) 
	VALUES (1, (SELECT class_id FROM schoolUsers WHERE user_id=1), TIMESTAMP '2020-12-02 12:00:01'),
		(2, (SELECT class_id FROM schoolUsers WHERE user_id=2), TIMESTAMP '2020-12-04 16:10:00'),
		(4, (SELECT class_id FROM schoolUsers WHERE user_id=4), TIMESTAMP '2020-12-01 12:40:00'),
		(5, (SELECT class_id FROM schoolUsers WHERE user_id=5), TIMESTAMP '2020-12-02 02:30:00'),
		(6, (SELECT class_id FROM schoolUsers WHERE user_id=6), TIMESTAMP '2020-11-22 14:00:00'),
		(1, (SELECT class_id FROM schoolUsers WHERE user_id=1), TIMESTAMP '2020-12-02 10:50:00'),
		(6, (SELECT class_id FROM schoolUsers WHERE user_id=6), TIMESTAMP '2020-12-01 12:40:00'),
		(4, (SELECT class_id FROM schoolUsers WHERE user_id=4), TIMESTAMP '2020-12-01 18:40:00'),
		(2, (SELECT class_id FROM schoolUsers WHERE user_id=2), TIMESTAMP '2020-12-12 12:40:00'),
		(4, (SELECT class_id FROM schoolUsers WHERE user_id=4), TIMESTAMP '2020-12-02 12:40:00'),
		(3, (SELECT class_id FROM schoolUsers WHERE user_id=3), TIMESTAMP '2020-12-03 14:40:00');

INSERT INTO schedule (room_id,weekday_id,startTime,finishTime,subject_id,class_id) 
	VALUES (1,3, '2020-12-01 12:15:00', '2020-12-01 12:40:00',2,3),
	(1,1, '2020-12-09 14:15:00', '2020-12-09 15:40:00',1,5),
	(2,4, '2020-12-06 16:25:00', '2020-12-06 17:40:00',3,5),
	(4,2, '2020-12-05 16:15:00', '2020-12-05 18:00:00',4,4),
	(3,1, '2020-12-01 14:45:00', '2020-12-01 15:00:00',2,2),
	(1,1, '2020-12-01 14:45:00', '2020-12-01 15:00:00',5,3),
	(3,5, '2020-12-06 13:45:00', '2020-12-06 15:00:00',2,2),
	(4,3, '2020-12-07 09:30:00', '2020-12-07 10:00:00',2,3),
	(1,2, '2020-12-01 15:45:00', '2020-12-01 17:00:00',5,2),
	(5,2, '2020-12-03 12:15:00', '2020-12-03 13:00:00',4,3),
	(2,5, '2020-12-01 10:15:00', '2020-12-01 11:40:00',5,1);

--Вывести расписание, отсортированное по времени для 1А класса
SELECT 
rooms.name as Кабинет,
weekdays.weekday as "День недели",
startTime as Начало,
finishTime as Конец, 
subjects.name as Предмет, 
class.name as Класс 
FROM schedule 
JOIN class ON schedule.class_id=class.class_id 
JOIN subjects ON schedule.subject_id=subjects.subject_id 
JOIN rooms ON schedule.room_id=rooms.room_id 
JOIN weekdays ON schedule.weekday_id=weekdays.weekday_id 
WHERE class.name='1A' 
ORDER BY schedule.startTime, schedule.finishTime; 

-- Вывести пользователей, которые пришли с 1 и 2 декабря и отсортировать по времени
SELECT 
schoolUsers.username as Пользователь,
attendance.time as Время
FROM attendance 
JOIN schoolUsers ON attendance.user_id=schoolUsers.user_id
WHERE time between '2020-12-01' and '2020-12-02 23:59:59'
ORDER BY attendance.time;

-- Посчитать сколько раз пришел пользователь с id=4
select count(*) FROM attendance WHERE user_id=4;

