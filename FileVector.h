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

struct File {
    char name[256]; // Filename (a path relative to .)
    time_t modTime; // Modification time
};

struct Vector {
    int size;
    struct File *data;
};

void initVector(struct Vector *v) {
    v->data = malloc(sizeof(struct File));
    v->size = 0;
}

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

#endif /* FILEVECTOR_H */