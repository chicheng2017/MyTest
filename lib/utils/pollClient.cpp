/*
 * pollClient.cpp
 *
 *  Created on: Dec 15, 2014
 *      Author: echgchi
 */

#include "pollClient.h"

pollClient::pollClient() {
	// TODO Auto-generated constructor stub

}

pollClient::~pollClient() {
	// TODO Auto-generated destructor stub
}


int pollClient::startPollClient()
{
    int                 sockfd;
    struct sockaddr_in  servaddr;
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET,IPADDRESS,&servaddr.sin_addr);
    connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));

    handle_connection(sockfd);
    return 0;
}

void pollClient::handle_connection(int sockfd)
{
    char    sendline[MAXLINE],recvline[MAXLINE];
    int     maxfdp,stdineof;
    struct pollfd pfds[2];
    int n;

    pfds[0].fd = sockfd;
    pfds[0].events = POLLIN;

    pfds[1].fd = STDIN_FILENO;
    pfds[1].events = POLLIN;
    for (; ;)
    {
        poll(pfds,2,-1);
        if (pfds[0].revents & POLLIN)
        {
            n = read(sockfd,recvline,MAXLINE);
            if (n == 0)
            {
                    fprintf(stderr,"client: server is closed.\n");
                    close(sockfd);
            }
            write(STDOUT_FILENO,recvline,n);
        }

        if (pfds[1].revents & POLLIN)
        {
            n = read(STDIN_FILENO,sendline,MAXLINE);
            if (n  == 0)
            {
                shutdown(sockfd,SHUT_WR);
        continue;
            }
            write(sockfd,sendline,n);
        }
    }
}