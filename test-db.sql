DROP TABLE IF EXISTS attendance CASCADE;
DROP TABLE IF EXISTS class CASCADE;
DROP TABLE IF EXISTS rooms CASCADE;
DROP TABLE IF EXISTS schedule CASCADE;
DROP TABLE IF EXISTS schoolUsers CASCADE;
DROP TABLE IF EXISTS weekdays CASCADE;
DROP TABLE IF EXISTS subjects CASCADE;

CREATE TABLE class
(
	class_id  SERIAL PRIMARY KEY,
	name  CHAR(18) NULL
)
;

CREATE TABLE rooms
(
	room_id  SERIAL PRIMARY KEY,
	name  CHAR(18) NULL,
	access_level  INT NULL
)
;

CREATE TABLE subjects
(
	subject_id  SERIAL PRIMARY KEY,
	name  CHAR(18) NULL
)
;

CREATE TABLE weekdays
(
	weekday_id  SERIAL PRIMARY KEY,
	weekday  CHAR(18) NULL
)
;

CREATE TABLE schoolUsers
(
	user_id  SERIAL PRIMARY KEY,
	username  CHAR(18) NULL,
	level  INT NULL,
	class_id  INT NOT NULL,
      	FOREIGN KEY(class_id) 
	REFERENCES class(class_id)
)
;

CREATE TABLE attendance
(
	attendance_id SERIAL PRIMARY KEY,
	user_id  INT REFERENCES schoolUsers,
	time TIMESTAMP NULL,
	class_id  INT REFERENCES class
)
;

CREATE TABLE schedule
(
	lesson_id  SERIAL PRIMARY KEY,
	room_id  INT NOT NULL,
	weekday_id  INT NOT NULL,
	startTime  TIMESTAMP NULL,
	finishTime  TIMESTAMP NULL,
	subject_id  INT NOT NULL,
	class_id  INT NOT NULL,
      	FOREIGN KEY(weekday_id) 
	REFERENCES weekdays(weekday_id),
      	FOREIGN KEY(room_id) 
 	REFERENCES rooms(room_id),
      	FOREIGN KEY(subject_id) 
	REFERENCES subjects(subject_id),
      	FOREIGN KEY(class_id) 
	REFERENCES class(class_id)
)
;
