#include <stdbool.h>
#include <my_global.h>
#include <mysql.h>
#include <string.h>
#include <time.h>
// global variables------------------------------------
#define PASSWORD_LENGTH 20
#define USERID_LENGTH 11
//struct declaration------------------------------------
typedef struct Quarter {
    char qtr[2]; //spring :1 summer:2 fall:3 winter:4
    char year[5];
    char quarter[10];
    struct Quarter* nextQtr;
} Qtr;
// Function decalration---------------------------------
void initDB();
void finish_with_error(MYSQL *con);
void loggin();
bool checkLogin(char* ID,char* PW);
void studenMenu( char* userID);
void printCurrCour(char* userID);
void transcript(char* userID);
void showTrans(char* userID);
void showCourDtl(char* UserID, char* courseNum);
void enrollPage(char* UserID);
void withdraw();
void personalDetl(char* userID);
void queryDB(char* query, int mode);
char* readQuery(char* fileName);
Qtr* checkQuarter();
char* processQuery(char* query, char* userPart,int flag);
bool checkCourse(char* userID,Qtr* queryTime,char* courseNum);
bool enrollDB(char * query);
void checkTrigger();
//-------------------------------------------------
// void initDB(){
// 	char* changeADDR= readQuery("./sql_script/[SP]changeADDR.sql");
// 	printf("1\n");
// 	char* changePW= readQuery("./sql_script/[SP]changePW.sql");
// 	printf("2\n");
// 	char* enroll= readQuery("./sql_script/[SP]enroll.sql");
// 	printf("3\n");
// 	char* drop= readQuery("./sql_script/[SP]drop.sql");
// 	printf("4\n");
// 	char* enoughtStu = readQuery("./sql_script/[TGR]enoughStu.sql");
// 	printf("5\n");
// 	MYSQL *conn = mysql_init(NULL);
// 	if(conn == NULL){
// 		fprintf(stderr,"mysql_init() failed\n");
// 		exit(1);
// 	}
// 	if (mysql_real_connect(conn, "localhost", "root", "lhp920428",
// 		"project3-nudb", 0, NULL, 0) == NULL){
// 		finish_with_error(conn);
// 	}
// 	if(mysql_query(conn, changeADDR)){
// 		finish_with_error(conn);
// 		}
// 	if(mysql_query(conn, changePW)){
// 		finish_with_error(conn);
// 		}
// 	if(mysql_query(conn, enroll)){
// 		finish_with_error(conn);
// 		}
// 	if(mysql_query(conn, drop)){
// 		finish_with_error(conn);
// 		}
// 	if(mysql_query(conn, enoughtStu)){
// 		finish_with_error(conn);
// 		}
// 	mysql_close(conn);


// }
int main()
{
//	initDB();
    while(1) {
        printf("-----------------Welcome to My Ceasar-----------------\n\n");
        printf("1. Login\n");
        printf("2. exit\n");
        printf("Please enter your choice.\n");
        int choise;
        scanf("%d",&choise);
        switch(choise) {
        case 1:
            loggin();
            break;
        case 2:
            printf("Bye!\n");
            exit(2);
            break;
        }
    }
}
void finish_with_error(MYSQL *con)
{
    printf("Error!!!!!!!\n");
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

void loggin()
{
    printf("-----------------Welcome to My Ceasar Login-----------------\n\n");
    printf("Please Use your User name and password to login\n");
    char userID[USERID_LENGTH];  //length of userID
    char password[PASSWORD_LENGTH]; //length of password
    //TODO: userID and password need to be processed,drop space

    printf("UserID:");
    scanf("%s",userID);
    printf("Your ID: %s\n",userID);

    printf("Password: ");
    scanf("%s",password);
    printf("Password: %s\n",password);

    if(checkLogin(userID,password)) {
        //succssfully logged in
        //go into main manu
        studenMenu(userID);
    } else {
        printf("UserID or Password is not correct.");
        printf("Please Check. \n");
    }
}
bool checkLogin(char* userID,char* PW)
{
    //query the database to check password
    char* query;
    char* query_after_process;
    bool isFaculty=false; //reserved for handle faculty login
    //create a connection to database
    MYSQL *con = mysql_init(NULL);
    if (con == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }

    if (mysql_real_connect(con, "localhost", "root", "lhp920428",
                           "project3-nudb", 0, NULL, 0) == NULL) {
        finish_with_error(con);
    }

    // //make query
    query = readQuery("./sql_script/checkPassWord_std.sql");
    // //processQuery
    query_after_process = processQuery(query,userID,1);

    //printf("query: %s\n", query_after_process);

    if (mysql_query(con, query_after_process)) {
        finish_with_error(con);
    }
    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        finish_with_error(con);
    }
    MYSQL_ROW row;
    row = mysql_fetch_row(result);
    if (row==NULL) {
        //not a student,query the faculty table
        query = readQuery("./sql_script/checkPassWord_prof.sql");
        // //processQuery
        query_after_process = processQuery(query,userID,1);
        //	printf("prof: %s\n",query_after_process);

        if (mysql_query(con, query_after_process)) {
            finish_with_error(con);
        }
        result = mysql_store_result(con);

        if (result == NULL) {
            finish_with_error(con);
        }
        row = mysql_fetch_row(result);
        if(row==NULL) {
            return false;
        }


    }
//	printf("password: %s\n", row[0]);
    //check password and the query result equall or not?
    if(strcmp(row[0],PW)!=0) {
        mysql_free_result(result);
        mysql_close(con);
        free(query);
        free(query_after_process);
        return false;
    };

    mysql_free_result(result);
    mysql_close(con);
    free(query);
    free(query_after_process);

    return true;
}

void studenMenu(char* userID)
{
    printf("---------------------Student Main Menu---------------------\n");
    int quit=0;
    printCurrCour(userID);
    while(!quit) {
        printf("---------------------Menu Options---------------------\n");
        printf("1. Transcipt\n");
        printf("2. Personal Detail\n");
        printf("3. Enroll or Withdraw\n");
        printf("4. Log out\n");
        printf("Please enter your choice.\n");
        int choise;
        scanf("%d",&choise);
        int c;
        switch(choise) {
        case 1:
            transcript(userID);
            break;
        case 2:
            personalDetl(userID);
            break;
        case 3:
            enrollPage(userID);
            break;
        case 4:
            quit = 1;
            printf("Logged out successfully!\n");
            break;
        }

    }

}

char* processQuery(char* query, char* userPart,int flag)
{
    //replce flag=1:'[', flag=2: ']' flag=3: '?' in the query with userPart
    //warning: after return from the function, please free the response pointer
    int lengthQuery =strlen(query); //sizeof(*query);// sizeof(char);
    int lengthPart = strlen(userPart);// sizeof(char);
//	printf("userPart: %s\n",userPart);
//	printf("query before:\n %s\n",query);
    char s[2]=" ";
    if (flag==1) {
        s[0]='[';   //replace the '[' in query with userPart
    } else if (flag==2) {
        s[0]=']';   //replace the ']' in query with userPart
    } else if (flag ==3) {
        s[0]='?';	//replace the '?'' in query with userPart
    } else {
        s[0]='$';
    }
    char* res =	(char*)malloc(sizeof(char)*(lengthQuery + 10*lengthPart+1));
    memset(res,'\0',lengthQuery + 10*lengthPart);
    char* token;
//	char* token1;
//	char* token2;
    token = strtok(query,s);
    while(token!=NULL) {
        strcat(res,token);
        if((token=strtok(NULL,s))!=NULL) {
            strcat(res,userPart);
        }
    }
//	printf("query after:\n %s\n",res);
    return res;
}

void queryDB(char* query, int mode)
{
    MYSQL *conn = mysql_init(NULL);
    if(conn == NULL) {
        fprintf(stderr,"mysql_init() failed\n");
        exit(1);
    }
    if (mysql_real_connect(conn, "localhost", "root", "lhp920428",
                           "project3-nudb", 0, NULL, 0) == NULL) {
        finish_with_error(conn);
    }
    if(mysql_query(conn, query)) {
        finish_with_error(conn);
    }
    if(mode == 1) {
        MYSQL_RES *result = mysql_store_result(conn);
        if(result==NULL) {
            finish_with_error(conn);
        }
        int num_fields = mysql_num_fields(result);
        MYSQL_ROW row;
        MYSQL_FIELD * fields;
        fields = mysql_fetch_fields(result);
        int field_size;
        int m;
        for(int j = 0 ; j < num_fields; j++) {
            field_size= fields[j].max_length;
            for(m=0; m<field_size/2; m++) {
                printf(" ");
            }
            printf("%s",fields[j].name);
            for(m=0; m<field_size/2; m++) {
                printf(" ");
            }
        }
        printf("\n-------------------------------------------------------\n");
        while ((row = mysql_fetch_row(result))) {
            for(int i = 0; i < num_fields; i++) {
                field_size=fields[i].max_length;
                for(m=0; m<field_size/1.5; m++) {
                    printf(" ");
                }
                printf("%s", row[i] ? row[i] : "NULL");
                for(m=0; m<field_size/1.5; m++) {
                    printf(" ");
                }
            }
            printf("\n");
        }
        mysql_free_result(result);
    } else {
        // echo the update, print the database after edit
    }

    mysql_close(conn);

}
void printCurrCour(char* userID)
{
    //form query
    char* query;
    char* query_after_process;
    Qtr* queryTime;
    queryTime =	checkQuarter();
    printf("quarter: %s year: %s\n",queryTime->quarter, queryTime->year);
    //make query
    query = readQuery("./sql_script/showCurrCour.sql");
    //processQuery
    query_after_process = processQuery(query,userID,1);
    query_after_process = processQuery(query_after_process,queryTime->quarter,2);
    query_after_process = processQuery(query_after_process,queryTime->year,3);
    queryDB(query_after_process,1);
    free(query);
    free(query_after_process);
}

void transcript(char* userID)
{
//print the transcript of current student, include current quarter with NULL value
// query the db
    printf("---------------------Student Transcipt---------------------\n");
    showTrans(userID);
// provide an option for detail info or go back
    int back=0;
    while(!back) {
        printf("---------------------Transcipt Options---------------------\n");
        printf("1. See detail\n");
        printf("2. Go back to [Student Main Menu]\n");
        printf("Please enter your choice.\n");
        int choice;
        char courseNum[10];
        scanf("%d",&choice);
        int c;
        switch(choice) {
        case 1:
            //query the db to see detail of a specific course
            printf("Please input a course number\n");
            scanf("%s",courseNum);
            showCourDtl(userID,courseNum);
            break;
        case 2:
            back = 1;
            break;
        }
    }
}
void showTrans(char* userID)
{
    //show the transcript of a student
    //form query
    char* query;
    char* query_after_process;
    //make query
    query = readQuery("./sql_script/showTrans.sql");
    query_after_process = processQuery(query,userID,1);
    queryDB(query_after_process,1);
    free(query);
    free(query_after_process);

}
void showCourDtl(char* UserID, char* courseNum)
{
    //The course details should include: the course number and title, the year
    //and quarter when the student took the course, the number of enrolled students,
    //the maximum enrollment and the lecturer (name), the grade scored by the student.
    char* query;
    char* query_after_process;
    //make query
    query = readQuery("./sql_script/showCourseDtl.sql");
//	printf("---%s\n",query);
    query_after_process = processQuery(query,UserID,1);
//	printf("courseNum-+++%s\n",courseNum);
    query_after_process = processQuery(query_after_process,courseNum,2);
    //printf("-****%s\n",query_after_process);
    queryDB(query_after_process,1);
    free(query);
    free(query_after_process);

}
void showPersonDtl(char* userID)
{
    char* query;
    char* query_after_process;
    query = readQuery("./sql_script/showPersonDtl.sql");
    query_after_process = processQuery(query,userID,1);
    queryDB(query_after_process,1);
    free(query);
    free(query_after_process);
}

void personalDetl(char* userID)
{
// The P ERSONAL D ETAILS screen shall show the current personal record of the
// student and allow him/her to change his/her password and address. On
// submission, the corresponding Student record shall be updated accordingly.
// Students cannot change their student id or name.
    printf("---------------------Personal Detail---------------------\n");
    showPersonDtl(userID);
    int back =0;
    while(!back) {
        printf("---------------------Personal Detail Options---------------------\n");
        printf("1. Change Password\n");
        printf("2. Change Address\n");
        printf("3. Go back to [Student Main Menu]\n");
        printf("Please enter your choice.\n");
        int choice;
        char courseNum[10];
        scanf("%d",&choice);
        int confirm=0;
        char oldPassword[PASSWORD_LENGTH];
        char newPassword[PASSWORD_LENGTH];
        char confirmPassword[PASSWORD_LENGTH];
        char newAddr[70];
        char* query;
        char* query_after_process;

        switch(choice) {
        case 1:
            //query the db to change the password, update table
            printf("Please input your old password\n");
            scanf("%s",oldPassword);
            if(checkLogin(userID,oldPassword)) {
                while(1) {
                    printf("Please input your NEW password\n");
                    scanf("%s",newPassword);
                    printf("Confirm your NEW password\n");
                    scanf("%s",confirmPassword);
                    if(strcmp(newPassword,confirmPassword)!=0) {
                        printf("Your new password not same, please input again\n");
                    } else {
                        //update table to new password
                        //form update query
                        query = readQuery("./sql_script/updatePassword.sql");
                        //TODO: 2 argument
                        query_after_process = processQuery(query,userID,1);
                        query_after_process = processQuery(query_after_process,newPassword,2);
                        queryDB(query_after_process,0);
                        free(query);
                        free(query_after_process);
                        break;
                    }
                }
            } else {
                //old password not right
                printf("Warning : Your old password not right\n");
            }
            break;
        case 2:
            //Set new Address,
            while(1) {
                printf("Please input your NEW ADDRESS\n");
                scanf("%s",newAddr);
                printf("Confirm your new address: %s\n",newAddr);
                printf("YES: 1 ,    NO:0");
                scanf("%d",&confirm);
                if (confirm ==1) {
                    //update table to newaddr, using transaction
                    //read query from file
                    query = readQuery("./sql_script/updateAddr.sql");
                    //TODO: 3rd argument
                    query_after_process = processQuery(query,userID,1);
                    query_after_process = processQuery(query_after_process,newAddr,2);
                    queryDB(query_after_process,0);
                    //free the pointer
                    free(query);
                    free(query_after_process);
                    //after update , refresh the page and print new personal detail
                    showPersonDtl(userID);
                    break;
                }
            }
        case 3:
            back = 1;
            break;
        }
    }
}
void enrollPage(char* userID)
{
// The ENROLL screen shall allow logged-in students to enrollPage in a new course.
// Students shall be able to select a specific course offering in this screen, but only
// subject offerings of the current year and quarter or the following quarter shall be
// presented (again: don’t hard-code these years but determine year and quarter
// programmatically from the current date using the clock). Students can only enrollPage
// in courses whose direct pre-requisites they have passed (not failed or
// incomplete) and whose maximum enrollment number has not been reached (i.e.
// MaxEnrollment > Enrollment). On successful enrollment, a new entry in the
// Transcript table shall be created with a NULL grade, plus the Enrollment attribute
// of the corresponding course shall be increased by one. In case the student
// cannot enrollPage because he/she has not cleared the prerequisites, print those
// prerequisites on the screen. Implement this part using stored procedures and
// call it from your database client program.

    printf("----------------------------Enroll or Withdraw--------------------------\n");
    Qtr* queryTime;
    queryTime = checkQuarter();
    int choice=0;
    int back=0;
    char* query;
    char* query_after_process;
    while(choice!=1 && choice!=2) {
        printf("Please choose Quarter\n");
        printf("1. %s, %s\n", queryTime->quarter, queryTime->year);
        printf("2. %s, %s\n", queryTime->nextQtr->quarter, queryTime->nextQtr->year);
        scanf("%d",&choice);
        if (choice==1) {
            printf("Here are the courses you have enrolled in %s, %s\n", queryTime->quarter, queryTime->year);
            query = readQuery("./sql_script/showCurrCour.sql");
            //process the query to add new information
            query_after_process = processQuery(query,userID,1);
            query_after_process = processQuery(query_after_process,queryTime->quarter,2);
            query_after_process = processQuery(query_after_process,queryTime->year,3);
        } else if(choice==2) {
            printf("Here are the courses you have enrolled in %s, %s\n", queryTime->nextQtr->quarter, queryTime->nextQtr->year);
            query = readQuery("./sql_script/showCurrCour.sql");
            //process the query to add new information
            query_after_process = processQuery(query,userID,1);
            query_after_process = processQuery(query_after_process,queryTime->nextQtr->quarter,2);
            query_after_process = processQuery(query_after_process,queryTime->nextQtr->year,3);
            strcpy(queryTime->quarter,queryTime->nextQtr->quarter);
            strcpy(queryTime->year,queryTime->nextQtr->year);


        }
    }
    char * temp = query_after_process;
    queryDB(query_after_process,1);
    // provide options
    int choice1;
    char courseNum[10];
    bool enrollSucceed;
    int i =0;
    while(!back) {
        printf("---------------------Enrollment/Withdraw Options---------------------\n");
        printf("1. View New Courses\n");
        printf("2. Enroll\n");
        printf("3. Drop\n");
        printf("4. Go back to [Student Main Menu]\n");
        printf("Please enter your choice.\n");
        scanf("%d",&choice1);
        switch(choice1) {
        case 1:
            //make query display new courses in current quarter
            query = readQuery("./sql_script/showNewCour.sql");
            if (choice==1) {
                //current quarter
                //process query
                query_after_process = processQuery(query,userID,1);
                query_after_process = processQuery(query_after_process,queryTime->quarter,2);
                query_after_process = processQuery(query_after_process,queryTime->year,3);

            } else {
                //next quarter
                //process query
                query_after_process = processQuery(query,userID,1);
                query_after_process = processQuery(query_after_process,queryTime->nextQtr->quarter,2);
                query_after_process = processQuery(query_after_process,queryTime->nextQtr->year,3);

            }
            queryDB(query_after_process,1);

            free(query);
            free(query_after_process);
            break;
        case 2:
            printf("Please input the course number you want to enroll\n");
            //make update query to DB , enroll courses
            scanf("%s",courseNum);

            query = readQuery("./sql_script/enroll.sql");
            query_after_process = processQuery(query,userID,1);
            query_after_process = processQuery(query_after_process,courseNum,2);
            query_after_process = processQuery(query_after_process,queryTime->quarter,3);
            query_after_process = processQuery(query_after_process,queryTime->year,4);
            enrollSucceed = enrollDB(query_after_process);
            free(query);
            free(query_after_process);
            if (!enrollSucceed) {
                printf("\n\nYou can't be enrolled in this course.\n");
                printf("Because you don't meet the requirement\n");

                query = readQuery("./sql_script/showPrereq.sql");
                query_after_process = processQuery(query,courseNum,1);
                queryDB(query_after_process,1);
                free(query);
                free(query_after_process);
            }
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("Here are the courses you have enrolled in %s, %s\n", queryTime->quarter, queryTime->year);
            queryDB(temp,1);

            break;
        case 3:
            printf("Please input the course number you want to drop\n");
            //make update query to DB , drop courses
            scanf("%s",courseNum);
            if(!checkCourse(userID,queryTime,courseNum)) {
                printf("You can't drop a completed course.\n");
                break;
            }
            query = readQuery("./sql_script/drop.sql");
            //process query
            query_after_process = processQuery(query,userID,1);
            query_after_process = processQuery(query_after_process,courseNum,2);
            query_after_process = processQuery(query_after_process,queryTime->quarter,3);
            query_after_process = processQuery(query_after_process,queryTime->year,4);
            printf("%s\n",query_after_process);
            queryDB(query_after_process,0);
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("Here are the courses you have enrolled in %s, %s\n", queryTime->quarter, queryTime->year);

            queryDB(temp,1);
            free(query);
            free(query_after_process);
            break;
        case 4:
            back = 1;
            break;
        }
    }
    //free the pointer
    free(temp);
    free(queryTime->nextQtr);
    free(queryTime);
}
bool enrollDB(char * query)
{
    MYSQL *con = mysql_init(NULL);
    if (con == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }
    mysql_real_connect(con, "localhost", "root", "lhp920428",
                       "project3-nudb", 0, NULL, 0) ;

    mysql_query(con, "SET @pre = 0");
    mysql_query(con,query);
    mysql_query(con,"select @pre");
    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        finish_with_error(con);
    }

    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;
    row = mysql_fetch_row(result);

    if(strcmp(row[0],"100")==0) {
        //check enought student enrolled
        mysql_free_result(result);
        mysql_query(con,"select * from monitor where  Num = 1");
        result = mysql_store_result(con);
        if (result == NULL) {
            finish_with_error(con);
        }
        num_fields = mysql_num_fields(result);
        MYSQL_ROW row;
        row = mysql_fetch_row(result);
        if (strcmp(row[4] , "1")==0) {
            printf("\n---------------------------WARNING-------------------------------------\n");
            printf("Course %s  in %s,%s has low enrollment rate, which is lower than %%50\n",row[1],row[2],row[3]);
        }
        mysql_query(con,"update monitor set flag = 0 where Num = 1");
        mysql_close(con);
        return true;
    } else {
        mysql_free_result(result);
        mysql_close(con);
        return false;
    }

}
bool checkCourse(char* userID,Qtr* queryTime,char* courseNum)
{
    char* query;
    char* query_after_process;

    MYSQL *con = mysql_init(NULL);
    if (con == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }

    if (mysql_real_connect(con, "localhost", "root", "lhp920428",
                           "project3-nudb", 0, NULL, 0) == NULL) {
        finish_with_error(con);
    }
    // //make query
    query = readQuery("./sql_script/checkCourse.sql");

    // //processQuery
    query_after_process = processQuery(query,userID,1);

    query_after_process = processQuery(query_after_process,courseNum,2);

    query_after_process = processQuery(query_after_process,queryTime->year,3);

    if (mysql_query(con, query_after_process)) {
        finish_with_error(con);
    }
    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        //if it drop a course not existed
        mysql_free_result(result);
        mysql_close(con);
        free(query);
        free(query_after_process);
        return false;
    }
    MYSQL_ROW row;
    row = mysql_fetch_row(result);
    if (row[0]!=NULL) {
        mysql_free_result(result);
        mysql_close(con);
        free(query);
        free(query_after_process);
        return false;
    }
    mysql_free_result(result);
    mysql_close(con);
    free(query);
    free(query_after_process);
    return true;


}
void checkTrigger()
{

}

char* readQuery(char* fileName)
{
    char ch;
    FILE *fp;
    int length;
    char* file_name= fileName;
    fp = fopen(file_name,"r"); // read mode

    fseek(fp,0,SEEK_END);
    length = ftell(fp);
    char* query= (char*)malloc(sizeof(char)*length+1);
    query[length]='\0';
    if( fp == NULL ) {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }
    fseek(fp,0,SEEK_SET);
    int i=0;
    while( ( ch = fgetc(fp) ) != EOF ) {
        query[i] = ch;
        i++;
    }
    fclose(fp);
    return query;
}

Qtr* checkQuarter()
{
    Qtr* res = (Qtr*)malloc(sizeof(Qtr));
    Qtr* next = (Qtr*)malloc(sizeof(Qtr));
    int temp_year;
    res->nextQtr = next;
    time_t curTime;
    struct tm *localTime;
    time(&curTime);
    localTime = localtime(&curTime);

    //if tm_mon, tm_year (years since 1900)
    temp_year = 1900 + localTime->tm_year;
    char buf[5];
    // int to string
    sprintf(buf, "%d", temp_year);
    strcpy(res->year, buf);
    memset(res->quarter, '\0', sizeof(res->quarter));
    memset(res->nextQtr->quarter,'\0',sizeof(res->nextQtr->quarter));
    if( 0 <= localTime->tm_mon && localTime->tm_mon <= 2) {
        strcpy(res->qtr,"2");
        strcpy(res->quarter,"Q2");
        strcpy(res->nextQtr->qtr,"3");
        strcpy(res->nextQtr->quarter,"Q3");
        strcpy(res->nextQtr->year,buf);
    } else if(3<=localTime->tm_mon && localTime->tm_mon<=5) {
        strcpy(res->qtr,"3");
        strcpy(res->quarter,"Q3");
        strcpy(res->nextQtr->qtr,"4");
        strcpy(res->nextQtr->quarter,"Q4");
        strcpy(res->nextQtr->year,buf);
    } else if(6<=localTime->tm_mon && localTime->tm_mon <=7) {
        strcpy(res->qtr,"4");
        strcpy(res->quarter,"Q4");
        strcpy(res->nextQtr->qtr,"1");
        strcpy(res->nextQtr->quarter,"Q1");
        char temp[5];
        temp_year+=1;
        sprintf(temp, "%d", temp_year);
        strcpy(res->nextQtr->year, temp);

    } else {
        strcpy(res->qtr,"1");
        strcpy(res->quarter,"Q1");
        strcpy(res->nextQtr->qtr,"2");
        strcpy(res->nextQtr->quarter,"Q2");
        strcpy(res->nextQtr->year,buf);
    }
    return res;
}
