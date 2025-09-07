#include <stdio.h>

int main(void)
{
    char *out = "txt/bin/output.bin";
    char *mode = "wb";
    unsigned short v = 0x1234; // Two bytes, 0x12 and 0x34

    FILE *fp;

    fp = fopen(out, mode);
    fwrite(&v, sizeof(v), 1, fp);
    fclose(fp);
}

void len_1_write_binary(void)
{
    char *output = "txt/bin/output.bin";
    char *mode = "wb";
    FILE *fp;
    unsigned char bytes[6] = {5, 37, 0, 88, 255, 12};

    fp = fopen(output, mode);

    // params
    // pointer of bytes
    // size of individual byte
    // length of bytes
    // pointer to stream.
    fwrite(bytes, sizeof(char), 6, fp);

    fclose(fp);
}

void len_2_read_binary(void)
{
    char *output = "txt/bin/output.bin";
    char *mode = "rb";
    unsigned char c;
    FILE *fp;

    fp = fopen(output, mode);

    // params
    // pointer to destination where value to set
    // size of each element
    // number of elements to read
    // pointer to stream
    while (fread(&c, sizeof(char), 1, fp) > 0)
    {
        printf("%d\t", c);
    }
    printf("\n");

    fclose(fp);
}