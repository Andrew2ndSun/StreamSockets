/*
Name: Quang Tran
PSID: 1800830
Assignment #2
*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <fstream>
#include <vector>
#include <iostream>

using namespace std;

int main()
{
    vector <string> ans;
    char buffer[256];
    ifstream wfile;
    string line;
    int sockfd, newsockfd, portno, clilen;

    //read weather file
    wfile.open("weather.txt");
    while (getline(wfile, line))
    {
        ans.push_back(line);
    }
    wfile.close();

    cout << "Please enter portnumber: ";
    cin >> portno; cin.ignore(); //take portnumber input but ignore the enter

    struct sockaddr_in serv_addr, cli_addr;
    int n;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
        cerr << "ERROR opening socket";

    bzero((char*)&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr*) & serv_addr,
        sizeof(serv_addr)) < 0)
        cerr << "ERROR on binding";
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    while (1)    //keep server running for same portnumber
    {
        newsockfd = accept(sockfd, (struct sockaddr*) & cli_addr, (socklen_t*)&clilen);
        if (newsockfd < 0)
        {
            cerr << "ERROR on accept";
        }
        bzero(buffer, 256);
        n = read(newsockfd, buffer, 255); //read info from socket

        if (n < 0) { cerr << "ERROR reading from socket"; }
        cout << "Weather report for " << buffer << endl;
        string clires; //take all outputs into 1 string
        bool found = false;
        for (int i = 0; i < ans.size(); i++)
        {
            string city = ans[i].substr(0, ans[i].find(','));
            if (buffer == city)
            {
                found = true;
                string sub = ans[i].substr(ans[i].find(',') + 1);
                cout << "Tomorrow’s maximum temperature is " << sub.substr(0, sub.find(',')) << endl;
                cout << "Tomorrow’s sky condition is " << sub.substr(sub.find(',') + 1) << endl;
                clires = "Tomorrow’s maximum temperature is " + sub.substr(0, sub.find(',')) + " F\n" +
                    "Tomorrow’s sky condition is " + sub.substr(sub.find(',') + 1) + "\n";
                break;
            }
        }
        if (found == false)
        {
            cout << "No data" << endl;
            clires = "No data\n";
        }

        n = write(newsockfd, clires.c_str(), 256); //write string into newsocket

        if (n < 0) cerr << "ERROR writing to socket";
        close(newsockfd);
    }
    return 0;
}