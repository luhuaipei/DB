select PrereqUoSCode,UoSName from requires Join unitofstudy on PrereqUoSCode= unitofstudy.UoSCode
where requires.UoSCode = '['
and requires.UosCode not in 
(select UosCode from transcript where StudId = ']');
