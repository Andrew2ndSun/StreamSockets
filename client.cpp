/*
Name: Quang Tran
PSID: 1800830
Assignment #2
*/
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <string>
#include <iostream>
using namespace std;

int main()
{
    string cityname;
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent* server;
    string hostname;
    const char* lhost = hostname.data();
    char buffer[256];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        cerr << "ERROR opening socket" << endl;
    }
    cout << "Enter the server host name: ";
    cin >> hostname;
    server = gethostbyname(lhost);
    if (server == NULL || hostname != "localhost")
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    cout << "Enter the server port number: ";
    cin >> portno;

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*)server->h_addr,
        (char*)&serv_addr.sin_addr.s_addr,
        server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr*) & serv_addr, sizeof(serv_addr)) < 0)
    {
        cerr << "ERROR connecting" << endl;
        exit(0);
    }

    cout << "Enter a city name: ";
    cin.ignore(); getline(cin, cityname); //take city input, ignore the enter
    bzero(buffer, 256);

    //transfer city name into buffer
    const char* chost = cityname.data();
    strcpy(buffer, chost);

    n = write(sockfd, buffer, strlen(buffer)); //write the buffer to socket
    if (n < 0)
        cerr << "ERROR writing to socket";
    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    if (n < 0)
        cerr << "ERROR reading from socket";
    printf("%s", buffer);
    return 0;
}
