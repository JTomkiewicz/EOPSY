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

// MAIN //
int main()
{
    return 0;
}