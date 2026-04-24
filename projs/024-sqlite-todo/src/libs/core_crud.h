#ifndef CORE_CRUD_H
#define CORE_CRUD_H

// Function declarations for CRUD operations
int create_task(const char *description);
int read_tasks();
int update_task(int id, const char *new_description);
int delete_task(int id);

#endif // CORE_CRUD_H