/*
 * Roll No: MT25055
 * File: MT25055_Part_A2_Server.c
 * Description: A2 One-Copy Server (sendmsg/iovec)
 */

#include "MT25055_Part_A_Common.h"

void *handle_client(void *arg) {
    int client_sock = *(int *)arg;
    free(arg);

    Message msg;
    init_message(&msg, 65536);

    // Prepare iovec - NO intermediate user buffer allocation needed
    struct msghdr msg_header = {0};
    struct iovec iov[NUM_FIELDS];

    // Point iovec entries directly to the 8 dynamic fields [cite: 46]
    for(int i=0; i<NUM_FIELDS; i++) {
        iov[i].iov_base = msg.fields[i];
        iov[i].iov_len = msg.field_sizes[i];
    }
    msg_header.msg_iov = iov;
    msg_header.msg_iovlen = NUM_FIELDS;

    time_t start = time(NULL);
    while (time(NULL) - start < 5) {
        // ONE COPY: Kernel gathers data directly from fields
        if (sendmsg(client_sock, &msg_header, 0) <= 0) break;
    }

    free_message(&msg);
    close(client_sock);
    return NULL;
}

// Main function identical to A1 (omitted for brevity, copy main from A1_Server)
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
    printf("A2 Server (One-Copy) Listening...\n");
    while (1) {
        new_sock = malloc(sizeof(int));
        *new_sock = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, new_sock);
        pthread_detach(tid);
    }
    return 0;
}