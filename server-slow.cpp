/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <strings.h>
#include <stdlib.h>
#include <pthread.h>
#include <vector>

using namespace std;

void error(string msg)
{
    cout<<msg<<endl;
    return ;
}

int main(int argc, char *argv[])
{
    FILE *f;
    pid_t pid;
    int sockfd, newsockfd, portno,count;
    unsigned int  clilen;
    char buffer[513];
    char buffer1[513];
    char *s;
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    if (argc < 2)
    {
        fprintf(stderr,"ERROR, no port provided\n");
        return 1;
    }

    /* create socket */

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    /* fill in port number to listen on. IP address can be anything (INADDR_ANY) */

    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    /* bind socket to this port number on this machine */
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    /* listen for incoming connection requests */
    bool flag = true;
    int status;
    while(flag)
    {
        flag = false;
        listen(sockfd,500);                                                  //to listen on the port.
        clilen = sizeof(cli_addr);      
        newsockfd = accept(sockfd , (struct sockaddr *) &cli_addr, &clilen); //accept the connections.
        if (newsockfd < 0)
            error("ERROR on accept");
        /* read message from client */
        pid = fork();                                                        //fork a new process.
        if(pid != 0 )
        {
            flag = true;
            count++ ;

        }
        /* accept a new request, create a newsockfd */
        if(pid == 0)
        {   
            bzero(buffer,512);
            cout<<newsockfd<<endl;
            n = read(newsockfd,s,20);                                       //read the filename.
            if (n < 0) error("ERROR reading from socket");
            char *filename = new char[15];
            cout<<"forked"<<endl;
            int i = 0;
            while(true)
            {
                if(i > 2)
                {
                    if(filename[i-1]=='t'&&filename[i-2]=='x'&&filename[i-3]=='t')
                    {
                        break;
                    }
                }
                filename[i] = s[i+9];
                i++;

            }
            filename[i]='\0';   
            cout<<filename<<endl;                                         //get the file name.
            f = fopen(filename,"r");                                     //open file.
            while(!feof(f))
            {
                n = fread(buffer1,1,512,f);                              //read from file.
                buffer1[512]='\0';
                if (n < 0) error("ERROR reading from file");
                n = write(newsockfd,buffer1,512);                        //write to socket.
                if (n < 0) error("ERROR writing to socket");
                bzero(buffer1,513);
                sleep(1);
            }
            close(newsockfd);                                           //close the socket.
            return 0;
        }
        int reapedpid = 1;
        while( reapedpid > 0 )
        {
            reapedpid = waitpid(-1, &status, WNOHANG);                 //reap the zombies.
        }
        close(newsockfd);


    }
    return 0;
}

