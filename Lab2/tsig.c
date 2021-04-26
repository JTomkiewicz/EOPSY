#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>



//nr of child processes
#define NUM_CHILD 6

//nr of child processes
int nrOfChildren = 0;

//array of children processes pids
pid_t childrenArray[NUM_CHILD];

//global var to see if keyboard interrupt occurance appeared
int keyboardInterruptOccurance = 0;



//sigint handler func
void sigintHandler() {
    //interrupt appeared
    keyboardInterruptOccurance = 1;
    
    printf("parent[%d]: received keyboard interrupt\n", getpid());
}

//sigterm handler func
void sigtermHandler() {
    printf("child[%d]: terminating\n", getpid());

    exit(0);
}

//kill children func
void killChildren(int id) {
    //loop to kill all children
    for(int i = 0; i <= id; i++) {
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

        //kill children
        killChildren(id);

        //exit with code 1
        exit(1);

    } else if(pid == 0) { //0 = child created & returned to child process

        //print msg with parent pid
        printf("child[%d]: pid of my parent is %d\n", getpid(), getppid());

        #ifdef WITH_SIGNALS
        //ignore all other signals
        for(int i = 0; i < NSIG; i++) {
            signal(i, SIG_IGN);
        }

        //my own handler of the SIGTERM signal
        signal(SIGTERM, sigtermHandler);
        #endif

        //sleep for 10s
        sleep(10);

        //print msg execution completion
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

#ifdef WITH_SIGNALS
//restore signals to default func
void restoreAllSignalsDefault() {
    //loop for all signals
    for(int i = 0; i <  NSIG; i++) {
        signal(i, SIG_DFL);
    }

    printf("parent[%d]: signals restored to default\n", getpid());
}
#endif

int main() {
    
    //print msg that parent created
    printf("parent[%d]: process created\n", getpid());

    #ifdef WITH_SIGNALS
        //own signal interrupt handler
        signal(SIGINT, sigintHandler);

        //restore sigchld to default handler
        signal(SIGCHLD, SIG_DFL); 
    #endif

    //create children
    for(int i = 0; i < NUM_CHILD; i++) {
        
        createChild(i);
        //1s delay between fork calls
        sleep(1);

        #ifdef WITH_SIGNALS
        //keyboard interrupt appeared
        if(keyboardInterruptOccurance == 1) {
            //signal children with SIGTERM
            killChildren(i);

            //no more children
            break;
        }
        #endif
    }

    //if keyboard interrupt occured msg that
    if(keyboardInterruptOccurance == 1) {
        printf("parent[%d]: child creation has been interrupted\n", getpid());
    } else {
        printf("parent[%d]: all child processes created\n", getpid());
    }

    //integer to count child terminations
    int countChildTerminations = 0;

    //infinite loop untill processes are running
    while(1) {
        if(wait(NULL) == -1) { //-1 if no process has any child processes

            printf("parent[%d]: there are no more child processes running\n", getpid());
            //break the loop
            break;
        }
        //nr of killed children + 1
        countChildTerminations++;
    }
    
    printf("parent[%d]: %d child processes exit codes received\n", getpid(), countChildTerminations);

    #ifdef WITH_SIGNALS
    //restore all signals to default
    restoreAllSignalsDefault();
    #endif

    return 0;
}