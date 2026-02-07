/*
 * Roll No: MT25055
 * File: MT25055_Part_A1_Server.c
 * Description: A1 Two-Copy Server
 */

#include "MT25055_Part_A_Common.h"

void *handle_client(void *arg) {
    int client_sock = *(int *)arg;
    free(arg);

    // Message setup (Size negotiated or fixed for experiment simplicity)
    // Here we default to a large size, but logic adapts to what's allocated
    Message msg;
    init_message(&msg, 65536); 

    // ALLOCATION FOR COPY 1: User-space linearization buffer
    char *send_buffer = (char *)malloc(msg.total_size);

    // Run for fixed duration [cite: 31]
    time_t start = time(NULL);
    while (time(NULL) - start < 5) {
        // COPY 1: Assemble 8 scattered fields into one user buffer
        size_t offset = 0;
        for (int i = 0; i < NUM_FIELDS; i++) {
            memcpy(send_buffer + offset, msg.fields[i], msg.field_sizes[i]);
            offset += msg.field_sizes[i];
        }

        // COPY 2: Standard send (User buffer -> Kernel buffer) [cite: 40]
        if (send(client_sock, send_buffer, msg.total_size, 0) <= 0) break;
    }

    free(send_buffer);
    free_message(&msg);
    close(client_sock);
    return NULL;
}

int main() {
    int server_fd, *new_sock;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { perror("socket failed"); exit(EXIT_FAILURE); }
    
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) { perror("bind failed"); exit(EXIT_FAILURE); }
    listen(server_fd, 10);

    printf("A1 Server (Two-Copy) Listening...\n");
    while (1) {
        new_sock = malloc(sizeof(int));
        *new_sock = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (*new_sock >= 0) {
            pthread_t tid;
            pthread_create(&tid, NULL, handle_client, new_sock);
            pthread_detach(tid); // One thread per client [cite: 25]
        } else {
            free(new_sock);
        }
    }
    return 0;
}