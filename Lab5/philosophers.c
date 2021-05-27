// DINING PHILOSOPHERS PROBLEM IN C WITH SEMAPHORES //
// BY JAKUB TOMKIEWICZ //

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
//semget & semop
#include <sys/ipc.h>
#include <sys/sem.h>

#define NUM_PHIL 5

//time manipulation
#define BEGIN_TIME 2
#define THINK_TIME 2
#define EAT_TIME 2

//array of philosophers pids
pid_t philosophersArray[NUM_PHIL];

//semaphores key by default set to error state
int semaphoreKey = -1;

// FUNCTIONS //
void grab_forks(int left_fork_id)
{
    struct sembuf operation[2];

    //two forks for philosopher
    operation[0].sem_num = left_fork_id;
    operation[1].sem_num = (left_fork_id + 1) % NUM_PHIL;

    operation[0].sem_op = -1;
    operation[1].sem_op = -1;

    //no additional flags
    operation[0].sem_flg = 0;
    operation[1].sem_flg = 0;

    if (semop(semaphoreKey, operation, 2) == -1)
    {
        printf("Error: semop error in grab_forks\n");
    }
}

void put_away_forks(int left_fork_id)
{
    struct sembuf operation[2];

    //two forks for philosopher
    operation[0].sem_num = left_fork_id;
    operation[1].sem_num = (left_fork_id + 1) % NUM_PHIL;

    operation[0].sem_op = 1;
    operation[1].sem_op = 1;

    //no additional flags
    operation[0].sem_flg = 0;
    operation[1].sem_flg = 0;

    if (semop(semaphoreKey, operation, 2) == -1)
    {
        printf("Error: semop error in put_away_forks\n");
    }
}

void killPhilosophers(int id) //kill existing philosophers
{
    //loop to kill all philosophers
    for (int i = 0; i <= id; i++)
    {
        //send SIGTERM to philosophers
        kill(philosophersArray[i], SIGTERM);
    }
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

// MAIN //
int main()
{
    //create semaphore key
    semaphoreKey = semget(IPC_PRIVATE, NUM_PHIL, 0644 | IPC_CREAT);

    //check if error occured
    if (semaphoreKey == -1)
    {
        printf("main: Error duting semger\n");
        exit(1);
    }

    //set up semaphores
    for (int i = 0; i < NUM_PHIL; i++)
    {
        //value set to one
        if (semctl(semaphoreKey, i, SETVAL, 1) == -1)
        {
            printf("main: Error during semctl\n");
            exit(1);
        }
    }

    // create philosophers
    for (int i = 0; i < NUM_PHIL; i++)
    {

        pid_t pid = fork(); //new philosopher

        if (pid <= -1) //negative = child process creation failed
        {

            //print msg about creation failure
            printf("main: philosopher %d process creation failure, killing philosophers and exiting\n", i);

            //kill philosophers
            killPhilosophers(i);

            //exit with code 1
            exit(1);
        }
        else if (pid == 0) //0 = child created & returned to child process
        {
            printf("philosopher[%d]: I'm alive\n", i);

            sleep(BEGIN_TIME); //sleep for 2 sec after init

            int countMeals = 0;

            while (1) //go throught the loop of philosopher destiny
            {
                thinking(i);

                printf("philosopher[%d]: Trying to get forks\n", i);
                grab_forks(i);

                eating(i);
                countMeals++;

                printf("philosopher[%d]: Ate %d meals. Trying to put away forks\n", i, countMeals);
                put_away_forks(i);
            }

            //exit code 0
            exit(0);
        }
        else //positive = child created & returned to parent
        {
            //remember philosopher id
            philosophersArray[i] = pid;
        }
    }

    //infinite loop untill processes are running
    while (1)
    {
        if (wait(NULL) == -1) //-1 if no process has any child processes
        {
            printf("main: there are no more philosophers alive\n");
            //break the loop
            break;
        }
    }

    return 0;
}