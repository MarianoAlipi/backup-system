/*
 * This file is a part of the client for the backup system.
 * It defines structures of a custom vector (an array of
 * variable size) to keep track of the files and subdirectories.
 * 
 * This program is designed for Linux systems.
 * 
 */

#ifndef FILEVECTOR_H
#define FILEVECTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// The File structure holding the name and modification time.
struct File {
    char name[256]; // Filename (a path relative to .)
    time_t modTime; // Modification time
    int checked;    // Flag to determine if the file still exists this "round"
};

// The Vector structure to keep track of files.
struct Vector {
    int size;
    struct File *data;
};

// A function to initialize the vector.
void initVector(struct Vector *v) {
    v->data = malloc(sizeof(struct File));
    v->size = 0;
}

// A function to add an item to the end of the vector.
// Returns the resulting size of the vector.
int push(struct Vector *v, struct File f) {

    f.checked = 1;

    if (v == NULL) {
        printf("ERROR: vector not initialized!\n");
        return -1;
    }

    if (v->size == 0) {
        v->data[0] = f;
        v->size++;
    } else {
        v->data = realloc(v->data, (v->size + 1) * sizeof(struct File));
        v->data[v->size] = f;
        v->size++;
    }

    return v->size;

}

// A function to delete an item at index 'index' from the vector.
// Returns the resulting size of the vector.
int delete(struct Vector *v, int index) {

    if (v == NULL) {
        printf("ERROR: vector not initialized!\n");
        return -1;
    }

    if (index < 0 || index >= v->size) {
        printf("ERROR: index out of bounds (%d). Size is %d.\n", index, v->size);
        return -1;
    }

    for (int i = index; i < v->size - 1; i++) {
        v->data[i] = v->data[i + 1];
    }

    v->size--;
    v->data = realloc(v->data, v->size * sizeof(struct File));

    return v->size;

}

// A function to set/replace the value of the specified index.
// Returns 0 if successful, -1 otherwise.
int set(struct Vector *v, int index, struct File f) {

    if (v == NULL) {
        printf("ERROR: vector not initialized!\n");
        return -1;
    }

    if (index < 0 || index >= v->size) {
        printf("ERROR: index out of bounds (%d). Size is %d.\n", index, v->size);
        return -1;
    }

    v->data[index] = f;

    return 0;

}

// Search for the file with the specified name and return its index in the vector.
// If the file is not in the vector, return -1.
// If the vector is not initialized, return -2.
int findByName(struct Vector *v, char name[256]) {

    if (v == NULL) {
        printf("ERROR: vector not initialized!\n");
        return -2;
    }

    for (int i = 0; i < v->size; i++) {
        if (strcmp(v->data[i].name, name) == 0) {
            return i;
        }
    }

    return -1;

}

// Mark all the files as unchecked.
// This resets the flag 'checked' of every file in the vector.
// Returns the number of items changed or -1 if a problem was found.
int markAllUnchecked(struct Vector *v) {

    int i;

    if (v == NULL) {
        printf("ERROR: vector not initialized!\n");
        return -1;
    }

    for (i = 0; i < v->size; i++) {
        v->data[i].checked = 0;
    }

    return i;
}

// Mark the file at the specified index as checked.
// Returns 0 if successful.
// Returns -1 if out of bounds.
// Returns -2 if vector is not initialized.
int markChecked(struct Vector *v, int index) {

    if (v == NULL) {
        printf("ERROR: vector not initialized!\n");
        return -2;
    }

    if (index < 0 || index >= v->size) {
        printf("ERROR: index out of bounds (%d). Size is %d.\n", index, v->size);
        return -1;
    }

    v->data[index].checked = 1;

    return 0;

}

#endif /* FILEVECTOR_H */