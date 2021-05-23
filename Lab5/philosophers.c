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

//print msg that philosopher is thinking
void msgThink() {

}

//print msg that philosopher is eating
void msgEat() {

}
 
int main() {

    for(int i = 0; i < NUM_PHIL; i++) {

        pid_t pid = fork(); //new philosopher

        if(pid <= -1) { //negative = child process creation failed

            //print msg about creation failure
            printf("main[%d]: philosopher %d process creation failure, killing philosophers and exiting\n", getpid(), i);

            //kill philosophers
            killPhilosophers(i);

            //exit with code 1
            exit(1);

        } else if(pid == 0) { //0 = child created & returned to child process

            //exit code 0
            exit(0);

        } else { //positive = child created & returned to parent
            
            //remember philosopher id
            philosophersArray[i] = pid;
            
        }
    }

    return 0;
}