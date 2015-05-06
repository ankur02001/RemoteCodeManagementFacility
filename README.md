# Message-Passing
 Design and Implement a socket-based message-passing communication channel.

Messages are modeled after HTTP messages containing a text header and optionally a body that may consist of binary or text data.
The header contains a sequence of lines, the first of which is a command, and remaining header lines contain message attributes2
e.g., address of sender and receiver, and, if the message contains a body, a content-length attribute that specifies the number
of bytes in the body. The message header is terminated with a blank line. 
