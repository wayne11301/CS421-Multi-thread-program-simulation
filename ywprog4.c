/*
This program allow users to simulate a solution to producer-consumer problem by semaphores and mutex lock.
 */

#include <pthread.h>	/* for pthread functions */
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define NUM_THREADS 5
#define buffer_size 5

typedef int buffer_item;
buffer_item buffer[buffer_size];
pthread_mutex_t mutex;
sem_t empty;
sem_t full;

int in = 0;
int out = 0;


int insert_item(buffer_item item){
	// check if buffer is full or not
	bool flag = false;
	int i;
	for(i=0; i<buffer_size;i++){
		if(buffer[i]== -1){
			flag = true;
			break;
		}
	}
	
        if(flag ==false){
        	return 1;
        }
	
	//get lock
	sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        
        //critical section
        
        buffer[in] = item;
        printf("Insert_item inserted item %d at position %d\n", item, in);

        in = (in+1)%buffer_size;
        for(i=0;i<buffer_size;i++){
        	if((buffer[i]) == -1) {
        		printf("[empty]");
        	}else{
        		printf("[%d]", buffer[i]);
        	}
	}
        printf("  in = %d, out = %d\n", in, out);
        
        //release lock
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
	return 0;
}
void *producer(void *param) {
	buffer_item item;
	int num;
	while(1) {
		num = (rand() % 3) + 1;
		/* sleep for random amount of time */
		printf("producer thread %lu sleeping for %d seconds.\n\n", pthread_self(), num);
		sleep(num);
		
		//wake up
		num = (rand() % 100) + 1;
		item = num;
		
		if (insert_item(item)) {
			printf("producer error, buffer full!\n");
		}else{
			printf("producer thread %lu inserted value %d.\n", pthread_self(), num);
		}
	}
}



int remove_item(){
        int i;
        bool flag=false;
        // check if there's any element to remove
        for(i=0;i<buffer_size;i++){
        	if(buffer[i]!=-1){
        		flag = true;
        		break;
        	}
        }
        
        if(flag == false){
        	return 1;
        }

	//get lock
	sem_wait(&full);
        pthread_mutex_lock(&mutex);
        buffer_item item = buffer[out];
        
        //critical section
        
        buffer[out] = -1;
        printf("remove_item removed item %d at position %d\n", (int) item, out);
        out = (out+1)%buffer_size;
        for(i=0;i<buffer_size;i++){
        	if((int)(buffer[i]) == -1) {
        		printf("[empty]");
        	}else{
        		printf("[%d]", buffer[i]);
        	}
	}
        printf("  in = %d, out = %d\n", in, out);

        //release lock
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
	
	return 0;
}

void *consumer(void *param) {
	buffer_item item;
	int num;
	while(1) {
		num = (rand() % 3) + 1;
		/* sleep for random amount of time */
		printf("consumer thread %lu sleeping for %d seconds.\n\n", pthread_self(), num);
		sleep(num);
		
		//wake up
		item = buffer[out];
		if(remove_item()){
			printf("consuming error, buffer empty!\n");
		}else{	
			printf("consumer thread %lu removed value %d.\n", pthread_self(), item);
		}

	}
}





int main(int argc, char *argv[])
{
	//get input from argv[]
	int i, run_time = atoi(argv[1]), producer_num = atoi(argv[2]), consumer_num = atoi(argv[3]), in, out;
	
	//initialize pthread and locks
	pthread_t producer_array[producer_num], consumer_array[consumer_num]; 	/* the thread identifier */
	pthread_attr_t attr; 		/* set of attributes for the thread */

	pthread_mutex_init(&mutex, NULL);
	sem_init(&empty,0,NUM_THREADS);
	sem_init(&full,0,0);
	
	/* get the default attributes */
	pthread_attr_init(&attr);

	// initialize buffer, create producer and consumer
	for(i = 0; i < buffer_size; i++) {
		buffer[i]=-1;
	}
	
	printf("Main thread beginning\n");
	
	// create producer and consumer
	for(i = 0; i < producer_num; i++) {
		pthread_create(&producer_array[i], NULL, producer, (void *)&i);
		printf("Creating producer thread with id %lu\n", producer_array[i]);
	}
	for(i = 0; i < consumer_num; i++) {
		pthread_create(&consumer_array[i], NULL, consumer, (void *)&i);
		printf("Creating consumer thread with id %lu\n", consumer_array[i]);
	}

	printf("Main thread sleeping for %d seconds\n\n", run_time);	
	sleep(run_time);
	
	//join thread
	/*
	for(i = 0; i < producer_num; i++) {
		pthread_join(producer_array[i], NULL);
	}
	for(i = 0; i < consumer_num; i++) {
		pthread_join(consumer_array[i], NULL);
	}
	*/
	pthread_mutex_destroy(&mutex);
	sem_destroy(&empty);
	sem_destroy(&full);
	
	printf("Main thread exiting\n");
	exit(0);
}




