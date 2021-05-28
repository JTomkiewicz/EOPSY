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

pthread_t philoThreads[NUM_PHIL]; //thread identifiers

pthread_mutex_t mutex; //mutex locker

pthread_mutex_t mutexForFork[NUM_PHIL];

struct philoStruct
{
    int id;
};

// FUNCTIONS //
void grab_forks(int philo_id)
{
    pthread_mutex_lock(&mutex);

    //if success of locking left
    if (pthread_mutex_trylock(&mutexForFork[philo_id]) == 0)
    {
        //if success of locking right
        if (pthread_mutex_trylock(&mutexForFork[(philo_id + 1) % NUM_PHIL]) == 0)
        {
        }
        else
        {
            pthread_mutex_unlock(&mutexForFork[philo_id]);
        }
    }
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
    sleep(THINK_TIME);
}

void eating(int id) //print msg that philosopher is eating
{
    printf("philosopher[%d]: eating\n", id);
    sleep(EAT_TIME);
}

void *philosopher(void *philoFromMain)
{
    struct philoStruct *philo = (struct philoStruct *)philoFromMain;

    int id = philo->id;

    printf("philosopher[%d]: I'm alive\n", id);

    sleep(BEGIN_TIME);

    int countMeals = 0;

    while (1)
    {
        thinking(id);

        printf("philosopher[%d]: Trying to get forks\n", id);
        grab_forks(id);

        eating(id);
        countMeals++;

        printf("philosopher[%d]: Ate %d meals. Trying to put away forks\n", id, countMeals);
        put_away_forks(id);
    }

    pthread_cancel(philoThreads[id]);
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