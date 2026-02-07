/*
 * Roll No: MT25055
 * File: MT25055_Part_A3_Server.c
 * Description: A3 Zero-Copy Server (MSG_ZEROCOPY)
 */

#include "MT25055_Part_A_Common.h"

void *handle_client(void *arg) {
    int client_sock = *(int *)arg;
    free(arg);

    int one = 1;
    if (setsockopt(client_sock, SOL_SOCKET, SO_ZEROCOPY, &one, sizeof(one))) {
        perror("setsockopt zerocopy");
    }

    Message msg;
    init_message(&msg, 65536); // [cite: 28, 30]

    struct msghdr msg_header = {0};
    struct iovec iov[NUM_FIELDS];
    for(int i=0; i<NUM_FIELDS; i++) {
        iov[i].iov_base = msg.fields[i];
        iov[i].iov_len = msg.field_sizes[i];
    }
    msg_header.msg_iov = iov;
    msg_header.msg_iovlen = NUM_FIELDS;

    time_t start = time(NULL);
    int send_count = 0;

    while (time(NULL) - start < 5) { // [cite: 31]
        // 1. Send data
        if (sendmsg(client_sock, &msg_header, MSG_ZEROCOPY) <= 0) break;
        send_count++;

        // 2. Optimized Notification: Only check error queue every 32 packets
        // This reduces system call overhead significantly.
        if (send_count % 32 == 0) {
            char buf[256];
            struct msghdr msg_err = {0}; 
            struct iovec iov_err = {.iov_base = buf, .iov_len = sizeof(buf)};
            msg_err.msg_iov = &iov_err;
            msg_err.msg_iovlen = 1;
            msg_err.msg_control = buf;
            msg_err.msg_controllen = sizeof(buf);
            
            // Non-blocking drain of the error queue 
            while (recvmsg(client_sock, &msg_err, MSG_ERRQUEUE | MSG_DONTWAIT) > 0);
        }
    }

    free_message(&msg);
    close(client_sock);
    return NULL;
}

// Main function (Standard Boilerplate)
int main() {
    int server_fd, *new_sock;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) exit(EXIT_FAILURE);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 10);
    printf("A3 Server (Zero-Copy) Listening...\n");
    while (1) {
        new_sock = malloc(sizeof(int));
        *new_sock = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, new_sock);
        pthread_detach(tid);
    }
    return 0;
}