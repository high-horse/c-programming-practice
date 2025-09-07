#include <stdio.h>

int main(void)
{
    
}

// using fgetc
void les_1_single_char(void)
{
    FILE *fp;
    fp = fopen("txt/test.txt", "r");

    // read single character
    int c = fgetc(fp);
    printf("%c\n", c);

    fclose(fp);
}
void les_1_single_line_loop(void)
{

    FILE *fp;
    int c;
    fp = fopen("txt/test.txt", "r");

    while ((c = fgetc(fp)) != EOF)
    {
        printf("%c", c);
    }

    printf("\n");
    fclose(fp);
}

void les_2_read_line(void)
{
    char *fname = "txt/quotes.txt";
    char *mode = "r";
    FILE *fp;
    fp = fopen(fname, mode);

    char stmt[1024];
    int line_count = 0;

    while (fgets(stmt, sizeof stmt, fp) != NULL)
    {
        printf("%d: %s", ++line_count, stmt);
    }

    printf("\n");
    fclose(fp);
}