//#include "stdafx.h"
//#include <windows.h>
#include "mysql.h"
//for windows users
//for windows users
//you must copy this from MySQL folder to your
//project folder
//#include <iostream>
//using namespace std;
#include <stdio.h>
#include <stdlib.h>
MYSQL *conn;
/* pointer to connection handler */
int main ( int argc, char *argv[] )
{
conn = mysql_init ( NULL );
// open connection
mysql_real_connect (
conn,
/* pointer to connection handler */
"localhost",
/* host to connect to */
"root",
/* user name */
"lhp920428",
/* password */
"project2-moviedb",
/* database to use */
3306,
/* port (default 3306) */
NULL,
/* socket or /var/lib/mysql.sock */
CLIENT_MULTI_RESULTS );
/* flags (none) */
// close connection
mysql_close ( conn );
return 0;
}
