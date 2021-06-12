#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

// FUNCTIONS //
void readWrite(int sF, int dF) //use read write functions
{
    //copies chars will be placed in buffer
    char buf[1024];
    int count, errorOccured = 0;

    while (true)
    {
        errorOccured = 0;

        if ((count = read(sF, buf, 1024)) == -1)
        {
            closeFiles(sF, dF, 1); //close files with error
            errorOccured = 1;
            break;
        }

        if (write(dF, buf, count) == -1)
        {
            closeFiles(sF, dF, 2); //close files with error
            errorOccured = 1;
            break;
        }

        if (errorOccured == 0)
            break;
    }

    closeFiles(sF, dF, 0); //close files without error
}

void memoryMap(int sF, int dF) //map files to memory region
{
    closeFiles(sF, dF, 0); //close files without error
}

void closeFiles(int sF, int dF, int errorID) //close open files
{
    switch (errorID)
    {
    case 1:
        printf("Eror occured in readWrite func during reading.\n");
        break;
    case 2:
        printf("Eror occured in readWrite func during writing.\n");
        break;
    default:
        break;
    }

    //close files
    if (close(sF) == -1)
    {
        printf("Error occured while closing source file. Use -h for help.\n");
    }

    if (close(dF) == -1)
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

    if (argc - optind < 2)
    {
        printf("Incorrect number of given arguments. Use -h for help.\n");

        return 1;
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