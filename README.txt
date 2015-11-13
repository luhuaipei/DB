In the sql script file, please replace the input parameter with the symbal after them. For example, 
				
		Select * from Student where Id = [   
									 (if ID is an int)

		Select * from stddent Where name = '['
									(if name is an varchar)

checkPassWord_std.sql input: userID		'['     +

showCurrCour.sql   input: userID		'['		+
					 	    quarter		']'		
					 	    year		'?'

showTrans.sql		 input: userID		'['		+

showCourseDtl.sql    input: userID		'['		+
							courseNum	']'
							
showPersonDtl.sql	 input: userID		'['		+

updatePassword.sql	 input: userID		'['		+
							newPassword	']'

updateAddr.sql		 input: userID		'['		+
					 		newAddr		']'

showNewCour.sql		 input: userID		'['		+
							quarter		']'
							year		'?'

Enroll.sql			 input: userID		'['		
					 		courseNum	']'

drop.sql			 input: userID		'['		+
							courseNum	']'
checkCourse.sql		 input: userID  '['
					 courseNum		']'
					 year			'?'

If the Enrollment number goes below 50% of the MaxEnrollment, then a warning
message should be shown on the screen. Implement this using Triggers.
