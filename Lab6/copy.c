#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

// FUNCTIONS //
void readWrite(int sourceFile, int destinationFile) //use read write functions
{
}

void memoryMap(int sourceFile, int destinationFile) //map files to memory region
{
}

void closeFiles(int sourceFile, int destinationFile, int errorID) //close open files
{
    switch (errorID)
    {
    case 1:
        break;
    default:
        break;
    }

    //close files
    if (close(sourceFile) == -1)
    {
        printf("Error occured while closing source file. Use -h for help.\n");
    }

    if (close(destinationFile) == -1)
    {
        printf("Error occured while closing source file. Use -h for help.\n");
    }
}

// MAIN //
int main(int argc, char *argv[])
{
    int option, useReadWrite = 1;

    //process command line options and arguments
    while ((option = getopt(argc, argv, ":hm")) != -1) //-1 if there are no more options
    {
        if (option == 'h')
        {
            printf("Program copies one file to another. Use syntax:\ncopy [-m] <file_name> <new_file_name> \ncopy [-h]\n");

            return 0;
        }
        else if (option == 'm')
        {
            useReadWrite = 0; //when -m use memory map
        }
        else
        {
            printf("Wrong parameters. Use -h for help.\n");

            return 1;
        }
    }

    //open file to COPY FROM
    int sourceFile;

    if ((sourceFile = open(argv[optind], O_RDONLY)) == -1)
    {
        printf("Error occured while opening source file. Use -h for help.\n");

        return 1;
    }

    //open file to PASTE TO
    int destinationFile;

    if ((destinationFile = open(argv[1 + optind], O_CREAT | O_RDWR, S_IRUSR | S_IWUSR)) == -1)
    {
        printf("Error occured while opening destiantion file. Use -h for help.\n");

        return 1;
    }

    //now the copy operation will be performed
    if (useReadWrite) //readWrite function will be used
    {
        readWrite(sourceFile, destinationFile);
    }
    else //memoryMap function will be used
    {
        memoryMap(sourceFile, destinationFile);
    }

    return 0;
}