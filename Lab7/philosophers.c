// DINING PHILOSOPHERS PROBLEM IN C WITH THREADS AND MUTAXES //
// BY JAKUB TOMKIEWICZ //

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
//threads & mutaxes
#include <pthread.h>

#define NUM_PHIL 5

//time manipulation
#define BEGIN_TIME 2
#define THINK_TIME 2
#define EAT_TIME 2

// FUNCTIONS //
void grab_forks(int philo_id)
{
}

void put_away_forks(int philo_id)
{
}

void *philosopher(void *number)
{
}

// MAIN //
int main()
{
    pthread_t philoThreads[NUM_PHIL]; //thread identifiers

    pthread_mutex_t mutex; //mutex locker

    pthread_mutex_t mutexForFork[NUM_PHIL];

    //init mutex lock
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < NUM_PHIL; i++)
    {
        pthread_mutex_init(&mutexForFork[i], NULL); //init mutex lock for mutexes in array
    }

    //create philosophers
    for (int i = 0; i < NUM_PHIL; i++)
    {
        if (pthread_create(&philoThreads[i], NULL, philosopher, (void *)i))
        {
            printf("main: Error occured while creating threads\n");
            exit(1);
        }
    }

    //join threads
    for (int i = 0; i < NUM_PHIL; i++)
    {
        if (pthread_join(philoThreads[i], NULL))
        {
            printf("main: Error occured while joining threads\n");
            exit(1);
        }
    }

    //destroy mutexes
    for (int i = 0; i < NUM_PHIL; i++)
    {
        if (pthread_mutex_destroy(&mutexForFork[i]))
        {
            printf("main: Error occured while mutex destruction\n");
            exit(1);
        }
    }

    return 0;
}