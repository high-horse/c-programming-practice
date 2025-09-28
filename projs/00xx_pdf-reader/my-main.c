#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    // char *filename = "/home/camel/Desktop/naamsari-certificate.pdf";
    // char *filename = "/home/camel/Desktop/Akhilesh-Adhikari.pdf";
    // char *filename = "/home/camel/Desktop/office-documents/Enhanced-Master-Mediator-Controller:Complete-Technical-Documentation.pdf";
    char *filename = "/home/camel/Desktop/office-documents/decompressed-sample.pdf";

    char *mode = "rb";
    FILE *fp;

    fp = fopen(filename, mode);
    if(!fp){
        perror("error opening pdf file");
        return 1;
    }
    int bufsize = 2048;
    int offset = 0;
    char *buf = calloc(1, bufsize);

    size_t bufread =  fread(buf, 1, bufsize, fp);
    // printf("%zu\n", bufread);
    
    // printf("\n\n\n%s\n", buf);
    printf("\n\n\n");
    fwrite(buf, 1, bufsize, stdout);
    printf("\n\n\n");

    free(buf);
    fclose(fp);
    printf("\n\ncompleted\n");
}