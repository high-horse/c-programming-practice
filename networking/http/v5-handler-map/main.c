#include "libs/uthash.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define PORT 9990
#define BACKLOG 10
#define BUFFER_SIZE 1024

typedef void *(*handler_fn)(void *ctx);
const char *html_heading = "HTTP/1.1 200 OK\r\n"
                           "Content-Type: text/plain\r\n"
                           "\r\n";

typedef struct {
  int client_fd;
  char method[10];
  char route[256];
  char version[10];
} Client;

typedef struct {
  char *route;
  handler_fn handler;
  UT_hash_handle hh;
} RouteHandlerMap;

RouteHandlerMap *handlerMap = NULL;

void add_handler(RouteHandlerMap **map, const char *name, handler_fn fn) {
  RouteHandlerMap *e = malloc(sizeof(RouteHandlerMap));
  e->route = strdup(name);
  e->handler = fn;

  HASH_ADD_KEYPTR(hh, *map, e->route, strlen(e->route), e);
}

handler_fn get_handler(RouteHandlerMap *map, const char *name) {
  RouteHandlerMap *e;
  HASH_FIND_STR(map, name, e);
  if (e)
    return e->handler;

  return NULL;
}

void success_response(Client *client, const char *msg) {
  send(client->client_fd, html_heading, strlen(html_heading), 0);
  if (send(client->client_fd, msg, strlen(msg), 0) < 0) {
    perror("FAILED TO SEND MESSAGE:");
    close(client->client_fd);
    free(client);
    return;
  }
  shutdown(client->client_fd, SHUT_WR);
  close(client->client_fd);
  free(client);
}

void error_response(Client *client, const char *header, const char *msg) {
  send(client->client_fd, header, strlen(header), 0);
  if (send(client->client_fd, msg, strlen(msg), 0) < 0) {
    perror("FAILED TO SEND MESSAGE:");
    close(client->client_fd);
    free(client);
    return;
  }
  shutdown(client->client_fd, SHUT_WR);
  close(client->client_fd);
  free(client);
}

void *home_request(void *ctx) {
  sleep(1); // simulate sleep
  Client *client = (Client *)ctx;
  char *message = "THis is home page\n";
  success_response(client, message);
  return NULL;
}

void *hello_handler(void *ctx) {
  Client *client = (Client *)ctx;

  char *message = "Hello Hello\n";
  success_response(client, message);
  return NULL;
}

void *bye_handler(void *ctx) {
  Client *client = (Client *)ctx;

  char *message = "goodbye\n";
  success_response(client, message);
  return NULL;
}

void *not_found_handler(void *ctx) {
  Client *client = (Client *)ctx;
  const char *not_found_header = "HTTP/1.1 404 Not Found\r\n"
                                 "Content-Type: text/plain\r\n"
                                 "\r\n";

  error_response(client, not_found_header, "route not found");
  return NULL;
}

void *dispatch_handler(void *ctx) {
  Client *client = (Client *)ctx;
  char request_buffer[BUFFER_SIZE];
  int n = recv(client->client_fd, request_buffer, BUFFER_SIZE, 0);
  if (n < 0) {
    perror("FAILED TO RECV:");
    close(client->client_fd);
    free(client);
    return NULL;
  }
  // null terminate the request buffer
  request_buffer[n] = '\0';
  sscanf(request_buffer, "%s %s %s", client->method, client->route,
         client->version);

  handler_fn h = get_handler(handlerMap, client->route);
  if (h) {
    h(client);
  } else {
    printf("ERROR:: HANDLER NOT FOUND FOR ROUTE %s\n", client->route);
    not_found_handler(client);
  }
  return NULL;
}

int register_routes() {
  add_handler(&handlerMap, "/", home_request);
  add_handler(&handlerMap, "/hello", hello_handler);
  add_handler(&handlerMap, "/bye", bye_handler);

  return EXIT_SUCCESS;
}

int main() {
    printf("process is running on pid %d\n", getpid());
  struct sockaddr_in server_addr = {
      .sin_family = AF_INET,
      .sin_port = htons(PORT),
      .sin_addr.s_addr = INADDR_ANY,
  };

  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    perror("FAILED TO CREATE SOCKET:");
    exit(EXIT_FAILURE);
  }

  int err = bind(server_fd, (struct sockaddr *)&server_addr,
                 (socklen_t)sizeof(server_addr));
  if (err != EXIT_SUCCESS) {
    perror("FAILED TO BIND:");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, BACKLOG) != EXIT_SUCCESS) {
    perror("FAILED TO LISTEN ON THE SOCKET:");
    close(server_fd);
    exit(EXIT_FAILURE);
  }
  if (register_routes() != EXIT_SUCCESS) {
    perror("FAILED TO REGISTER ROUTES:");
    exit(EXIT_FAILURE);
  }
  printf("LISTENING FOR CONN IN PORT %d...\n", PORT);

  while (true) {
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int client_fd =
        accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_size);
    if (client_fd < 0) {
      perror("FAILED TO CREATE CLIENT SOCKET:");
      continue;
    }

    printf("ACCEPTED CONN FROM %s:%d\n", inet_ntoa(client_addr.sin_addr),
           ntohs(client_addr.sin_port));

    Client *client = calloc(1, sizeof(Client));
    client->client_fd = client_fd;

    pthread_t new_conn;
    pthread_create(&new_conn, NULL, dispatch_handler, (void *)client);
    pthread_detach(new_conn);
  }

  close(server_fd);
  return EXIT_SUCCESS;
}
