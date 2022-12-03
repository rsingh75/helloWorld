#include<iostream>
#include<string.h>
#include<cstring>
#include<unistd.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>

#define PORT 4444

using namespace std;
int main( int argc, char const* argv[] )
{

	int sock = 0, client_fd;

	struct sockaddr_in serv_addr;

	char buffer[1024] = {0};


	if(( sock = socket( AF_INET, SOCK_STREAM,0)) < 0)
	{
		cout << "Socket creation failed" << endl;
		exit(EXIT_FAILURE);
	}

	cout << "Socket creation successful" << endl;

	memset(&serv_addr, '\0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");


	if( inet_pton( AF_INET, "127.0.0.1", &serv_addr.sin_addr) <=0 )
	{
		cout << "Invalid address" << endl;
		exit(EXIT_FAILURE);
	}

	if( ( client_fd = connect( sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0)
	{
		cout << "Connection failed" << endl;
		exit(EXIT_FAILURE);
	}

	cout << "Connected to server" << endl;

	cout << "Start typing your query, type exit/EXIT to finish the conversation" << endl;
	while(1)
	{
		cout << "Client's message" << endl;
		bzero(buffer, sizeof(buffer));

		cin.getline(buffer,1024);

		send(sock, buffer, strlen(buffer), 0);

		if( strncmp(buffer,"exit", strlen(buffer)) == 0 || strncmp(buffer,"EXIT", strlen(buffer)) == 0 )
		{
			close(client_fd);
			cout << "Disconnected from server" << endl;
			exit(1);
		}

		bzero(buffer, sizeof(buffer));
		if (recv(sock, buffer, 1024, 0) < 0)
		{
			cout << "Error in receiving the data from server" << endl;
		}
		else
		{
			cout << "Buffer=" << buffer << endl;
		}

	}

	return 0;
}
