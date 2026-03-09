#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <raylib.h>

int main(int argc, char *argv[]){
    int opt;
    bool c_flag = false, d_flag = false;
    char *input_filename;
    char *file_extension ;
    
    while((opt = getopt(argc, argv, "cdhf:")) != -1) {
        switch (opt) {
            case 'c':
                c_flag = true; 
                file_extension = ".comp";
                break;
            
            case 'd':
                d_flag = true; 
                file_extension = ".decomp";
                break;
            
            case 'f':
                input_filename = optarg; 
                break;
            
            case 'h':
                printf("Usage: %s [-c | -d] -f=filename\n"
                    "\t-d\tdecompress\n"
                    "\t-c\tcompress\n", argv[0]);
                
                exit(EXIT_SUCCESS);
              
            case '?':
                fprintf(stderr, "Unknown option: %c\n", optopt);
                exit(EXIT_FAILURE);
        }
    };
    
    printf("Passed file %s\n", input_filename);
    FILE *input_fp = fopen(input_filename, "r");
    int input_filesize = -1;
    if(input_fp == NULL) {
        perror("Failed to open file to compress:");
        exit(EXIT_FAILURE);
    }

    // output file
    int len = strlen(input_filename) + strlen(file_extension) + 1;
    char output_filename[len];
    snprintf(output_filename, len, "%s%s", input_filename, file_extension);

    FILE *output_fp =  fopen(output_filename, "w");
    
    if(output_fp == NULL) {
        perror("Failed to open output file:");
        exit(EXIT_FAILURE);
    }
    
    const unsigned char *loadedData = LoadFileData(input_filename, &input_filesize);
    if(input_filesize < EXIT_SUCCESS) {
        perror("FAILED TO READ INPUT FILE:");
        exit(EXIT_FAILURE);
    }
    unsigned char *processed_data;
    int processed_datasize = -1;
    if(c_flag) {
        // comporess
        processed_data = CompressData(loadedData, input_filesize, &processed_datasize);
        
    } else if(d_flag){
        // decompress
        processed_data = DecompressData(loadedData, input_filesize, &processed_datasize);
    }
    
    // write the processed to file
    if(fwrite(processed_data, 1, processed_datasize, output_fp) != processed_datasize) {
        perror("FAILED TO WRITE OUTPUT FILE:");
        exit(EXIT_FAILURE);
    }
    printf("Processed file %s \n", output_filename);
    
    return EXIT_SUCCESS;
}