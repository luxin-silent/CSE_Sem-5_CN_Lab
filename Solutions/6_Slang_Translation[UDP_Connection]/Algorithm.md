---**Client Algorithm**---

1. Initialize UDP socket structures and communication buffers.
2. Create a UDP socket.
3. Define the server network address and port number.
4. Prompt the user to enter a sentence containing gen-z or modern slang abbreviations.
5. Read the input sentence from the user into the send buffer.
6. Send the sentence buffer to the server using the UDP socket and server address details.
7. Receive the translated formal sentence from the server via the socket into the receive buffer.
8. Display the translated formal sentence to the user.
9. Close the UDP socket.



---**Server Algorithm**---

1. Initialize UDP socket structures, communication buffers, and slang translation mapping pairs for abbreviations (tbh -> to be honest, ig -> I guess, tbf -> to be fair, atm -> at the moment, irl -> in real life, lol -> laughing out loud, asap -> as soon as possible, omg -> oh my god, ttyl -> talk to you later, idk -> I don't care / I don't know, nvm -> never mind).
2. Create a UDP socket.
3. Bind the UDP socket to the local server port and IP address.
4. Wait to receive a message from the client using the socket while capturing the client address details.
5. Store the incoming sentence into the processing buffer.
6. Tokenize or scan the received sentence word by word to detect slang abbreviations.
7. Replace target matching abbreviations (tbh, ig, tbf, atm, irl, lol, asap, omg, ttyl, idk, nvm) with their formal English expansions, preserving surrounding punctuation and spaces.
8. Construct the final translated formal sentence in an output buffer.
9. Send the translated formal sentence back to the client using the captured client address.
10. Close the UDP socket or return to waiting for subsequent client requests.
