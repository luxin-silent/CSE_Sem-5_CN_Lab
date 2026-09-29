---**UDP Time Server Algorithm**---

1. Include necessary system headers for UDP sockets, network IP address structures, string handling, time functions, and standard IO.
2. Create a UDP socket using `socket(AF_INET, SOCK_DGRAM, 0)`.
3. Configure the server socket address structure (`sockaddr_in`) with IPv4 domain, local IP address (`INADDR_ANY`), and the designated port number.
4. Bind the created socket to the configured server address using `bind()`.
5. Initialize memory to handle incoming client requests and store client socket address structures.
6. Enter an infinite loop to keep receiving time requests from clients.
7. Block and wait for a request message from any client using `recvfrom()`, while saving the client's network address.
8. Obtain the current system epoch time using `time(NULL)`.
9. Format the raw epoch time into a readable time string using `ctime()` or `strftime()`.
10. Send the formatted time string back to the client's network address using `sendto()`.
11. Repeat the loop to wait for subsequent requests.
12. Close the server socket upon termination.

---

---**UDP Time Client Algorithm**---

1. Include necessary system headers for UDP sockets, network IP address structures, string handling, and standard IO.
2. Create a UDP socket using `socket(AF_INET, SOCK_DGRAM, 0)`.
3. Configure the server target address structure (`sockaddr_in`) with IPv4 domain, target server IP address using `inet_addr()`, and the matching server port number.
4. Send a dummy time request string or empty message to the server address using `sendto()`.
5. Block and wait to receive the time string response from the server using `recvfrom()`.
6. Null-terminate the received byte buffer to safely handle it as a string.
7. Display the received server time string to standard output.
8. Close the UDP socket using `close()` and terminate execution.
