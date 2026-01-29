#include <stdio.h>
#include <stdlib.h>

extern char **environ;

int main(int argc, char **argv, char **env) {
    (void)argc; (void)argv;

    for (char **p = env; *p != NULL; *p ++){
        printf("%s\n", *p);
    }

    // OR 

    for (int i = 0; env[i] != NULL; i++){
        printf("%s\n", env[i]);
    }
}

int _main(void){
    for(int i = 0; environ[i] != NULL; i++) {
        printf("%s\n", environ[i]);
    }
    return 0;
}

void envoron_1() {
    for (char **p = environ; *p != NULL; *p++){
        printf("%s\n", *p);
    }

    // OR 
    for(int i = 0; environ[i] != NULL; i++) {
        printf("%s\n", environ[i]);
    }
}

int environment_variable_basic(void) {
    char *val = getenv("PATH");

    if(val == NULL) {
        printf("cannot find firtox in envoronment variable \n");
        return EXIT_FAILURE;
    }

    printf("Value found \n%s\n", val);
    return EXIT_SUCCESS;
}