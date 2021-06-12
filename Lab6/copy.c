#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

// FUNCTIONS //
void readWrite() //use read write functions
{
}

void memoryMap() //map files to memory region
{
}

// MAIN //
int main(int argc, char *argv[])
{
    int option;

    //process command line options and arguments
    while ((option = getopt(argc, argv, ":hm:")) != -1) //-1 if there are no more options
    {
        if (option == 'h')
        {
            printf("Program copies one file to another. Use syntax:\ncopy [-m] <file_name> <new_file_name> \ncopy [-h]\n");

            return 0;
        }
        else if (option == 'm')
        {
        }
        else
        {
            printf("Wrong parameters. Use -h for help.\n");

            return 1;
        }
    }

    return 0;
}