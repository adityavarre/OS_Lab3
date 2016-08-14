#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <strings.h>
#include <stdlib.h>
#include <pthread.h>
#include <vector>
#include <signal.h>
#include <time.h>
using namespace std;
int bytecount;
void error(string msg)
{
    cout<<msg<<endl;
    return ;
}
void signalHandler( int signum )
{
    cout << "Interrupt signal (" << signum << ") received.\n";
    printf("downloaded %d number of bytes so far ", bytecount);
    // cleanup and close up stuff here  
    // terminate program  

   exit(signum);  

}

int main(int argc, char *argv[])
{
	signal(SIGINT, signalHandler);
	if(argc != 5)
	{
		fprintf(stderr,"usage %s filename ip-address port display/nodisplay ", argv[0]);
		return 1;
	}
	int glo_port;
	bool display = 0;
	struct hostent *glo_server;
	glo_port = atoi(argv[3]);
    glo_server = gethostbyname(argv[2]);
    if(argv[4] == "display")
    {
    	display = 1;
    }
    int portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[513];
    portno = glo_port;
    /* fill in server address in sockaddr_in datastructure */    
    server = glo_server;
    if (server == NULL)
    {
        fprintf(stderr,"ERROR, no such host\n");
        return -1;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    	bytecount  = 0;
        int sockfd;
        string s(argv[1]);
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
            error("ERROR connecting");
        int yes=1;
        if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1)
        {
            cout<<"error at setsockopt"<<endl;
            close(sockfd);
        }
        cout<<sockfd<<endl;
        bzero(buffer,512);
        s = "get " + s;
        cout<<s<<endl;
        n = write(sockfd,s.c_str(),s.length()); 
        if (n < 0)
        error("ERROR writing to socket");
        int byte ;
    	while((byte = read(sockfd,buffer,512) )> 0)
        {
            bytecount += byte;
            if (display)
            {
              printf("%s",buffer);
            }
        }
        cout<<"vachesindhi"<<endl;

}