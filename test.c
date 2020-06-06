#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "FileVector.h"

int main(void) {
    
    /*
     * These variables are used to list
     * files and directories in the
     * specified path.
     * In this case '.'.
     */
    DIR *d;
    struct dirent *dir;
    d = opendir(".");

    /*
    * Relevant values for dir->dtype:
    * 
    * DT_DIR: directory (unsigned int)
    * DT_REG: regular file (unsigned int)
    */ 

    // The vector to track the files.
    struct Vector vector;
    initVector(&vector);

    // The auxiliary File variable
    // to add files to the vector.
    struct File file;
    
    if (d) {

        // List all files in the directory.
        while ((dir = readdir(d)) != NULL) {
            printf("%s\t(%s)\n", dir->d_name, dir->d_type == DT_DIR ? "directory" : dir->d_type == DT_REG ? "file" : "unknown");

            // Copy the name to the auxiliary File variable.
            strcpy(file.name, dir->d_name);

            // Add it to the vector only if it's a file.
            // We must implement recursive directory searching too.
            if (dir->d_type == DT_REG) {
                struct stat file_stat;
                int err = stat(dir->d_name, &file_stat);

                if (err != 0) {
                    printf("ERROR\n");
                    return -1;
                }

                // Set the file's modification time.
                file.modTime = file_stat.st_mtime;
                push(&vector, file);
            }
        }

        closedir(d);

    }

    // Print the contents of the vector for testing purposes.
    printf("\nVector is of size %d\n", vector.size);
    for (int i = 0; i < vector.size; i++) {
        file = vector.data[i];
        printf(" - %s : %li\n", file.name, file.modTime);
    }

    return 0;
}