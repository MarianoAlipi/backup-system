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

    // This variable is used to get a file's modification time.
    struct stat file_stat;
    int err;

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
    
    // An auxiliary integer to use indices.
    int index;

    while (1) {

        d = opendir(".");

        if (d) {

            // List all files in the directory.
            while ((dir = readdir(d)) != NULL) {
                /* DEBUGGING: print the file's name and type.
                 * printf("%s\t(%s)\n", dir->d_name, dir->d_type == DT_DIR ? "directory" : dir->d_type == DT_REG ? "file" : "unknown");
                 */
                index = findByName(&vector, dir->d_name);

                err = stat(dir->d_name, &file_stat);

                if (err != 0) {
                    printf("ERROR: could not get the file's modification time.\n");
                    return -1;
                }

                // Detected the creation of a file.
                if (index == -1 && dir->d_type == DT_REG) {
                    // Copy the name to the auxiliary File variable.
                    strcpy(file.name, dir->d_name);

                    // Set the file's modification time.
                    file.modTime = file_stat.st_mtime;

                    // Add the file to the vector.
                    push(&vector, file);

                    printf("Created file '%s'.\n", file.name);

                // The file exists, but check if it has been modified.
                } else if (index >= 0 && dir->d_type == DT_REG) {

                    // Mark it as checked.
                    markChecked(&vector, index);

                    // Compare the modification times.
                    if (vector.data[index].modTime != file_stat.st_mtime) {
                        // Set the file's new modification time.
                        file.modTime = file_stat.st_mtime;

                        // Update the file in the vector.
                        set(&vector, index, file);

                        printf("Modified file '%s'.\n", file.name);

                    }
                }
            }

            closedir(d);

            // Detect deleted files.
            for (int i = 0; i < vector.size; i++) {
                // The file was not checked.
                // This means it was not found.
                if (vector.data[i].checked == 0) {
                    printf("Deleted file '%s'.\n", vector.data[i].name);
                    delete(&vector, i);
                    // Subtract 1 from i so that this index is repeated.
                    i--;
                }
            }

            markAllUnchecked(&vector);

            // Wait for 1 second until detecting changes again.
            sleep(1);

        }

    }

    // Print the contents of the vector for testing purposes.
    printf("\nVector is of size %d\n", vector.size);
    for (int i = 0; i < vector.size; i++) {
        file = vector.data[i];
        printf(" - %s : %li\n", file.name, file.modTime);
    }

    printf("Find by name 'test': %d\n", findByName(&vector, "test"));

    return 0;
}