#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 

    char sendBuff[1025];
    time_t ticks; 

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

/* socket(domain, type, protocol);
domain, which specifies the protocol family of the created socket. For example:

    AF_INET for network protocol IPv4 (IPv4-only)
    AF_INET6 for IPv6 (and in some cases, backward compatible with IPv4)
    AF_UNIX for local socket (using a file)

type, one of:

    SOCK_STREAM (reliable stream-oriented service or Stream Sockets)
    SOCK_DGRAM (datagram service or Datagram Sockets)
    SOCK_SEQPACKET (reliable sequenced packet service)
    SOCK_RAW (raw protocols atop the network layer)

protocol specifying the actual transport protocol to use. The most common are:
IPPROTO_TCP, IPPROTO_SCTP, IPPROTO_UDP, IPPROTO_DCCP. 
These protocols are specified in file netinet/in.h. 
The value 0 may be used to select a default protocol from the selected domain and type

*/




    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

/*
bind() assigns a socket to an address. When a socket is created using socket(), it is only given a protocol family, but not assigned an address. This association with an address must be performed with the bind() system call before the socket can accept connections to other hosts. bind() takes three arguments:

    sockfd, a descriptor representing the socket to perform the bind on.
    my_addr, a pointer to a sockaddr structure representing the address to bind to.
    addrlen, a socklen_t field specifying the size of the sockaddr structure.
*/

    listen(listenfd, 10); 
/*
After a socket has been associated with an address, listen() prepares it for incoming connections. However, this is only necessary for the stream-oriented (connection-oriented) data modes, i.e., for socket types (SOCK_STREAM, SOCK_SEQPACKET). listen() requires two arguments:

    sockfd, a valid socket descriptor.
    backlog, an integer representing the number of pending connections that can be queued up at any one time. The operating system usually places a cap on this value.

Once a connection is accepted, it is dequeued. On success, 0 is returned. If an error occurs, -1 is returned. 

*/

	printf("\n Time Server Started!");

    while(1) //periodical polling
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
/*
The accept() function creates a new socket for each connection and removes the connection from the listen queue. 
It takes the following arguments:

    sockfd, the descriptor of the listening socket that has the connection queued.
    cliaddr, a pointer to a sockaddr structure to receive the client's address information.
    addrlen, a pointer to a socklen_t location that specifies the size of the client address structure passed to 
accept(). When accept() returns, this location indicates how many bytes of the structure were actually used.

The accept() function returns the new socket descriptor for the accepted connection, or -1 if an error occurs. All further communication with the remote host now occurs via this new socket.

Datagram sockets do not require processing by accept() since the receiver may immediately respond to the request using the listening socket. 
*/


        ticks = time(NULL);
        snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
        write(connfd, sendBuff, strlen(sendBuff)); 

        close(connfd);
        sleep(1);
    }
}
