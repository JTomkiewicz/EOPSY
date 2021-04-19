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

//global var to see if keyboard interrupt occurance appeared
bool keyboardInterruptOccurance = false;



//sigint handler func
void sigintHandler() {
    //interrupt appeared
    keyboardInterruptOccurance = false;
    
    printf("parent[%d]: received keyboard interrupt\n",getpid());
}

//sigterm handler func
void sigtermHandler() {
    printf("child[%d]: terminating\n", getpid());
}

//kill children func
void killChildren(int id) {

    // loop to kill all children
    for(int i = 0; i < id; i++) {
        //send SIGTERM to child
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

        //my own handler of the SIGTERM signal & ignore keyboard interuptions
        #ifdef WITH_SIGNALS
            signal(SIGTERM, sigtermHandler);

            signal(SIGINT, SIG_IGN);
        #endif

        //sleep for 10s
        sleep(10);

        //print msg execution competition
        printf("child[%d]: execution completed\n", getpid());

        //exit code 0
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

    #ifdef WITH_SIGNALS
        //own signal interrupt handler
        signal(SIGINT, sigintHandler);

        signal(SIGCHLD, SIG_DFL); //sigchld reset to default
    #endif

    //create children
    for(int i = 0; i < NUM_CHILD; i++) {
        
        createChild(i);
        //sleep for 1s
        sleep(1);

        #ifdef WITH_SIGNALS
            //keyboard interrupt is set
            if(keyboardInterruptOccurance) {
                //signal children with SIGTERM
                killChildren(i);

                //continue to wait's loop
                break;
            }
        #endif
    }

    //if keyboard interrupt msg that
    if(keyboardInterruptOccurance) {
        printf("parent[%d]: interrupt of child creation\n", getpid());
    } else {
        printf("parent[%d]: all child processes created\n", getpid());
    }

    int childTerminations = 0;

    //infinite loop
    while(1) {
        if(wait(NULL) == -1) { //-1 if no process has any child processes

            printf("parent[%d]: there are no more child processes\n", getpid());
            //break the loop
            break;
        }
        //nr of killed children + 1
        childTerminations++;
    }
    
    printf("parent[%d]: %d terminations performed\n", getpid(), childTerminations);

    //restore all signals to default
    #ifdef WITH_SIGNALS

    #endif

    return 0;
}