#!/bin/bash

# Author: Ryan Page
# Date: 2/06/2025
# Last Updated: 2/08/2025
# Description: This is a test script that will compile and run the server, intermediate host, and client programs.

# Compile the programs
g++ -o server Server.cpp -std=c++11
g++ -o intermediateHost IntermediateHost.cpp -std=c++11
g++ -o client Client.cpp -std=c++11

# Run the server in the background
echo "Starting server..."
./server &
SERVER_PID=$!

# Run the intermediate host in the background
echo "Starting intermediate host..."
./intermediateHost &
INTERMEDIATE_PID=$!

# Run the client (foreground)
echo "Starting client..."
./client 

# Cleanup: Kill background processes when the script exits
trap "kill $SERVER_PID $INTERMEDIATE_PID" EXIT

