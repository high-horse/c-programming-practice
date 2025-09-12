#include<stdio.h>
#include<stdlib.h>

int main(void) {
    float *fp = calloc(20, sizeof(*fp));
    for (int i = 0; i < 20; i++)
    {
        fp[i] = 1000.0 / i;
    }

    float *fp_add = realloc(fp, sizeof(*fp) * 40);
    if(fp_add == NULL) {
        printf("Could not reallocate more mem\n"); return 1;
    } 

    // it copies and pointes to the same memory as fp.
    fp = fp_add;
    for (int i = 0; i < 40; i++)
    {
        printf("%d => %f\t", i, fp[i]);
    }
    printf("\n");
    
    free(fp);
}