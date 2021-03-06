/*
 * This file is a part of the client for the backup system.
 * It contains functions to connect to the server and
 * send the instructions and files.
 * 
 * This program is designed for Linux systems.
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAXBUF  1024
#define CREATE_PREFIX "create:"
#define CREATE_DIR_PREFIX "createDir:"
#define DELETE_PREFIX "delete:"
#define DELETE_DIR_PREFIX "deleteDir:"
#define MODIFY_PREFIX "modify:"

struct sockaddr_in serv_addr;
int     s;
int         source_fd;
char        buf[MAXBUF];
int         file_name_len, read_len;

int PORT;
char IP[256];

// Try to connect to the server.
// Returns 0 on success and 1 otherwise.
int connectToServer(char ip[256], int port) {

    PORT = port;

    if (strcmp(ip, "localhost") == 0) {
        strcpy(IP, "127.0.0.1");
    }

    if (PORT == 0) {
        printf("Invalid port.\n");
        return 1;
    }

    /* socket() */
    s = socket(AF_INET, SOCK_STREAM, 0);
    if(s == -1) {
        return 1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(IP);
    serv_addr.sin_port = htons(PORT);

    if(connect(s, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("Connection error: ");
        printf("Failed to connect.\n");
        close(s);
        return 1;
    }

    return 0;

}

// Sends the instruction to the server and any
// needed files.
// Returns 0 on success and 1 otherwise.
int sendToServer(char message[1024]) {

    memset(buf, 0x00, MAXBUF);

    strcpy(buf, message);

    file_name_len = strlen(buf) + 1;

    // The filename or instruction.
    send(s, buf, file_name_len, 0);

    // If it's a 'create file' instruction...
    if (strstr(buf, CREATE_PREFIX) != NULL) {

        char tmpName[256];

        int posOfColon;

        for(posOfColon = 0; posOfColon < strlen(buf); posOfColon++) {
            if (buf[posOfColon] == ':') {
                break;
            }
        }

        int c = 0;
        while (c < strlen(buf) - strlen(CREATE_PREFIX)) {
            tmpName[c] = buf[posOfColon + 1 + c];
            c++;
        }
        tmpName[c] = '\0';

        // Send the file's contents.
        source_fd = open(tmpName, O_RDONLY);
        if(!source_fd) {
            perror("Error: ");
            return 1;
        }

        while(1) {
            printf("sending...\n");
            memset(buf, 0x00, MAXBUF);
            read_len = read(source_fd, buf, MAXBUF);
            send(s, buf, read_len, 0);
            if(read_len == 0) {
                break;
            }

        }

        printf("done.\n");

    // If it's a 'modify file' instruction...
    } else if (strstr(buf, MODIFY_PREFIX) != NULL) {

        char tmpName[256];

        int posOfColon;

        for(posOfColon = 0; posOfColon < strlen(buf); posOfColon++) {
            if (buf[posOfColon] == ':') {
                break;
            }
        }

        int c = 0;
        while (c < strlen(buf) - strlen(MODIFY_PREFIX)) {
            tmpName[c] = buf[posOfColon + 1 + c];
            c++;
        }
        tmpName[c] = '\0';

        // Send the file's contents.
        source_fd = open(tmpName, O_RDONLY);
        if(!source_fd) {
            perror("Error: ");
            return 1;
        }

        while(1) {
            memset(buf, 0x00, MAXBUF);
            read_len = read(source_fd, buf, MAXBUF);
            send(s, buf, read_len, 0);
            if(read_len == 0) {
                break;
            }

        }
    }

    close(source_fd);

    return 0;
}