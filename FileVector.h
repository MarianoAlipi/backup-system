/*
 * A simple custom vector type
 * (an array of variable size).
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

#endif /* FILEVECTOR_H */