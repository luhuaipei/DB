DROP procedure IF EXISTS `enroll`;
delimiter //
CREATE procedure enroll(
IN userId int(11),
IN classId char(8),
IN quarter char(8),
IN inputyear char(11),
OUT pre int(8))

Begin 
DECLARE numOfPre int(11);
DECLARE passedPre int(11);
SELECT count(*) INTO numOfPre from requires where UoSCode = classId;
SELECT count(*) into passedPre FROM transcript
			WHERE UoSCode in (SELECT PrereqUoSCode from requires where UoSCode = classId)
			AND Grade is not null
            AND Grade != 'F'
            AND StudId = userId;
if passedPre = numOfPre THEN
	START TRANSACTION; 
    INSERT INTO transcript VALUES(userId,classId,quarter,inputyear,NULL);
    UPDATE uosoffering SET uosoffering.Enrollment = uosoffering.Enrollment + 1 where UoSCode=classId AND Semester = quarter AND uosoffering.Year = inputyear;
    SET pre = 100;
    COMMIT;
    else 
		set pre = numOfPre+1;

 END IF;
END //
DELIMITER ;
