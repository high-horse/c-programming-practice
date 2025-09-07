#include <stdio.h>

/*
    read from stream
    fgetc, fgets, and fscanf
    write to stream
    fputc, fputs, and fpritf
*/
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

void les_3_read_lines_format(void)
{

    char *file_name = "txt/whales.txt";
    char *mode = "r";

    FILE *fp;
    char name[1024];
    float length;
    int mass;
    int line = 0;

    fp = fopen(file_name, mode);

    while (fscanf(fp, "%s %f %d", name, &length, &mass) != EOF)
    {
        printf("%d : name %s\t length: %f\t mass: %d\n", ++line, name, length, mass);
    }
    printf("\n");

    fclose(fp);
}

void les_4_write_to_console(void)
{
    FILE *fp;
    // replace thie stdout to fopen to write to file.
    fp = stdout;

    fputc('H', fp);
    fputc('\n', fp);
    fputs("HELLO WORLD\n", fp);
    fprintf(fp, "value of x is %d\n", 6);

    fclose(fp);
}

void les_4_write_to_file(void)
{
    char *file_name = "txt/put.txt";
    char *mode = "w";

    FILE *fp;
    fp = fopen(file_name, mode);

    fputc('A', fp);
    fputc('\n', fp);
    fputs("Hello World\n", fp);

    int a = 5;
    fprintf(fp, "the file path is %s, and this is int a %d", file_name, a);

    fclose(fp);
}