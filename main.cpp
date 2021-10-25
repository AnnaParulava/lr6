#include <stdio.h>
#include <string>
#include <stdlib.h>
#include "sqlite3.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

void CreateTable(sqlite3 *db, char *zErrMsg, int rc, const char* data)
{
    //Создание таблицы
   char *sql = "DROP TABLE IF EXISTS TECHNICAL_MEANS;" //Удаление таблицы, если сущесвует
      "CREATE TABLE TECHNICAL_MEANS("  \
      "ID INT PRIMARY KEY     NOT NULL," \
      "NAME           TEXT    NOT NULL," \
      "AUTHOR         TEXT    NOT NULL," \
      "YEAR           INT     NOT NULL," \
      "NUMBER         INT     NOT NULL);";

   // Выполнить инструкцию SQL
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Table created successfully\n");
   }
}

void FillTable(sqlite3 *db, char *zErrMsg, int rc, const char* data)
{
   //Заполнение таблицы
   char *sql = "INSERT INTO TECHNICAL_MEANS (ID,NAME,AUTHOR,YEAR,NUMBER) "  \
         "VALUES (1, 'AdobePhotoshop', 'AdobeSystems', 1988 , 7567); " \
         "INSERT INTO TECHNICAL_MEANS (ID,NAME,AUTHOR,YEAR,NUMBER) "  \
         "VALUES (2, 'Code::Blocks', 'TheCode::BlocksTeam', 2017, 83567); "\
         "INSERT INTO TECHNICAL_MEANS (ID,NAME,AUTHOR,YEAR,NUMBER)" \
         "VALUES (3, 'KOMPAS-3D', 'ASCON', 2008, 43513);";

   // Выполнить инструкцию SQL
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Records created successfully\n");
   }
}

void SQlOperations(sqlite3 *db, char *zErrMsg, int rc, char *sql, const char* data){

   // Выполнить инструкцию SQL
   rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);

   if( rc != SQLITE_OK ) {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Operation done successfully\n");
   }
}

int main(int argc, char* argv[]) {
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   const char* data = "Callback function called";

   //Открытие БД
   rc = sqlite3_open("technical_means.db", &db);

   if( rc ) {fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db)); return 1;}
   else {fprintf(stderr, "Opened database successfully\n");}

   CreateTable(db, zErrMsg, rc, data); //Создание таблицы
   FillTable(db, zErrMsg, rc, data); //Заполнение таблицы

   /* Запросы к БД */

   //Вывод таблицы
   char *sqlSelect = "SELECT * from TECHNICAL_MEANS";

   //Изменение таблицы
   char *sqlUpdate = "UPDATE TECHNICAL_MEANS set NUMBER = 7567 where ID=2; ";

   //Удаление таблицы
   char *sqlDelete = "DELETE from TECHNICAL_MEANS where ID=2; ";

   //Добавление записи
   char *sqlInsert =  "INSERT INTO TECHNICAL_MEANS (ID,NAME,AUTHOR,YEAR,NUMBER)" \
         "VALUES (4, 'LabVIEW', 'NationalInstrument', 2004, 6756); ";

   //Вывод таблицы по возрастанию
   char *sqlSelectASC = "SELECT * from TECHNICAL_MEANS ORDER BY name ASC;";

   //Вывод таблицы по убыванию
   char *sqlSelectDESC = "SELECT * from TECHNICAL_MEANS ORDER BY name DESC;";

   //Вывод самого старого тех средства
   char *sqlSelectMIN_YEAR = "SELECT * from TECHNICAL_MEANS "\
                             "WHERE TECHNICAL_MEANS.YEAR = (SELECT MIN(TECHNICAL_MEANS.YEAR) from TECHNICAL_MEANS);";

   //Поиск по двум условиям: производитель и год
   char *sqlSelectFIND = "SELECT * from TECHNICAL_MEANS "\
                             "WHERE TECHNICAL_MEANS.AUTHOR = 'ASCON' AND TECHNICAL_MEANS.YEAR = 2008;";

   //Сколько различных значение в поле с повторяющимися значениям; сколько раз встречается конкретное значение в этом поле
   char *sqlSelectDISTINCT = "SELECT COUNT(DISTINCT TECHNICAL_MEANS.NUMBER) AS 'Number of different values' FROM TECHNICAL_MEANS;" \
                             "SELECT COUNT(NUMBER) AS 'Number of times a specific value occurs'  FROM TECHNICAL_MEANS where TECHNICAL_MEANS.NUMBER = '43513';";

   //Вычислить количество записей в таблице
   char *sqlCounAllValues = "SELECT COUNT(*) AS 'All values' FROM TECHNICAL_MEANS;";

   SQlOperations(db, zErrMsg, rc, sqlSelect, data);
   SQlOperations(db, zErrMsg, rc, sqlUpdate, data);
   SQlOperations(db, zErrMsg, rc, sqlSelectASC, data);
   SQlOperations(db, zErrMsg, rc, sqlSelectDESC, data);
   SQlOperations(db, zErrMsg, rc, sqlSelectMIN_YEAR, data);
   SQlOperations(db, zErrMsg, rc, sqlSelectFIND, data);
   SQlOperations(db, zErrMsg, rc, sqlInsert, data);
   SQlOperations(db, zErrMsg, rc, sqlUpdate, data); //изменение, так чтобы были повторяющиеся значения
   SQlOperations(db, zErrMsg, rc, sqlSelectDISTINCT, data);
   SQlOperations(db, zErrMsg, rc, sqlCounAllValues, data); //До удаления
   SQlOperations(db, zErrMsg, rc, sqlDelete, data);
   SQlOperations(db, zErrMsg, rc, sqlCounAllValues, data);//После удаления

   sqlite3_close(db);

   return 0;
}
