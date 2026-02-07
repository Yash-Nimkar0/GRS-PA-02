/*
 * Roll No: MT25055
 * File: MT25055_Part_A_Common.h
 * Description: Common definitions for PA02 Server and Client
 */

#ifndef COMMON_H
#define COMMON_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <time.h>
#include <sys/uio.h>
#include <errno.h>
#include <linux/errqueue.h>

#define SERVER_IP "10.0.0.1"
#define PORT 8080
#define NUM_FIELDS 8

// Structure comprising 8 dynamically allocated string fields 
typedef struct {
    char *fields[NUM_FIELDS];
    size_t field_sizes[NUM_FIELDS];
    size_t total_size;
} Message;

// Helper to allocate and fill message with data [cite: 30]
void init_message(Message *msg, size_t total_msg_size) {
    msg->total_size = total_msg_size;
    size_t base_chunk = total_msg_size / NUM_FIELDS;
    
    for (int i = 0; i < NUM_FIELDS; i++) {
        // Adjust last chunk to handle remainders
        size_t current_chunk = (i == NUM_FIELDS - 1) ? 
            (total_msg_size - base_chunk * (NUM_FIELDS - 1)) : base_chunk;
            
        msg->fields[i] = (char *)malloc(current_chunk);
        if (!msg->fields[i]) {
            perror("Malloc failed");
            exit(EXIT_FAILURE);
        }
        msg->field_sizes[i] = current_chunk;
        memset(msg->fields[i], 'A' + i, current_chunk); // Fill with dummy data
    }
}

// Free the heap-allocated buffers
void free_message(Message *msg) {
    for (int i = 0; i < NUM_FIELDS; i++) {
        free(msg->fields[i]);
    }
}

#endif