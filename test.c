/*
 * This program displays the names of all files in the current directory.
 */

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "FileVector.h"

/*
 * dir->d_type values:
 * DT_DIR: directory (unsigned int)
 * DT_REG: regular file (unsigned int)
 * 
 */ 

int main(void) {
    
    DIR *d;
    struct dirent *dir;
    d = opendir(".");

    struct Vector vector;
    initVector(&vector);

    struct File file;
    
    if (d) {

        while ((dir = readdir(d)) != NULL) {
            printf("%s\t(%s)\n", dir->d_name, dir->d_type == DT_DIR ? "directory" : dir->d_type == DT_REG ? "file" : "unknown");

            strcpy(file.name, dir->d_name);

            if (dir->d_type == DT_REG) {
                struct stat file_stat;
                int err = stat(dir->d_name, &file_stat);

                if (err != 0) {
                    printf("ERROR\n");
                    return -1;
                }

                file.modTime = file_stat.st_mtime;
                push(&vector, file);
            }
        }

        closedir(d);

    }

    printf("\nVector is of size %d\n", vector.size);
    for (int i = 0; i < vector.size; i++) {
        file = vector.data[i];
        printf(" - %s : %li\n", file.name, file.modTime);
    }

    return(0);
}