# TCP Echo Server and Client – Project Documentation

## Project Description

This project implements a simple TCP echo server and client in C++.
The server listens for incoming connections and handles multiple clients concurrently
using detached threads. Each client can send messages, which the server echoes back
to the sender. This setup demonstrates basic socket programming, multithreading,
and client-server communication over TCP.

**Note:**
The current implementation does **not** provide direct communication between clients
(peer-to-peer). Instead, multiple clients can independently connect to the server,
which echoes messages back to the respective sender only.

---

## How to Build and Run

1. Clone the repository:

   ```bash
   git clone https://github.com/husxk/so2-project
   cd so2-project/2
   ```

2. Build the server and client executables:

   ```bash
   make
   ```

3. Run the server by specifying the port to listen on:

   ```bash
   ./build/server <PORT>
   ```

4. Run the client by specifying the server IP address and port:

   ```bash
   ./build/client <SERVER_IP> <PORT>
   ```

## Program Components

| Executable | Role                                               |
|------------|----------------------------------------------------|
| server     | Listens for connections and handles clients concurrently |
| client     | Connects to server and sends messages, receiving echoed responses |

## How It Works

### Server

- Creates a listening socket bound to the specified port.
- Accepts client connections in a loop.
- For each client, spawns a detached thread to handle communication.
- Each thread reads data from the client and echoes it back.
- Handles clients independently and closes connection on disconnect or error.

### Client

- Connects to the server IP and port.
- Reads user input from standard input.
- Sends messages to the server.
- Receives echoed messages and prints them to the console.

## Requirements and Behavior

- Supports multiple clients concurrently.
- Handles partial sends and receives correctly.
- Prints connection and message logs on server console.
- Properly closes sockets on disconnect or errors.
- Minimal error handling with descriptive messages.

## Build System

- Uses a simple Makefile to compile server and client.
- Executables are placed in the `build/` directory.
- Run `make clean` to remove built binaries.
