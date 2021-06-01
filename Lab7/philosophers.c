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

//time manipulation (values in sec)
int beginTime = 2;
int thinkTime = 2;
int eatTime = 2;

pthread_t philoThreads[NUM_PHIL]; //thread identifiers

pthread_mutex_t mutexForFork[NUM_PHIL]; //one fork = one mutex

//int that can be 1 (if fork can be locked) and 0 (if not)
int canBeLocked[NUM_PHIL] = {0};

//this is the philosopher structure
struct philoStruct
{
    //that stores identifier of philo
    int id;
    //and nr of meals that he ate
    int countMeals;
};

// FUNCTIONS //
void grab_forks(int philo_id)
{
    //if success of locking left
    if (pthread_mutex_lock(&mutexForFork[philo_id]) == 0)
    {
        //if success of locking right
        if (pthread_mutex_lock(&mutexForFork[(philo_id + 1) % NUM_PHIL]) == 0)
        {
            //YES! philosopher can eat
            canBeLocked[philo_id] = 1;
        }
        else
        {
            //right is busy, release left
            pthread_mutex_unlock(&mutexForFork[philo_id]);

            //NO :( philosopher must think
            canBeLocked[philo_id] = 0;
        }
    }
    else
    {
        //NO :( philosopher must think
        canBeLocked[philo_id] = 0;
    }
}

void put_away_forks(int philo_id)
{
    //release left fork
    pthread_mutex_unlock(&mutexForFork[philo_id]);

    //release right fork
    pthread_mutex_unlock(&mutexForFork[(philo_id + 1) % NUM_PHIL]);
}

void thinking(int id) //print msg that philosopher is thinking
{
    printf("philosopher[%d]: thinking\n", id);
    sleep(thinkTime);
}

void eating(int id) //print msg that philosopher is eating
{
    printf("philosopher[%d]: eating\n", id);
    sleep(eatTime);
}

void *philosopher(void *philoFromMain)
{
    //create philo struct
    struct philoStruct *philo = (struct philoStruct *)philoFromMain;

    printf("philosopher[%d]: I'm alive\n", philo->id);

    //count meals in 0
    philo->countMeals = 0;

    sleep(beginTime);

    //infinite loop (this is the philosopher life)
    while (1)
    {
        thinking(philo->id);

        printf("philosopher[%d]: Trying to get forks\n", philo->id);
        grab_forks(philo->id);

        if (canBeLocked[philo->id] == 1)
        {
            eating(philo->id);
            philo->countMeals++;

            printf("philosopher[%d]: Ate %d meals. Trying to put away forks\n", philo->id, philo->countMeals);
            put_away_forks(philo->id);
        }
    }

    pthread_cancel(philoThreads[philo->id]);
}

// MAIN //
int main()
{
    for (int i = 0; i < NUM_PHIL; i++)
    {
        //initialize mutex object with DEFAULT values
        if (pthread_mutex_init(&mutexForFork[i], NULL) != 0)
        {
            printf("main: Error while mutexForFork initialisation\n");
            exit(1);
        }
    }

    //create philosophers
    for (int i = 0; i < NUM_PHIL; i++)
    {
        //create new structure in C
        struct philoStruct *philo;
        philo = malloc(sizeof(struct philoStruct));

        //loop id is the philo id
        philo->id = i;

        //create a new thread
        if (pthread_create(&philoThreads[i], NULL, philosopher, philo) != 0)
        {
            printf("main: Error occured while creating threads\n");
            exit(1);
        }
    }

    //waits for the thread to terminate
    for (int i = 0; i < NUM_PHIL; i++)
    {
        if (pthread_join(philoThreads[i], NULL) != 0)
        {
            printf("main: Error occured while joining threads\n");
            exit(1);
        }
    }

    //destroy mutex object and release sources
    for (int i = 0; i < NUM_PHIL; i++)
    {
        if (pthread_mutex_destroy(&mutexForFork[i]) != 0)
        {
            printf("main: Error occured while mutex destruction\n");
            exit(1);
        }
    }

    return 0;
}