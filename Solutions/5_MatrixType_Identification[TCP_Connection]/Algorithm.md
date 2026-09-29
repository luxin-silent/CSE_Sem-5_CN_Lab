
---**Algorithm for Server**---

1. Create a socket for IPv4 network communication using the TCP protocol.
2. Bind the socket to a specific local port number and network IP address.
3. Set the server socket to listening mode to wait for incoming client connection requests.
4. Accept an incoming client connection request, establishing a dedicated communication socket for that client.
5. Receive the integer value representing the order of the square matrix from the connected client socket.
6. Allocate memory or declare space to store a square matrix of size specified by the received order value.
7. Receive the elements of the square matrix sequentially from the client socket.
8. Initialize three boolean status flags indicating whether the matrix is upper triangular, lower triangular, and diagonal to true.
9. Iterate through all rows and columns of the matrix using zero-indexed row and column variables.
10. Check if the row index is greater than the column index and the matrix element at that location is non-zero, set the upper triangular flag to false.
11. Check if the row index is less than the column index and the matrix element at that location is non-zero, set the lower triangular flag to false.
12. Check if the row index is not equal to the column index and the matrix element at that location is non-zero, set the diagonal flag to false.
13. Determine the matrix type string based on the flags: if the diagonal flag is true, set the matrix type string to "Diagonal Matrix"; else if the upper triangular flag is true, set it to "Upper Triangular Matrix"; else if the lower triangular flag is true, set it to "Lower Triangular Matrix"; otherwise set it to "Neither Upper, Lower, nor Diagonal Matrix".
14. Send the determined matrix type string through the client socket back to the client.
15. Close the client communication socket and the server socket to release system resources.


---**Algorithm for Client**----

1. Read the positive integer input representing the order of the square matrix from the user.
2. Create a socket for IPv4 network communication using the TCP protocol.
3. Connect the client socket to the server using the server's IP address and designated port number.
4. Send the integer value representing the order of the square matrix to the server.
5. Initialize a random number generator using the current system time as the seed.
6. Populate the square matrix by generating random integer values in the range from 1 to 50 for each row and column position.
7. Display the generated square matrix on the screen.
8. Send all generated elements of the square matrix sequentially through the socket connection to the server.
9. Receive the string containing the matrix type classification result from the server through the socket.
10. Display the received matrix type string to the user.
11. Close the socket connection to release system resources.


