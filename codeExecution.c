/*
  OsCourse task1 part2 -> C program to iterate over a directory recursively, and execute compilation and more
*/

#include <stdio.h>
#include <string.h>

void exploreFilesRecursively(char *path);

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


    exploreFilesRecursively(line1, line2, line3);

    // Closing files and buffers
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

void exploreFilesRecursively(char* basePath, char* input, char* output)
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
        // Avoid getting into hidden and system dir
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            printf("Folder - %s\n", dp->d_name);

            pid_t pid = fork();
            if (pid == 0) {

                // Building the char* argv input argument for the executed program
                char* prog_args[2];
                my_args[0] = dp->d_name;
                my_args[1] = input;
                // executing the inner program with the relevant arguments
                //execv("/usr/bin/gcc", dp->d_name);  -- thats for compilation
                execvp(dp->d_name, prog_args);
                
                exit(127); /* only if execv fails */
    }
    else { /* pid!=0; parent process */
        waitpid(pid,0,0); /* wait for child to exit */
    }

            
        }
    }

    closedir(dir);
}