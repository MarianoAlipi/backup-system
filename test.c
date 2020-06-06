/*
 * This program displays the names of all files in the current directory.
 */

#include <dirent.h>

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
    
    if (d) {

        while ((dir = readdir(d)) != NULL) {
            printf("%s\t(%s)\n", dir->d_name, dir->d_type == DT_DIR ? "directory" : dir->d_type == DT_REG ? "file" : "unknown");
        }

        closedir(d);

    }

  return(0);
}