#ifndef CORE_CRUD_H
#define CORE_CRUD_H


#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sqlite3.h>

#define DB_NAME "/home/camel/Desktop/extra/C/C-Programming-Practice/projs/024-sqlite-todo/db/todo.sqlite3"
#define CYAN "\033[1;36m"
#define RESET "\033[0m"
#define TABLE "todos"

typedef enum
{
  Ok,
  Err
} DB_STATUS;


typedef enum
{
  INCOMPLETE,
  COMPLETED,
  CANCELLED,
} STATUS;

sqlite3 *open_db(char *filename);
void close_db(sqlite3 *db);


DB_STATUS insert_handler(sqlite3 *db);
DB_STATUS view_handler(sqlite3 *db);
DB_STATUS delete_haneler(sqlite3 *db) ;
DB_STATUS update_handler(sqlite3 *db);

#endif // CORE_CRUD_H