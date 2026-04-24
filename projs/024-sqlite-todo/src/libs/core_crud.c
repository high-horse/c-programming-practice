#include "core_crud.h"

sqlite3 *open_db(char *filename)
{
  sqlite3 *db;
  int rc;

  rc = sqlite3_open(filename, &db);
  if (rc != SQLITE_OK)
  {
    fprintf(stderr, "Failed to open db file %s , %s", filename, sqlite3_errmsg(db));
    sqlite3_close(db);
    exit(EXIT_FAILURE);
  }

  return db;
}

void close_db(sqlite3 *db)
{
  sqlite3_close(db);
}

void new_trimline(char *str) {
  if(!str) return;
  str[strcspn(str, "\n")] = '\0';
}


DB_STATUS insert_handler(sqlite3 *db)
{
  char *title = NULL, *desc = NULL;
  size_t len = 0, read = 0;

  printf("" CYAN "\ttitle:" RESET " ");

  read = getline(&title, &len, stdin);
  if (read < 0)
  {
    perror("FAILED TO READ LINE");
    goto error_cleanup;
  }
  new_trimline(title);

  read = 0;
  len = 0;
  printf("" CYAN "\tdesc:" RESET " ");
  read = getline(&desc, &len, stdin);
  if (read < 0)
  {
    perror("Failed to read desc");
    goto error_cleanup;
  }
  new_trimline(desc);

  sqlite3_stmt *res;
  char *query = "INSERT INTO todos (title, description) VALUES (? , ?);";

  int rc = sqlite3_prepare_v2(db, query, -1, &res, NULL);
  if (rc == SQLITE_OK)
  {
    sqlite3_bind_text(res, 1, title, -1, SQLITE_STATIC);
    sqlite3_bind_text(res, 2, desc, -1, SQLITE_STATIC);

    if (sqlite3_step(res) != SQLITE_DONE)
    {
      perror("failed to create row");
      goto error_cleanup;
    }

    sqlite3_finalize(res);
  }
  goto ok_cleanup;

ok_cleanup:
  if (title)
    free(title);
  if (desc)
    free(desc);
  return Ok;

error_cleanup:
  if (title)
    free(title);
  if (desc)
    free(desc);
  return Err;
}


DB_STATUS view_handler(sqlite3 *db)
{

  int rc = 0;
  const char *query = "SELECT * FROM todos;";
  sqlite3_stmt *res;

  rc = sqlite3_prepare_v2(db, query, -1, &res, NULL);
  if (rc != SQLITE_OK)
  {
    perror("failed to prepare view query");
    return Err;
  }

  printf("\n" CYAN "All Todos:" RESET "\n");
  printf("--------------------------------------------------------------------------------\n");
  printf(CYAN "%-10s %-10s %-50s %-50s\n" RESET, "ID", "Done", "Title", "Description");

  while ((rc = sqlite3_step(res)) == SQLITE_ROW)
  {
    const char *id = sqlite3_column_text(res, 0);
    const char *status_str = sqlite3_column_text(res, 3);
    int status = status_str ? atoi(status_str) : 0;
    char *status_label = status == 1 ? "[*]" : "[ ]";
    const char *title = sqlite3_column_text(res, 1);
    const char *desc = sqlite3_column_text(res, 2);
    printf("%-10s %-10s %-50s %-50s\n", id, status_label, title, desc);
  }
  printf("--------------------------------------------------------------------------------\n");
  sqlite3_finalize(res);

ok_cleanup:
  return Ok;

error_cleanup:
  return Err;
}



DB_STATUS delete_haneler(sqlite3 *db) {
    char id[10];
    printf("" CYAN "\tTodo ID to delete:" RESET " ");
    scanf("%s", id);
    int del_id = atoi(id);
    const char *query = "DELETE FROM todos where id = ?;";
    sqlite3_stmt *stmt;
    
    if(sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return Err;
    }
    
    sqlite3_bind_int(stmt, 1, del_id);

    if(sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        goto error_cleanup;
    }
    
    goto  ok_cleanup;

    ok_cleanup:
        sqlite3_finalize(stmt);
        return Ok;
        
    error_cleanup:
        sqlite3_finalize(stmt);
        return Err;
}


DB_STATUS update_handler(sqlite3 *db)
{
    char id[10];
    printf("[" CYAN "0" RESET "] Incomplete  [" CYAN "1" RESET "] Mark Completed  \n");
    printf("" CYAN "\tTodo ID to update:" RESET " ");
    scanf("%s", id);
    int update_id = atoi(id);
    printf("" CYAN "\tTodo status to update:" RESET " ");
    scanf("%s", id);
    int status = atoi(id);
    if(status != 0 && status != 1) {
        fprintf(stderr, "Invalid status: %d\n", status);
        return Ok;
    }

    printf("todo to update |%d|\n", update_id);
    const char *sql = "UPDATE todos SET status = ? WHERE id = ?;";
    sqlite3_stmt *stmt;
    
    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        goto err_cleanup;
    }
    
    sqlite3_bind_int(stmt, 1, status);
    sqlite3_bind_int(stmt, 2, update_id);
    
    if(sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Failed to update todo: %s\n", sqlite3_errmsg(db));
        goto err_cleanup;
    }
    
    ok_cleanup:
        sqlite3_finalize(stmt);
        return Ok;
    
    err_cleanup:
        sqlite3_finalize(stmt);
        return Err;
}