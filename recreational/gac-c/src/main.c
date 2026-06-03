
#define _GNU_SOURCE 
#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ADD_CMD "git add ."
#define COMMIT_CMD "git commit -m"
#define PUSH_CMD "git push origin"
#define BRANCH_CMD "git branch --show-current"

bool add();
bool commit(const char *commit_message);
bool push(const char *branch);
char *get_branch();

int main(int argc, char *argv[]) {
    if(argc < 2) {
        fprintf(stderr, "USAGE: %s \"commit message\" \n", argv[0]);
        fprintf(stderr, "OR: %s \"commit message\" \"branch-name\" (or 1 for current branch)\n", argv[0]);
        
        goto failure;
    }
    
    if(argc == 2) {
        bool response = add();
        if(!response) {
            perror("FAILED ADD");
            goto failure;
        }
        response = commit(argv[1]);
        if(!response) {
            fprintf(stderr, "FAILED COMMIT\n");
            goto failure;
        }
        goto success;
    }

    if(argc > 2) {
        bool response = add();
        if(!response) {
            perror("FAILED ADD");
            goto failure;
        }
        response = commit(argv[1]);
        if(!response) {
            fprintf(stderr, "FAILED COMMIT\n");
            goto failure;
        }
        char *branch = strcasecmp(argv[2], "1") == 0 ? get_branch() : argv[2] ;
        response = push(branch);
        if(!response) {
            perror("FAILED PUSH");
            goto failure;
        }
        goto success;
    }

    success:
        return EXIT_SUCCESS;
    failure:
        return EXIT_FAILURE;
    
}


bool add() {
    printf("adding...\n");
    int res = system(ADD_CMD);
    return res == 0;
}

bool commit(const char *commit_message){
    printf("committing...\n");
    char *cmd ;
    if(0 > asprintf(&cmd, COMMIT_CMD "\"%s\"", commit_message)) {
        return false;
    }
    
    int res = system(cmd);
    free(cmd);
    return res == 0;
}

bool push(const char *branch) {
    printf("pushing to \033[1m`%s`\033[0m\n", branch);
    char *cmd;
    if(0 > asprintf(&cmd, PUSH_CMD " %s", branch)) {
        return false;
    }
    int res = system(cmd);
    free(cmd);
    return res == 0;
}

char *get_branch_() {
    char *branch = calloc(1, 1024);
    FILE *fp = popen(BRANCH_CMD, "r");
    if(fp == NULL) {
        perror("FAILED TO OPEN FP FOR BRANCH");
        return NULL;
    }

    if(fgets(branch, sizeof(branch), fp)) {
        branch[strcspn(branch, "\n")] = '\0';
        pclose(fp);
        return branch;
    }
    return NULL;
}

char *get_branch() {
    char *branch = calloc(1, 1024);

    FILE *fp = popen(BRANCH_CMD, "r");
    if (!fp) {
        perror("FAILED TO OPEN FP FOR BRANCH");
        free(branch);
        return NULL;
    }

    if (fgets(branch, 1024, fp) == NULL) {
        perror("FAILED TO READ BRANCH");
        pclose(fp);
        free(branch);
        return NULL;
    }

    pclose(fp);

    branch[strcspn(branch, "\n")] = '\0';
    branch[strcspn(branch, "\r")] = '\0';

    return branch;
}