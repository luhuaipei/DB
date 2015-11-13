DROP procedure IF EXISTS `changeADDR`;
DELIMITER //
CREATE PROCEDURE changeADDR
(IN userId int(11),
 IN newADDR varchar(50))
Begin
START TRANSACTION;
update student set Address = newADDR where Id=userId;
COMMIT;
END //
DELIMITER ;
