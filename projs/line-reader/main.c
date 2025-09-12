#include <stdio.h>
#include <stdlib.h>

char *readline(FILE *fp);

int main(void)
{
    char *filename = "foo-bar.txt";
    char *mode = "r";
    FILE *fp;

    fp = fopen(filename, mode);
    char *line;
    while ((line = readline(fp)) != NULL)
    {
        printf("%s\n", line);
        free(line);
    }

    fclose(fp);
}

char *readline(FILE *fp)
{
    int offset = 0;
    int bufsize = 4;
    char *buf;

    buf = malloc(bufsize);
    if (buf == NULL)
    {
        return NULL;
    }

    int c;
    while ((c = fgetc(fp)) != '\n' && c != EOF)
    {
        // buffer accounting
        if (offset > bufsize - 1)
        {
            bufsize += 2;
            char *new_buf = realloc(buf, bufsize);
            if (new_buf == NULL)
            {
                free(buf);
                return NULL;
            }

            buf = new_buf;
        }

        buf[offset] = c;
        offset++;
    }

    if (c == EOF && offset == 0)
    {
        free(buf);
        return NULL;
    }

    // trim the buffer
    if (offset < bufsize - 1)
    {
        char *new_buf = realloc(buf, (offset + 1));
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