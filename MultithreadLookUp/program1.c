#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* create a struct that marks line number of 
starting searching and end searching */
typedef struct Arr {
	int start;
	int end;
}Arr;

// the result array of all line numbers that contains the number to be searched.
int* array; 
static int index = 0;
// the array of numbers after reading the given file.
int* integer;
int number = 0;
int num_search;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void read(char* file) {
  FILE *f;
  f = fopen(file, "r");
  char buffer[1000];
  while (fgets(buffer, 999, f) != NULL)
  {
    integer[number++] = atoi(buffer);
  }
  fclose(f);
}

/*the process of searching number for each thread, 
using the mutex lock to void the conflict of different 
threads dealing with the same data.*/
void search(void* pointer) {
  Arr* read_arr = (Arr*) pointer;
  int start = read_arr->start;
  int end = read_arr->end;
  int* position = malloc(sizeof(int) * (end - start + 1));
  int temp = 0;
  for(int i = start; i <= end; i++) {
     if (integer[i] == num_search) {
      position[temp++] = i + 1;
     }
  }
 
  if(pthread_mutex_lock(&mutex) != 0) {
    perror("pthread_mutex_lock");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < temp; i++) {
    array[index++] = position[i];
  }
 
  if(pthread_mutex_unlock(&mutex) != 0) {
    perror("pthread_mutex_unlock");
    exit(EXIT_FAILURE);
  }
  free(position);
}

/*print out the given array*/
void print(int* arr, int length) {
	for(int i = 0; i < length; i++) {
		printf("%d\n",arr[i]);
	}
}

/* the main method that includes creating the multible threads by given numbers,
let all the threads starting the process of searching numbers, and finish the 
main methods until all the threads end.*/
int main(int argc, char* argv[]) {
	if(argc != 5) {
		return 1;
	}
	char* file_name = argv[1];
  	num_search = atoi(argv[2]);
  	int threads_num = atoi(argv[3]);
  	int size = atoi(argv[4]);

  	integer = (int*) malloc(size * 1024 * 1024);

  	array = (int*) malloc(size * 1024 * 1024);

  	read(file_name);

    int per_thread = number / threads_num;

    int rest = number % threads_num;
    
	pthread_t* thread = malloc(sizeof(pthread_t)*threads_num);

	int start = 0;
	int end = start + per_thread - 1;
	Arr* arr = malloc(sizeof(Arr)*threads_num);;

	for (int i = 0; i < threads_num; i++) {
    	while(rest > 0) {
			end++;
			rest--;
		}
		(arr + i)->start = start;
		(arr + i)->end = end;
		int ret = pthread_create(&thread[i], NULL, (void *)&search, (void *) (arr + i));
		if(ret != 0) {
		    printf ("Create pthread error!\n");
		    exit (1);
		}
		start = end + 1;
		end = start + per_thread - 1;
	}
	for (int i = 0; i < threads_num; i++) {
		pthread_join(thread[i], NULL);
	}
	print(array, index);
	free(thread);
	free(integer);
	free(arr);
	free(array);
	return 0;
}

