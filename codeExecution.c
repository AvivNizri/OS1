/*
  OsCourse task1 part2 -> C program to iterate over a directory recursively, and execute compilation and more
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>


void exploreFilesRecursively(char* basePath, char* input, char* output, dirent* filesComper);

int main(int argc, char* argv[])
{
    if(argc != 3){
        printf("The system got unexcpected amount of items..\n exiting...");
        return 0;
    }

    int configFIle = open(strcat(argv[2],argv[1]), O_RDONLY);

    if(configFIle == -1){
        printf("There was an error opening the files\n exiting...");
        return 0;
    }
    size_t len = 0;

    char* line1 = NULL;
    // Get the first line of the config file to get the path of entire users
    getline(&line1, &len, configFIle);

    // Get the second line which is it the system input for every user later on
    char* line2 = NULL;
    getline(&line2, &len, configFIle);

    // Get the third line which is it the system expected output for every user later on
    char* line3 = NULL;
    getline(&line3, &len, configFIle);

    // get the filesComper program for later usage
    // open current dir
    DIR* curDir = opendir(".");
    struct dirent* fc;
    // loop until finds the expected program
    while ((fc = readdir(curDir)) != NULL)
    {
        // Avoid getting into hidden and system dir
        if (strcmp(fc->d_name, "filesComper") == 0){
            break;
        }
    }

    int resultCSV = open("result.csv", O_WRONLY | O_CREAT | O_TRUNC , 0666);

    exploreFilesRecursively(line1, line2, line3, fc, resultCSV);

    // Closing files and buffers
    close(resultCSV);
    close(configFIle);
    free(line1);
    free(line2);
    free(line3);

    return 0;
}


/*
 * Lists all files and sub-directories recursively 
 * considering path as base path.
 */

void exploreFilesRecursively(char* basePath, char* input, char* output, dirent* filesComper, int resultFile)
{
    char path[1000];
    // dirent is the struct which holds dirPath with tons on prop
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    // Unable to open directory stream
    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        // maybe add here the writing to the csv file to write the student name

        // Avoid getting into hidden and system dir
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            //printf("Folder - %s\n", dp->d_name);

            // Building the char* argv input argument for the executed program
            char* prog_args[2];

            pid_t pid = fork();

            if (pid == 0) {
                // execution result file
                int result = open("result.txt",O_WRONLY | O_APPEND);

                // here the newfd is the file descriptor of stdout (i.e. 1) 
                // All the printf statements will be written in the file 
                dup2(result, 1); 
 
                prog_args[0] = dp->d_name;
                prog_args[1] = input;

                // executing the inner program with the relevant arguments
                // execv("/usr/bin/gcc", dp->d_name);  -- thats for compilation
                // also remember that the child process as been terminated
                execvp(dp->d_name, prog_args);
            }else{
                close(result);
                free(prog_args);
            }

            // comparison between the output file and the expected with the program from part1

            // first of all, we re-use the stdout as the screen and not the file
            dup2(1, result);
            int ret;
            char* args[3];
            // forking again
            pid_t pid2 = fork();
            if(pid2 == 0){
                // Building the char* argv input argument for the executed program  
                args[0] = filesComper->d_name;
                args[1] = result;
                args[2] = output;
                execvp(filesComper->d_name,args);
            }else{
                wait(&ret);
                free(args);
            }
            if(ret == 2){
                // write 100 to the student in the file
                resultFile.write(dp->d_name);
                resultFile.write(",");
                resultFile.write("100");
            }else{
                // write 0 to the student in the file
                resultFile.write(dp->d_name);
                resultFile.write(",");
                resultFile.write("0");
            }
        }
    }

    closedir(dir);
}