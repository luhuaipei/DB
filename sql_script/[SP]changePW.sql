DROP procedure IF EXISTS `changePW`;
DELIMITER //
CREATE PROCEDURE changePW
(IN userId int(11),
 IN newPW varchar(10))
Begin
START TRANSACTION;
update student set Password = newPW where Id=userId;
COMMIT;
END //
DELIMITER ;
