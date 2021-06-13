// EOPSY Lab6 Program that copies one file to another //
// By Jakub Tomkiewicz 300183 //

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

// FUNCTIONS //

void closeFiles(int sF, int dF, int errorID) //close open files & error display
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

    //close source file and destination file
    if (close(sF) == -1)
    {
        printf("Error occured while closing source file. Use -h for help.\n");
    }

    if (close(dF) == -1)
    {
        printf("Error occured while closing source file. Use -h for help.\n");
    }
}

void readWrite(int sF, int dF) //use read&write functions
{
    //copies chars will be placed in buffer
    char buf[1024];

    //number of read bytes
    int count = 0, errorOccured = 0, count2 = 0;

    while (1)
    {
        errorOccured = 0;

        //write up to 1024 bytes from source file to buffer
        if ((count = read(sF, buf, 1024)) == -1)
        {
            closeFiles(sF, dF, 1); //close files with error
            errorOccured = 1;
            break;
        }

        //write count bytes from buffer to destination file
        if ((count2 = write(dF, buf, count)) == -1)
        {
            closeFiles(sF, dF, 2); //close files with error
            errorOccured = 1;
            break;
        }

        //everything is fine, no errors
        if (errorOccured == 0)
        {
            printf("Number of coppied bytes: %d\n", count2);
            closeFiles(sF, dF, 0);
            break;
        }
    }

    return;
}

void memoryMap(int sF, int dF) //map files to memory region
{
    off_t fileOffset;

    //reposition file offset from source file to the offset argument, file offset is set to the size of file + offset
    if ((fileOffset = lseek(sF, 0, SEEK_END)) == -1)
    {
        closeFiles(sF, dF, 3);
        return;
    }

    //map with length offset starting address NULL, pages may be read (memory protection), private copy-on-write mapping, source file
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

    //map with length offset starting address NULL, pages may be read & written (memory protection), share this mapping, destination file
    char *dfData = mmap(NULL, fileOffset, PROT_WRITE | PROT_READ, MAP_SHARED, dF, 0);

    //failure of mmap
    if (dfData == MAP_FAILED)
    {
        closeFiles(sF, dF, 6);
        return;
    }

    //copy from source to destination number of offset bytes
    memcpy(dfData, sfData, fileOffset);

    //unmap mapped source file and destination file with mapping length of offset
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
        if (option == 'h') //when -h display help
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
            printf("Wrong parameters. Use -h for help.\n"); //something different then h or m was inserted

            return 1;
        }
    }

    //two arguments are needed: file to copy from and paste to
    if (argc - optind < 2 || argc - optind > 2) //optind is the index of next element
    {
        printf("Incorrect number of given arguments. Use -h for help.\n");

        return 1;
    }

    //open file to copy from
    int sourceFile;

    if ((sourceFile = open(argv[optind], O_RDONLY)) == -1) //open source file in read only
    {
        printf("Error occured while opening source file. Use -h for help.\n");

        return 1;
    }

    //open file to paste to
    int destinationFile;

    //open destination file in read&write, if does not exist create it (with read, write permissions)
    if ((destinationFile = open(argv[1 + optind], O_CREAT | O_RDWR, 00200 | 00400)) == -1)
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