/*
 * This program is the server for the backup system.
 * It receives instructions from the client as well
 * as the needed files for performing the backup.
 * 
 * This program is designed for Linux systems.
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // read, write
#include <arpa/inet.h>
#include <sys/types.h>  // socket, bind, accept, open
#include <sys/socket.h> // socket, bind, listen, accept
#include <sys/stat.h>   // open
#include <fcntl.h>      // open
#include <errno.h>

#define MAXBUF  1024
#define CREATE_PREFIX "create:"
#define CREATE_DIR_PREFIX "createDir:"
#define DELETE_PREFIX "delete:"
#define DELETE_DIR_PREFIX "deleteDir:"
#define MODIFY_PREFIX "modify:"


// Receive the contents of a file from the client and save it in
// a file named 'file_name'.
void receiveFile(char* file_name, int client_sockfd, char buf[MAXBUF]) {

    int des_fd, file_read_len;

    des_fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0700);
    if(!des_fd) {
        perror("Error opening file: ");
        return;
    }   

    while(1) {
        memset(buf, 0x00, MAXBUF);
        file_read_len = read(client_sockfd, buf, MAXBUF);
        write(des_fd, buf, file_read_len);
        
        if (file_read_len < MAXBUF) {
            if (file_read_len == -1) {
                printf("File is empty or the connection timed out.\n");
            } else {
               printf("Finished receiving file.\n");
            }
            break;
        }
    }
}

int main(int argc, char **argv) {

    int server_sockfd;
    int client_sockfd;
    int des_fd;
    struct sockaddr_in serveraddr, clientaddr;
    int client_len, read_len, file_read_len;
    char buf[MAXBUF];

    int check_bind;
    client_len = sizeof(clientaddr);

    if (argc < 2) {
        printf("usage: %s portNumber\n", argv[0]);
        return 1;
    }

    const int PORT = atoi(argv[1]);

    if (PORT == 0) {
        printf("ERROR: invalid port.\n");
        return 1;
    }

    printf("Running on port: %s\n\n", argv[1]);

    // socket()
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sockfd == -1) {
        perror("Socket error: ");
        exit(0);
    }

    // bind()
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family       = AF_INET;
    serveraddr.sin_addr.s_addr  = htonl(INADDR_ANY);
    serveraddr.sin_port         = htons(PORT);

    if (bind(server_sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) > 0) {
        perror("Bind error: ");
        exit(0);
    }

    // listen
    if (listen(server_sockfd, 5) != 0) {
        perror("Listen error: ");
    }

    while(1) {

        char file_name[MAXBUF];
        memset(buf, 0x00, MAXBUF);

        // accept()
        client_sockfd = accept(server_sockfd, (struct sockaddr *)&clientaddr, &client_len);
        printf("New connection: %s\n", inet_ntoa(clientaddr.sin_addr));

        struct timeval tv;
        tv.tv_sec = 2;
        tv.tv_usec = 0;
        setsockopt(client_sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

        // Filename
        read_len = read(client_sockfd, buf, MAXBUF);
        if(read_len > 0) {
            strcpy(file_name, buf);
            printf("%s: %s\n", inet_ntoa(clientaddr.sin_addr), file_name);
        } else {
            close(client_sockfd);
            break;
        }

        // If it's a 'create file' instruction...
        if (strstr(file_name, CREATE_PREFIX) != NULL) {

            char tmpName[256];

            int posOfColon;

            for(posOfColon = 0; posOfColon < strlen(file_name); posOfColon++) {
                if (file_name[posOfColon] == ':') {
                    break;
                }
            }

            int c = 0;
            while (c < strlen(file_name) - strlen(CREATE_PREFIX)) {
                tmpName[c] = file_name[posOfColon + 1 + c];
                c++;
            }
            tmpName[c] = '\0';

            receiveFile(tmpName, client_sockfd, buf);

        // If it's a 'create directory' instruction...
        } else if (strstr(file_name, CREATE_DIR_PREFIX) != NULL) {

            char tmpName[256];

            int posOfColon;

            for(posOfColon = 0; posOfColon < strlen(file_name); posOfColon++) {
                if (file_name[posOfColon] == ':') {
                    break;
                }
            }

            int c = 0;
            while (c < strlen(file_name) - strlen(CREATE_DIR_PREFIX)) {
                tmpName[c] = file_name[posOfColon + 1 + c];
                c++;
            }
            tmpName[c] = '\0';

            int res = mkdir(tmpName, 0700);
            
            if (res == 0) {
                printf("Created directory '%s'.\n", tmpName);
            }


        // If it's a 'modify file' instruction...
        } else if (strstr(file_name, MODIFY_PREFIX) != NULL) {

            char tmpName[256];

            int posOfColon;

            for(posOfColon = 0; posOfColon < strlen(file_name); posOfColon++) {
                if (file_name[posOfColon] == ':') {
                    break;
                }
            }

            int c = 0;
            while (c < strlen(file_name) - strlen(MODIFY_PREFIX)) {
                tmpName[c] = file_name[posOfColon + 1 + c];
                c++;
            }
            tmpName[c] = '\0';

            receiveFile(tmpName, client_sockfd, buf);
            
        // If it's a 'delete file' instruction...
        } else if (strstr(file_name, DELETE_PREFIX) != NULL) {

            char tmpName[256];

            int posOfColon;

            for(posOfColon = 0; posOfColon < strlen(file_name); posOfColon++) {
                if (file_name[posOfColon] == ':') {
                    break;
                }
            }

            int c = 0;
            while (c < strlen(file_name) - strlen(DELETE_PREFIX)) {
                tmpName[c] = file_name[posOfColon + 1 + c];
                c++;
            }
            tmpName[c] = '\0';

            remove(tmpName);

        // If it's a 'delete directory' instruction...
        } else if (strstr(file_name, DELETE_DIR_PREFIX) != NULL) {

            char tmpName[256];

            int posOfColon;

            for(posOfColon = 0; posOfColon < strlen(file_name); posOfColon++) {
                if (file_name[posOfColon] == ':') {
                    break;
                }
            }

            int c = 0;
            while (c < strlen(file_name) - strlen(DELETE_DIR_PREFIX)) {
                tmpName[c] = file_name[posOfColon + 1 + c];
                c++;
            }
            tmpName[c] = '\0';

            remove(tmpName);
            
        }

        close(client_sockfd);
        printf("Connection ended.\n\n");
        
        close(des_fd);

    }

    close(server_sockfd);
    return 0;
}