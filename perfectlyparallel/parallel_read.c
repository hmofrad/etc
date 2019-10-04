/*
 * main.c
 * (c) Mohammad HMofrad, 2017
 * (e) mohammad.hmofrad@pitt.edu
 */
 
// #include "libpara.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#define VERBOSE
#undef VERBOSE

pthread_mutex_t mutex;
unsigned char *big_buffer;
size_t buffer_size;

struct pthread_args_struct{
	int index;
	long long low;
	long long high;
    FILE *file_descriptor;
};

 void multiprocessing_pool(size_t total_size, int num_chunks, int *k1, size_t *k1_num, int *k2, size_t *k2_num)
{
    // Distributing nodes accross threads
	*k1 = total_size % num_chunks; // mod
	*k1_num = 1 + ((total_size - 1) / num_chunks); //ceiling division
	*k2 = num_chunks - *k1; // diff
	*k2_num = (size_t)((double) (total_size/num_chunks)); //floor division
	
	#ifdef VERBOSE
        printf("Multiprocesing chunks (Starting from 0)\n");
        printf("   Number of chunks  %3d\n", num_chunks);
        printf("   Chunks x size %3d x %3lli\n",  *k1, *k1_num);
        printf("   Chunks x size %3d x %3lli\n",  *k2, *k2_num);
        printf("   Total size %lli == Computed size %lli\n", total_size, (*k1 * *k1_num) + (*k2 * *k2_num));
	#endif
}



void *parallel_read(void *arguments) {
	struct pthread_args_struct *args = arguments;
	int index = args->index;
    (void) index;
	size_t low = args->low;
	size_t high = args->high;
    FILE *file_descriptor = args->file_descriptor;
    
    size_t read_length = 0;
    int num_chunks = 0;
    size_t buffer_length = 0;
    size_t offset = 0;
    size_t total_length = 0;
    size_t read_remained = high - low + 1;
    size_t last_read = (high - low + 1) % buffer_size;
    
    unsigned char buffer[buffer_size + 1];
    memset(buffer, '\0', buffer_size);

    fseek(file_descriptor, low, SEEK_SET);
    do {
        if(read_remained != last_read) {
            read_length = fread(buffer, buffer_size, 1, file_descriptor);
        } else {
            read_length = fread(buffer, read_remained, 1, file_descriptor);
        }
        if (read_length < 0)
        {
            fprintf(stderr, "Error: Reading input file\n");
            fclose(file_descriptor);
            exit(EXIT_FAILURE);
        }
        buffer[buffer_size] = '\0';
        buffer_length = strlen((const char *) buffer);
        
        read_remained -= buffer_length;
        
        offset = low + (num_chunks * buffer_size);
        memcpy(big_buffer + offset, buffer, buffer_length);
        memset(buffer, '\0', buffer_size);
        num_chunks++;
        total_length += buffer_length;
       //if(index == 2)
        //{
         //  pthread_mutex_lock(&mutex);
          // printf(">> %d %lli %d %lli\n", index, (high - low + 1) % buffer_size, num_chunks, read_remained);
            //printf(" %d %lli %lli %lli\n", index, high, total_length, offset + buffer_length);
           // pthread_mutex_unlock(&mutex); 
        //}
        
        //if() {
         //   break;
        //}
    } while((read_length == 1) && read_remained);
    //} while(buffer_length != high);
    
    if(!fclose(file_descriptor))
         ; // NoOp, we're good to go!
    else
    {
        fprintf(stderr, "Error: Closing input file\n");
        exit(EXIT_FAILURE);
    }
    
    //pthread_mutex_lock(&mutex);
    //    printf("%d %lli %lli %lli %lli %lli %lli %lli\n", index, low, high, high - low + 1,  total_length, offset + buffer_length, read_remained, read_length);
    //pthread_mutex_unlock(&mutex);
    
    free(args);
	return(NULL);
}
 


size_t pread(unsigned char *big_buffer, size_t big_buffer_length, size_t buffer_size, int num_threads, char *file_name) {
    unsigned char buffer[buffer_size + 1];
    memset(buffer, '\0', buffer_size);
    
    
    int k1 = 0;
    size_t k1_num = 0;
    int k2 = 0;
    size_t k2_num = 0;
    multiprocessing_pool(big_buffer_length, num_threads, &k1, &k1_num, &k2, &k2_num);
 
    pthread_t tid[num_threads];
	memset(tid, 0, sizeof(tid));
	struct pthread_args_struct *args = NULL;
    int status = 0;
    int i = 0;

    FILE *file_descriptor_per_thread;
    
    for(i = 0; i < num_threads; i++) {
        args = malloc(sizeof(struct pthread_args_struct) * num_threads);
        if(!args) {
            return(EXIT_FAILURE);
        }
        memset(args, 0, sizeof(struct pthread_args_struct));
        
        file_descriptor_per_thread =  fopen(file_name,"r");
        if(!file_descriptor_per_thread)
        {
            fprintf(stderr, "Error: Opening %s\n", file_name);
            return(EXIT_FAILURE);
        }
        
        args->index = i;
        args->file_descriptor = file_descriptor_per_thread;
        
        if((i < k1) && (k1 != 0)) {
            args->low = i * k1_num;
            args->high = ((i + 1)* k1_num) - 1;
            //printf("%d %lli %lli\n", i, args->low, args->high);
        } else {
            args->low = (k1 * k1_num) + ((i - k1) * k2_num);
            args->high = (k1 * k1_num) + (((i + 1) - k1) * k2_num) - 1;
            //printf("%d %lli %lli\n", i, args->low, args->high);
        }
        
        status = pthread_create(&tid[i], NULL, (void*)parallel_read, (void *)args);	
        
        if (status != EXIT_SUCCESS)
        {
            fprintf(stderr, "Error: pthread_create(%d)\n", i);
            exit(EXIT_FAILURE);
        }
    }
    
    for(i = 0; i < num_threads; i++)
    {
        pthread_join(tid[i], NULL);
    }
    big_buffer[big_buffer_length] = '\0';    
    printf("%s\n", big_buffer);
    return(0);
}


 int main(int argc, char *argv[])
{   
    
    int i = 0;
    if((argc != 7) || !(!strcmp(argv[1],"-f") || !strcmp(argv[3],"-s") || !strcmp(argv[5],"-n"))) {
        fprintf(stderr, "USAGE: %s -f <file> -s <buffersize> -n <numthreads>\n", argv[0]);
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
        fprintf(stderr, "Error: Opening \"%s\"\n", file_name);
        return(EXIT_FAILURE);
    }

    fseek(file_descriptor, 0L, SEEK_END);
    size_t big_buffer_length = ftell(file_descriptor);
    
    if(!fclose(file_descriptor)) {
         ; // NoOp, we're good to go!
    } else {
        fprintf(stderr, "Error: Closing %s\n", file_name);
        return(EXIT_FAILURE);
    }
    
    big_buffer = malloc((big_buffer_length + 1) * sizeof(unsigned char));
    
    buffer_size = atoi(argv[4]);
    
    int num_threads = atoi(argv[6]);
    
    pread(big_buffer, big_buffer_length, buffer_size, num_threads, file_name);
    free(big_buffer);
    
	
    gettimeofday(&t2, NULL);
    elapsedTime  = (t2.tv_sec - t1.tv_sec) * 1000;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000;   // us to ms
    printf("%.0f milliseconds | %.2lf seconds\n", elapsedTime, elapsedTime / 1000);
    
    return(EXIT_SUCCESS);
}
