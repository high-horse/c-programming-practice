#include <stdio.h>
#include<stdlib.h>

char *readLine(FILE *fp);

int main()
{
    char *filename = "foo-bar.txt";
    char *mode = "r";

    FILE *fp;

    fp = fopen(filename, mode);
    char *line;
    while ((line = readLine(fp)) != NULL)
    {
        printf("%s \n", line);
    }
    
}

char *readLine(FILE *fp) {
    int offset = 0; // intes

    int bufsize = 4; // initial size of buffer
    char *buf; // buffer
    int c; // characater we read

    buf = malloc(bufsize);
    if(buf == NULL){
        return NULL;
    }

    // while (c = fgetc(fp) &&  c != '\n' && c != EOF)
    while ((c = fgetc(fp)) != '\n' && c != EOF)
    {
        // check if we are out of buffer
        // extra index for NUL terminator
        if(offset == bufsize -1){
            bufsize = bufsize *2;

            char *new_buf = realloc(buf, bufsize);
            if (new_buf == NULL){
                free(buf);
                return NULL;
            }

            buf = new_buf;
        }

        // buf[offset++] = c
        buf[offset] = c;
        offset++;
    }

    // hit eof, 
    if (c == EOF && offset == 0)
    {
        free(buf);
        return NULL;
    }

    // shrink buffer 
    if (offset < (bufsize -1))
    {
        char *new_buf = realloc(buf, offset + 1); // extra 1 for null terminator
        if (new_buf == NULL)
        {
            free(buf);
            return NULL;
        }
        
        buf = new_buf;
    }
    
    buf[offset] = '\0';
    return buf;
    
}