/*
  OsCourse task1 part2 -> C program to iterate over a directory , execute inner program and check expected result
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/fcntl.h>
#include <sys/wait.h>

#define studentProg "main.exe"
#define filesCompare "./filesComper"

void exploreFilesRecursively(const char* basePath, char* input, char* output, FILE* resultFile);

int main(int argc, char* argv[])
{
    if(argc != 2){
        printf("The system got unexcpected amount of items..\nexiting...\n");
        return 0;
    }

    FILE* configFIle = fopen(argv[1], "r");
    
    if(configFIle == NULL){
        printf("There was an error opening the files\n exiting...");
        return 0;
    }

    size_t len = 0;
    
    char* line1 = NULL;
    // Get the first line of the config file to get the path of entire users
    getline(&line1, &len, configFIle);
    // removing "newline" char
    char *newLine1 = strchr(line1, '\n');
	if (newLine1)  *newLine1 = 0;
    
    // Get the second line which is it the system input for every user later on
    char* line2 = NULL;
    getline(&line2, &len, configFIle);
    // removing "newline" char
    char *newLine2 = strchr(line2, '\n');
	if (newLine2)  *newLine2 = 0;

    // Get the third line which is it the system expected output for every user later on
    char* line3 = NULL;
    getline(&line3, &len, configFIle);
    // removing "newline" char
    char *newLine3 = strchr(line3, '\n');
	if (newLine3)  *newLine3 = 0;

    fclose(configFIle);

    FILE* resultCSV = fopen("result.csv", "w");

    exploreFilesRecursively(line1, line2, line3, resultCSV);

    fclose(resultCSV);
    return 0;
}


/*
 * Lists all files and sub-directories 
 * considering path as base path.
 */

void exploreFilesRecursively(const char* basePath, char* input, char* output, FILE* resultFile)
{
    // dirent is the struct which holds dirPath with tons of properties
    struct dirent *dp;
    DIR* dir = opendir(basePath);

    // Unable to open directory stream
    if (!dir){
        printf("Problem opening basePath dir\n");
        exit(0);
    }

    while ((dp = readdir(dir)) != NULL)
    {
        // Avoid getting into hidden and system dir
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0 && dp->d_type == DT_DIR)
        {
            printf("Folder - %s\n", dp->d_name);

            pid_t pid = fork();
            if (pid == 0){

                pid_t pidRunner = fork();

                // execution result file
                int result = open("result.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
                // here the newfd is the file descriptor of stdout (i.e. 1) 
                // All the printf statements will be written in the file 
                close(1);
				dup(result); 
                close(result);
                if(pidRunner < 0){
                    exit(-1);
                }
                else if(pidRunner == 0){
                    // Building the char* argv input argument for the executed program
                    char* prog_args[3];
                    prog_args[0] = studentProg;
                    prog_args[1] = input;
                    prog_args[2] = NULL;
                    // executing the inner program with the relevant arguments
                    // execv("/usr/bin/gcc", dp->d_name);  -- thats for compilation

                    // also remember that the child process as been terminated
                    if(execvp(studentProg, prog_args) == -1){
                        exit(-1);
                    }
                }
                else{
                    if(waitpid(pidRunner, NULL, 0) != pidRunner){
                        exit(-1);
                    }
                    // comparison between the output file and the expected with the program from part1
                    char* args[4];
                    // Building the char* argv input argument for the executed program  
                    args[0] = filesCompare;
                    args[1] = "result.txt";
                    args[2] = output;
                    args[3] = NULL;

                    if(execvp(filesCompare,args) == -1){
                        exit(-1);
                    }
                }
            }else{
                int ret;
                waitpid(pid, &ret, 0);
                if(WEXITSTATUS(ret) == 1 || WEXITSTATUS(ret) == 2){
                    fprintf(resultFile,"%s,", dp->d_name);
                    if(WEXITSTATUS(ret) == 2){
                        // write 100 to the student in the file
                        fprintf(resultFile,"%s", "100\n");
                    }else if(WEXITSTATUS(ret) == 1){
                        // write 0 to the student in the file
                        fprintf(resultFile,"%s", "0\n");
                    }
                }
            }
        }
    }
    closedir(dir);
}