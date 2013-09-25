/*
 * multithreading_test.cpp
 *
 *  Created on: Sep 23, 2013
 *      Author: vinhtt
 */

#include <iostream>
#include <pthread.h>

#ifdef MULTITHREADING

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int counter = 0;
int length = 10000000;

void *threading(void* args);
void print(int id, int i);

int main(int argc, char** args)
{
	int NUM_THREADS = 4;
	pthread_t threads[NUM_THREADS];
	int ids[NUM_THREADS];

	for (int i = 0; i < NUM_THREADS; i++)
	{
		std::cout << "starting thread " << i << std::endl;
		std::flush(std::cout);
		ids[i] = i;
		pthread_create(&threads[i], NULL, threading, (void*)(ids + i));
	}

	pthread_exit(NULL);

	return 0;
}

void *threading(void* args)
{
	int id = *((int*)args);
	std::cout << "thread " << id << " started" << std::endl;
	std::flush(std::cout);

	while (counter < length)
	{
		pthread_mutex_lock(&mutex);
		counter++;
		pthread_mutex_unlock(&mutex);
		print(id, counter);
	}

	pthread_exit(NULL);
}

void print(int id, int i)
{
	std::cout << id << " : " << i << std::endl;
	std::flush(std::cout);
}

#endif
