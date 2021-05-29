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

pthread_mutex_t mutex; //mutex locker

pthread_mutex_t mutexForFork[NUM_PHIL];

//int that can be 1 (if fork can be locked) and 0 (if not)
int canBeLocked[NUM_PHIL] = {0};

//this is the philosopher structure
struct philoStruct
{
    int id;
    int countMeals;
};

// FUNCTIONS //
void grab_forks(int philo_id)
{
    //lock mutex
    pthread_mutex_lock(&mutex);

    //if success of locking left
    if (pthread_mutex_trylock(&mutexForFork[philo_id]) == 0)
    {
        //if success of locking right
        if (pthread_mutex_trylock(&mutexForFork[(philo_id + 1) % NUM_PHIL]) == 0)
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

    //unlock mutex
    pthread_mutex_unlock(&mutex);
}

void put_away_forks(int philo_id)
{
    //left
    pthread_mutex_unlock(&mutexForFork[philo_id]);

    //right
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
    ;
}

void *philosopher(void *philoFromMain)
{
    struct philoStruct *philo = (struct philoStruct *)philoFromMain;

    printf("philosopher[%d]: I'm alive\n", philo->id);

    sleep(beginTime);

    philo->countMeals = 0;

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
    //init mutex lock
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < NUM_PHIL; i++)
    {
        pthread_mutex_init(&mutexForFork[i], NULL); //init mutex lock for mutexes in array
    }

    //create philosophers
    for (int i = 0; i < NUM_PHIL; i++)
    {
        struct philoStruct *philo;

        philo = malloc(sizeof(struct philoStruct));

        philo->id = i;

        if (pthread_create(&philoThreads[i], NULL, philosopher, philo))
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
        if (pthread_mutex_destroy(&mutexForFork[i]) == -1)
        {
            printf("main: Error occured while mutex destruction\n");
            exit(1);
        }
    }

    return 0;
}