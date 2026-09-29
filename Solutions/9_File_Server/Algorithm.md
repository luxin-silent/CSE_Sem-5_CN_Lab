---**Server Algorithm**---

1. Create a TCP socket, bind it to a local port, and set it to listen for incoming connection requests.
2. Accept a connection from a client and fork a new child process to handle the client concurrently.
3. In the child process, retrieve its process ID and receive the requested filename from the client.
4. Check if the requested file exists on the server.
5. If the file exists, send a success message containing the process ID to the client, followed by the file contents read in chunks.
6. If the file does not exist, send an error message containing the process ID to the client indicating the file was not found.
7. Close the connected socket in both parent and child processes, clean up child process resources to prevent zombies, and loop back to accept the next client.

---

---**Client Algorithm**---

1. Create a TCP socket and define the server's address and port details.
2. Connect the socket to the server.
3. Read the desired filename from the user and send it to the server.
4. Receive the initial response message containing the server's process ID and status message from the server.
5. Display the received process ID and status message on the user's screen.
6. Continuously receive and display the remaining data sent by the server until the transmission finishes.
7. Close the socket and terminate the client program.
