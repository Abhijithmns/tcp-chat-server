# TCP Chat Server (C)

A simple multithreaded TCP chat server written in C using POSIX sockets and pthreads.
## Project Structure


├── Server/ # Server-side code

├── Client/ # Client-side code

├── Utils/ # Socket utility functions

├── CMakeLists.txt

├── .gitignore

└── README.md

## Features

- IPv4 TCP server
- One thread per client
- Multiple clients can connect simultaneously
- Messages from one client are broadcast to others
- Clean separation of Server / Client / Utils



## Build Instructions

```bash
mkdir build
cd build
cmake ..
make
