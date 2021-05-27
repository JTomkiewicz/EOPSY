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
    sleep(0);
}

void eating(int id) //print msg that philosopher is eating
{
    printf("philosopher[%d]: eating\n", i);
    sleep(1);
}

// MAIN //
int main()
{

    for (int i = 0; i < NUM_PHIL; i++)
    {

        pid_t pid = fork(); //new philosopher

        if (pid <= -1) //negative = child process creation failed
        {

            //print msg about creation failure
            printf("main[%d]: philosopher %d process creation failure, killing philosophers and exiting\n", getpid(), i);

            //kill philosophers
            killPhilosophers(i);

            //exit with code 1
            exit(1);
        }
        else if (pid == 0) //0 = child created & returned to child process
        {

            while (1) //go throught the loop of philosopher destiny
            {
                thinking(i);
                grab_forks(i);
                eating(i);
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

    return 0;
}