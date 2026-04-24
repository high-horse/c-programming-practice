#define _GNU_SOURCE
#include "libs/core_crud.h"

void create_cli(sqlite3 *db)
{

  char action;
  bool running = true;

  while (running)
  {
    printf("[" CYAN "i" RESET "]nsert  [" CYAN "v" RESET "]iew  [" CYAN "u" RESET "]pdate  [" CYAN "d" RESET "]elete  [" CYAN "q" RESET "]uit\n");
    view_handler(db);
    scanf(" %c", &action);
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
      ;

    switch (action)
    {
    case 'i':
    case 'I':
      if (insert_handler(db) != Ok) running = false;
      break;

    case 'v':
    case 'V':
      view_handler(db);
      break;

    case 'u':
    case 'U':
        update_handler(db);
      break;

    case 'd':
    case 'D':
        if(delete_haneler(db) != Ok) running = false;
      break;

    case 'q':
    case 'Q':
      running = false;
      break;

    default:
      printf("invalid input %c\n", action);
      break;
    }
  }
}


int main(void)
{

  sqlite3 *db = open_db(DB_NAME);
  create_cli(db);

  close_db(db);
}
