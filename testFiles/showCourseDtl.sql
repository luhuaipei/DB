#showCourseDtl.sql    input: 3213 COMP3419     userid courseNum
-- The course details should include: the course number and title, the year
-- and quarter when the student took the course, the number of enrolled students,
-- the maximum enrollment and the lecturer (name), the grade scored by the
-- student UoSCode, UosName,Enrollment,MaxEnrollment,Name 
select Year, Semester,UoSCode, UosName,Enrollment,MaxEnrollment,Name from uosoffering Join unitofstudy Using(UosCode) JOIN faculty on faculty.Id = InstructorId
Where Semester = 
	(select Semester from transcript where  UoSCode='COMP3419' AND StudId=3213)
    AND Year = 
    (select Year from transcript where UoSCode = 'COMP3419' AND StudId=3213)
    AND UoSCode = 'COMP3419';
 