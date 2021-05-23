#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>



//number of philosophers
#define NUM_PHIL 5

//array of philosophers pids
pid_t philosophersArray[NUM_PHIL];

void grab_forks(int left_fork_id) {
}

void put_away_forks(int left_fork_id) {
}

//kill philosophers func
void killPhilosophers(int id) {
    //loop to kill all philosophers
    for(int i = 0; i <= id; i++) {
        //send SIGTERM to philosophers
        kill(philosophersArray[i], SIGTERM);
    }
}

int main() {

    for(int i = 0; i < NUM_PHIL; i++) {

        pid_t pid = fork(); //new philosopher

        if(pid <= -1) { //negative = child process creation failed

            //print msg about creation failure
            printf("main[%d]: philosopher process creation failure, killing philosophers and exiting\n", getpid());

            //kill philosophers
            killPhilosophers(i);

            //exit with code 1
            exit(1);

        } else if(pid == 0) { //0 = child created & returned to child process

        } else { //positive = child created & returned to parent
            //remember philosopher id
            philosophersArray[id] = pid;
        }
    }

    return 0;
}