#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>



//with or without signals
#define WITH_SIGNALS
//nr of child processes
#define NUM_CHILD 6

//nr of child processes
int nrOfChildren = 0;

//array of children processes pids
pid_t childrenArray[NUM_CHILD];



//kill children func
void killChildren(int id) {

    // loop to kill all children
    for(int i = id; i > 0; i--) {
        //send SIGTERM to children
        kill(childrenArray[i], SIGTERM);
    }
}

//create new child func
void createChild(int id) {

    pid_t pid = fork(); //new child process

    if(pid <= -1) { //negative = child process creation failed

        //print msg about creation failure
        printf("parent[%d]: child process creation failure, killing children and exiting\n", getpid());

        //killing children
        #ifdef WITH_SIGNALS
            killChildren(id);
        #endif

        //exit with code 1
        exit(1);

    } else if(pid == 0) { //0 = child created & returned to child process

        //print msg with parent pid
        printf("child[%d]: pid of my parent is %d\n", getpid(), getppid());

        //sleep for 10s
        sleep(10);

        //print msg execution competition
        printf("child[%d]: execution completed\n", getpid());

        exit(0);

    } else { //positive = child created & returned to parent

        //nr of children++
        nrOfChildren++;
        //remember child id
        childrenArray[id] = pid;
    }
}

int main() {
    
    //print msg that parent created
    printf("parent[%d]: process created\n", getpid());

    //create children
    for(int i = 0; i < NUM_CHILD; i++) {
        
        createChild(i);
        //sleep for 1s
        sleep(1);
    }

    return 0;
}