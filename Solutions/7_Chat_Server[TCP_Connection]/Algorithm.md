---**Server Algorithm**---

1. Create a socket for listening to incoming connection requests using the TCP protocol and IPv4 address family.
2. Configure the socket options to allow address and port reuse to prevent bind errors during rapid restarts.
3. Bind the socket to a specified IP address and port number.
4. Set the server socket to listening mode with a designated backlog limit for pending connection queues.
5. Create a master file descriptor set and add the listening socket to this set.
6. Track the highest file descriptor value currently open for monitoring purposes.
7. Allocate memory for an array or list of active client dynamic buffers and tracking structures.
8. Start an infinite main server execution loop.
9. Copy the master file descriptor set to a temporary working set for input operations.
10. Pass the working set to the socket multiplexing function to block until activity occurs on one or more descriptors.
11. Iterate through all valid file descriptors from zero up to the maximum descriptor value.
12. Check if the current file descriptor has pending readable data.
13. If the active descriptor is equal to the listening socket, accept the new incoming connection to obtain a new client socket descriptor.
14. Add the new client descriptor to the master set and update the maximum descriptor tracking value.
15. Send a welcome text string to the newly connected client socket.
16. If the active descriptor is not the listening socket, attempt to read data into a temporary buffer.
17. Check the return value of the read operation.
18. If the read bytes count is zero or negative, handle client disconnection by closing the socket descriptor.
19. Remove the closed descriptor from the master set and clean up the associated client buffer memory.
20. If data is successfully received, format the payload with client identification details.
21. Iterate through all descriptors in the master set, skipping the listening socket and the sender's descriptor.
22. Write the formatted message payload to each remaining valid client descriptor.
23. Repeat the main execution loop continuously to process incoming data and new connections.

---

---**Client Algorithm**---

1. Create a socket for network communication using the TCP protocol and IPv4 address family.
2. Initialize target socket address structures with the destination server IP address and port number.
3. Initiate an active TCP connection to the server using the prepared address structure.
4. Verify the connection status and exit with an error if the connection attempt fails.
5. Create a file descriptor set containing the standard input descriptor and the client socket descriptor.
6. Start an infinite client main communication loop.
7. Reset and repopulate the file descriptor set with standard input and socket descriptors.
8. Pass the descriptor set to the socket multiplexing function to wait for user input or incoming server data.
9. Check if standard input has readable data available.
10. Read user input from standard input into a outgoing message buffer.
11. Check if the input contains a termination command.
12. If a termination command is detected, close the client socket and terminate the client execution.
13. Send the message buffer contents over the socket to the server.
14. Check if the server socket has readable data available.
15. Read the incoming message payload from the socket into a receiving buffer.
16. Check if the read operation returns zero bytes indicating the server closed the connection.
17. If the server disconnected, output an notification message, close the socket, and exit.
18. Print the received message payload from the buffer to standard output.
19. Clear all temporary message buffers.
20. Repeat the client main loop continuously.
