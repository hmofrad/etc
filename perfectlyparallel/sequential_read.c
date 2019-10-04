/*
 * File: sequential_read.c
 * Comile: gcc -o sequential_read sequential_read.c
 * Run:    ./sequential_read -f input.txt
 * (c) Mohammad H Mofrad, 2017
 * (e) mohammad.hmofrad@pitt.edu
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
 
 int main(int argc, char *argv[]) {
    if((argc != 5) || !(!strcmp(argv[1],"-f") || !strcmp(argv[3],"-s"))) {
        fprintf(stderr, "USAGE: %s -f <file> -s <buffersize>\n", argv[0]);
        int i = 0;
        for(i = 0; i < argc; i++)
            fprintf(stderr, "argv[%d]: %s\n", i, argv[i]);
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));
    struct timeval t1, t2;
	double elapsedTime;
    gettimeofday(&t1, NULL);    

    char *file_name = argv[2];
    FILE *file_descriptor = fopen(file_name,"r");
    if(!file_descriptor) {
        fprintf(stderr, "Error: opening \"%s\"\n", file_name);
        return(EXIT_FAILURE);
    }

    fseek(file_descriptor, 0L, SEEK_END);
    long long file_length = ftell(file_descriptor);
    rewind(file_descriptor);
    unsigned char *data = malloc((file_length + 1) * sizeof(unsigned char));
    long long read_length = 0;
    int num_chunks = 0;
    
    long long buffer_size = atoi(argv[4]);
    unsigned char buffer[buffer_size + 1];
    memset(buffer, '\0', buffer_size);
    
    do {
        read_length = fread(buffer, buffer_size, 1, file_descriptor);
        if (read_length < 0)
        {
            fprintf(stderr, "Error: Reading %s\n", file_name);
            fclose(file_descriptor);
            return (EXIT_FAILURE);
        }
        buffer[buffer_size] = '\0';
        memcpy(data + (num_chunks * buffer_size), buffer, strlen((const char *) buffer));
        memset(buffer, '\0', buffer_size);
        num_chunks++;
    } while(read_length == 1);
    data[file_length] = '\0';
    //printf("%s", data);
    
	if(!fclose(file_descriptor)) {
         ; // NoOp, we're good to go!
    } else {
        fprintf(stderr, "Error: Closing \"%s\"\n", file_name);
        return(EXIT_FAILURE);
    }
    free(data);
	
    gettimeofday(&t2, NULL);
    elapsedTime  = (t2.tv_sec - t1.tv_sec) * 1000;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000;   // us to ms
    printf("%.0f milliseconds | %.2lf seconds\n", elapsedTime, elapsedTime / 1000);
    
    return(EXIT_SUCCESS);
}