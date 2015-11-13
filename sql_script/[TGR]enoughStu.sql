DROP table IF EXISTS `monitor`;

DROP trigger IF EXISTS `enoughStu`;

create table monitor(
	Num int(8),
	UoSCode char(8),
    Semester char(8),
    Year char(8),
    flag int(3),
    PRIMARY KEY (Num)
);
insert into monitor values(1,NULL,NULL,NULL,0);

delimiter //
create trigger enoughStu
after update on uosoffering
for each row
begin

if old.Enrollment+1 < old.MaxEnrollment/2 then
	UPDATE monitor set UoSCode = old.UoSCode,
						Semester = old.Semester,
                        monitor.Year = old.Year,
                        flag = 1
                        WHERE Num = 1;
   
end if;
end //
delimiter ;