#include <stdio.h>
#include <stdlib.h>

int main(const int argc, const char *argv[]) {
  FILE *fd[argc];
  if (argc == 1) {
    fd[0] = stdin;
  } else {
    for (int i = 0; i < argc-1; i++) {
      fd[i] = fopen(argv[i+1], "r");
      if (fd[i] == NULL) {
        perror("FILE COULD NOT BE OPENED :");
        exit(EXIT_FAILURE);
      }
    }
  }

  int c;
  for (int i = 0; i < argc-1; i++) {
    while ((c = fgetc(fd[i])) != EOF) {
      putc(c, stdout);
    }
  }

  return EXIT_SUCCESS;
}
