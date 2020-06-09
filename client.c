/*
 * This program is the client for the backup system.
 * It keeps track of all files and subdirectories
 * located at its path.
 * 
 * This program is designed for Linux systems.
 * 
 */
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "FileVector.h"
#include "FileSender.h"

int main(int argc, char **argv) {
    
    if (argc < 3) {
        printf("usage: %s hostname port\n", argv[0]);
        return 1;
    }

    char SERVER_IP[256];
    int SERVER_PORT;

    strcpy(SERVER_IP, argv[1]);
    SERVER_PORT = atoi(argv[2]);

    if (SERVER_PORT == 0) {
        printf("ERROR: invalid port.\n");
        return 1;
    }

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

    // The vector to track the subdirectories.
    struct Vector dirsVector;
    initVector(&dirsVector);
    struct File rootDir;
    sprintf(rootDir.name, ".");
    rootDir.modTime = 0;
    push(&dirsVector, rootDir);

    // The auxiliary File variable
    // to add files to the vector.
    struct File file;
    
    // An auxiliary integer to use indices.
    int index;
    // A flag to determine if the current file is a directory or not.
    int isDir;
    // The current path.
    char currPath[1024];
    // The name (with path) of the current file.
    char fullfilename[1024];

    while (1) {

        for (int dirI = 0; dirI < dirsVector.size; dirI++) {

            d = opendir(dirsVector.data[dirI].name);
            strcpy(currPath, dirsVector.data[dirI].name);

            if (d) {

                // List all files in the directory.
                while ((dir = readdir(d)) != NULL) {

                    sprintf(fullfilename, "%s/%s", currPath, dir->d_name);

                    // If it's a directory...
                    if (dir->d_type == DT_DIR) {

                        // Ignore the "." and ".." directories.
                        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
                            continue;
                        }

                        isDir = 1;
                        index = findByName(&dirsVector, fullfilename);

                    // Else (it's most likely a file)...
                    } else {

                        // Ignore the executable file of this program.
                        if (strcmp(fullfilename, argv[0]) == 0) {
                            continue;
                        }

                        isDir = 0;
                        index = findByName(&vector, fullfilename);
                    }

                    err = stat(fullfilename, &file_stat);

                    if (err != 0) {
                        printf("ERROR: could not get the modification time of file: '%s'.\n", fullfilename);
                        // return -1;
                        file_stat.st_mtime = 0;
                    }

                    // Detected the creation of a file or directory.
                    if (index == -1) {
                        
                        // Copy the name to the auxiliary File variable.
                        strcpy(file.name, fullfilename);

                        // Set the file's modification time.
                        file.modTime = file_stat.st_mtime;

                        // Add the file to the corresponding vector.
                        if (isDir) {
                            push(&dirsVector, file);
                            printf("Created directory '%s'.\n", file.name);

                            char str[1024];
                            sprintf(str, "%s%s", CREATE_DIR_PREFIX, file.name);
                            if (connectToServer(SERVER_IP, SERVER_PORT) == 0) {
                                if (sendToServer(str) == 0) {
                                    sleep(3);
                                } else {
                                    printf("Transmission failed.\n");
                                }
                            } else {
                                printf("Connection failed.\n");
                            }
                        } else {
                            push(&vector, file);
                            printf("Created file '%s'.\n", file.name);

                            char str[1024];
                            sprintf(str, "%s%s", CREATE_PREFIX, file.name);

                            if (connectToServer(SERVER_IP, SERVER_PORT) == 0) {
                                if (sendToServer(str) == 0) {
                                    sleep(3);
                                } else {
                                    printf("Transmission failed.\n");
                                }
                            } else {
                                printf("Connection failed.\n");
                            }
                        }


                    // The file or directory exists. If it's a file, check if it has been modified.
                    } else if (index >= 0) {

                        strcpy(file.name, fullfilename);

                        // Mark it as checked.
                        if (isDir) {
                            markChecked(&dirsVector, index);
                        } else {
                            markChecked(&vector, index);

                            // Compare the modification times.
                            if (vector.data[index].modTime != file_stat.st_mtime) {
                                // Set the file's new modification time.
                                file.modTime = file_stat.st_mtime;

                                // Update the file in the vector.
                                set(&vector, index, file);
                                markChecked(&vector, index);

                                printf("Modified file '%s'.\n", file.name);

                                char str[1024];
                                sprintf(str, "%s%s", MODIFY_PREFIX, file.name);

                                if (connectToServer(SERVER_IP, SERVER_PORT) == 0) {
                                    if (sendToServer(str) == 0) {
                                        sleep(3);
                                    } else {
                                        printf("Transmission failed.\n");
                                    }
                                } else {
                                    printf("Connection failed.\n");
                                }

                            }
                        }

                    }
                }

                closedir(d);

                // Wait for 1 second until detecting changes again.
                sleep(1);

            }

        }

        // Detect deleted files.
        for (int i = 0; i < vector.size; i++) {
            // The file was not checked.
            // This means it was not found.
            if (vector.data[i].checked == 0) {
                printf("Deleted file '%s'.\n", vector.data[i].name);

                char str[1024];
                sprintf(str, "%s%s", DELETE_PREFIX, vector.data[i].name);

                if (connectToServer(SERVER_IP, SERVER_PORT) == 0) {
                    if (sendToServer(str) == 0) {
                        sleep(3);
                    } else {
                        printf("Transmission failed.\n");
                    }
                } else {
                    printf("Connection failed.\n");
                }

                delete(&vector, i);
                // Subtract 1 from i so that this index is repeated.
                i--;
            }
        }

        // Detect deleted directories.
        for (int i = 0; i < dirsVector.size; i++) {

            // Ignore the "." directory.
            if (strcmp(dirsVector.data[i].name, ".") == 0) {
                continue;
            }

            // The directory was not checked.
            // This means it was not found.
            if (dirsVector.data[i].checked == 0) {
                printf("Deleted directory '%s'.\n", dirsVector.data[i].name);

                char str[1024];
                sprintf(str, "%s%s", DELETE_DIR_PREFIX, dirsVector.data[i].name);
                
                if (connectToServer(SERVER_IP, SERVER_PORT) == 0) {
                    if (sendToServer(str) == 0) {
                        sleep(3);
                    } else {
                        printf("Transmission failed.\n");
                    }
                } else {
                    printf("Connection failed.\n");
                }

                delete(&dirsVector, i);
                // Subtract 1 from i so that this index is repeated.
                i--;
            }
        }

        markAllUnchecked(&vector);
        markAllUnchecked(&dirsVector);

    }

    return 0;
}