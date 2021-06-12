#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

// FUNCTIONS //

void closeFiles(int sF, int dF, int errorID) //close open files
{
    //errors switch, by default (0) nothing is displayed
    switch (errorID)
    {
    case 1:
        printf("Eror occured in readWrite func during reading.\n");
        break;
    case 2:
        printf("Eror occured in readWrite func during writing.\n");
        break;
    case 3:
        printf("Eror occured in memoryMap func during offset reposition.\n");
        break;
    case 4:
        printf("Eror occured in memoryMap func during mmap sourceFile.\n");
        break;
    case 5:
        printf("Eror occured in memoryMap func during ftruncate.\n");
        break;
    case 6:
        printf("Eror occured in memoryMap func during mmap destinationFile.\n");
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

void readWrite(int sF, int dF) //use read write functions
{
    //copies chars will be placed in buffer
    char buf[1024];
    int count, errorOccured = 0;

    while (1)
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
        {
            closeFiles(sF, dF, 0);
            break;
        }
    }

    return;
}

void memoryMap(int sF, int dF) //map files to memory region
{
    off_t fileOffset;

    //reposition read/write file offset
    if ((fileOffset = lseek(sF, 0, SEEK_END)) == -1)
    {
        closeFiles(sF, dF, 3);
        return;
    }

    //map files (sourceFile)
    char *sfData = mmap(NULL, fileOffset, PROT_READ, MAP_PRIVATE, sF, 0);

    //failure of mmap
    if (sfData == MAP_FAILED)
    {
        closeFiles(sF, dF, 4);
        return;
    }

    //truncate file to specified length
    if (ftruncate(dF, fileOffset) == -1)
    {
        closeFiles(sF, dF, 5);
        return;
    }

    //map files (destinationFile)
    char *dfData = mmap(NULL, fileOffset, PROT_WRITE | PROT_READ, MAP_SHARED, dF, 0);

    //failure of mmap
    if (dfData == MAP_FAILED)
    {
        closeFiles(sF, dF, 6);
        return;
    }

    //copy from source to destination
    memcpy(dfData, sfData, fileOffset);

    //unmap mapped files
    munmap(sfData, fileOffset);
    munmap(dfData, fileOffset);

    closeFiles(sF, dF, 0); //close files without error

    return;
}

// MAIN //

int main(int argc, char *argv[])
{
    int option, useReadWrite = 1; //by default program uses read write

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

    //two arguments are needed: file to copy from and paste to
    if (argc - optind < 2)
    {
        printf("Incorrect number of given arguments. Use -h for help.\n");

        return 1;
    }

    //open file to copy from
    int sourceFile;

    if ((sourceFile = open(argv[optind], O_RDONLY)) == -1)
    {
        printf("Error occured while opening source file. Use -h for help.\n");

        return 1;
    }

    int destinationFile;

    //open file to paste to
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