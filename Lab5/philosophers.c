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
#define LEFT (philoId + 4) % NUM_PHIL
#define RIGHT (philoId + 1) % NUM_PHIL

//array of philosophers pids
pid_t philosophersArray[NUM_PHIL];

// FUNCTIONS //
void grab_forks(int left_fork_id)
{
}

void put_away_forks(int left_fork_id)
{
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
    printf("philosopher[%d]: thinking\n", i);
    sleep(1);
}

void eating(int id) //print msg that philosopher is eating
{
    printf("philosopher[%d]: eating\n", i);
    sleep(2);
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
        //value to one
        if (semctl(semaphoreKey, i, SETVAL, 1) == -1)
        {
            printf("main: Error during semctl");
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
            printf("main: philosopher %d process creation failure, killing philosophers and exiting\n", getpid(), i);

            //kill philosophers
            killPhilosophers(i);

            //exit with code 1
            exit(1);
        }
        else if (pid == 0) //0 = child created & returned to child process
        {
            printf("philosopher[%d]: I'm alive", i);

            sleep(2); //sleep for 2 sec after init

            while (1) //go throught the loop of philosopher destiny
            {
                thinking(i);

                printf("philosopher[%d]: Trying to get forks\n");
                grab_forks(i);

                eating(i);

                printf("philosopher[%d]: Trying to put away forks\n");
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