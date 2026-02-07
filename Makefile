# Roll No: MT25055
# Makefile for PA02

CC = gcc
CFLAGS = -Wall -pthread
# Clients reuse the same source file but we create symlinks or copy for clarity if needed
# Here we compile the Generic Client as the client for all 3 parts
TARGETS = MT25055_Part_A1_Server MT25055_Part_A1_Client \
          MT25055_Part_A2_Server MT25055_Part_A2_Client \
          MT25055_Part_A3_Server MT25055_Part_A3_Client

all: $(TARGETS)

MT25055_Part_A1_Server: MT25055_Part_A1_Server.c
	$(CC) $(CFLAGS) -o MT25055_Part_A1_Server MT25055_Part_A1_Server.c

MT25055_Part_A1_Client: MT25055_Part_A1_Client.c
	$(CC) $(CFLAGS) -o MT25055_Part_A1_Client MT25055_Part_A1_Client.c

MT25055_Part_A2_Server: MT25055_Part_A2_Server.c
	$(CC) $(CFLAGS) -o MT25055_Part_A2_Server MT25055_Part_A2_Server.c

# Reuse A1 Client source for A2 Client binary
MT25055_Part_A2_Client: MT25055_Part_A1_Client.c
	$(CC) $(CFLAGS) -o MT25055_Part_A2_Client MT25055_Part_A1_Client.c

MT25055_Part_A3_Server: MT25055_Part_A3_Server.c
	$(CC) $(CFLAGS) -o MT25055_Part_A3_Server MT25055_Part_A3_Server.c

# Reuse A1 Client source for A3 Client binary
MT25055_Part_A3_Client: MT25055_Part_A1_Client.c
	$(CC) $(CFLAGS) -o MT25055_Part_A3_Client MT25055_Part_A1_Client.c

clean:
	rm -f $(TARGETS) perf_out.txt