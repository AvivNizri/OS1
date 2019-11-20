/*
  OsCourse task1 part2 -> C program to iterate over a directory recursively, and execute compilation and more
*/

#include <stdio.h>
#include <string.h>

void exploreFilesRecursively(char *path);

int main(int argc, char* argv[])
{
    if(argc != 3){
        printf("The system got more items than needed..\n exiting...");
        return 0;
    }

    int configFIle = open(strcat(argv[2],argv[1]), O_RDONLY);

    if(configFIle == -1){
        printf("There was an error opening the files\n exiting...");
        return 0;
    }

    char* line1 = NULL;
    size_t len = 0;
    while(getline(&line1, &len, fp) != -1)

    listFilesRecursively(path);

    return 0;
}


/**
 * Lists all files and sub-directories recursively 
 * considering path as base path.
 */
void exploreFilesRecursively(char *basePath)
{
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    // Unable to open directory stream
    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            printf("%s\n", dp->d_name);

            // Construct new path from our base path
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);

            listFilesRecursively(path);
        }
    }

    closedir(dir);
}}