/*
 * Roll No: MT25055
 * File: MT25055_Part_A1_Client.c
 */
#include "MT25055_Part_A_Common.h"

typedef struct {
    size_t msg_size;
    double throughput; 
    double latency;    
} thread_args_t;

void *client_thread(void *arg) {
    thread_args_t *args = (thread_args_t *)arg;
    int sock = 0;
    struct sockaddr_in serv_addr;
    char *buffer = malloc(args->msg_size);
    if (!buffer) return NULL;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        free(buffer); return NULL;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        free(buffer); return NULL;
    }

    // Connect with retry
    int retries = 10;
    while (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        if (retries-- <= 0) { free(buffer); close(sock); return NULL; }
        usleep(100000);
    }

    // Measure
    struct timespec start, end;
    long iterations = 0;
    long total_bytes = 0;
    clock_gettime(CLOCK_MONOTONIC, &start);

    while (recv(sock, buffer, args->msg_size, 0) > 0) {
        total_bytes += args->msg_size;
        iterations++;
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    close(sock);
    free(buffer);

    double time_sec = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    if (time_sec > 0 && iterations > 0) {
        args->throughput = (double)(total_bytes * 8) / (time_sec * 1e9); // Gbps
        args->latency = (time_sec * 1e6) / iterations; // Microseconds
    } else {
        args->throughput = 0; args->latency = 0;
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) return -1;
    int num_threads = atoi(argv[1]);
    size_t msg_size = atoi(argv[2]);

    pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
    thread_args_t *args = malloc(num_threads * sizeof(thread_args_t));

    for (int i = 0; i < num_threads; i++) {
        args[i].msg_size = msg_size;
        pthread_create(&threads[i], NULL, client_thread, &args[i]);
    }

    double total_tput = 0;
    double avg_latency = 0;
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        total_tput += args[i].throughput;
        avg_latency += args[i].latency;
    }
    if (num_threads > 0) avg_latency /= num_threads;

    printf("%.4f %.2f\n", total_tput, avg_latency);

    free(args); free(threads);
    return 0;
}