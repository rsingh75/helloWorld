#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<cstring>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define PORT 4444
#define _BSD_SOURCE

using namespace std;
int main( int argc, char const* argv[] )
{

	int ser_fd;
	int new_socket;

	struct sockaddr_in address;
	struct sockaddr_in newaddr;

	int opt =1;

	int addrlen = sizeof(newaddr);

	char buffer[1024] = {0};

	pid_t childpid;


	if(( ser_fd = socket( AF_INET, SOCK_STREAM,0)) < 0)
	{
		cout << "Socket creation failed" << endl;
		exit(EXIT_FAILURE);
	}

	cout << "Socket creation successful" << endl;
	if( setsockopt( ser_fd, SOL_SOCKET,
	    SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		cout << "Reusability of IP and port failed" << endl;
		exit(EXIT_FAILURE);
	}
	
	memset(&address, '\0', sizeof(address));
	address.sin_family = AF_INET;
	//address.sin_addr.s_addr = INADDR_ANY;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(PORT);

	if( bind( ser_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
	{
		cout << "Bind failed" << endl;
		exit(EXIT_FAILURE);
	}

	cout << "Bind to specific port successful" << endl;

	if(listen(ser_fd,2) < 0)

	{
		cout << "LIsten failed" << endl;
		exit(EXIT_FAILURE);
	}

	while(1)
	{
		new_socket = accept ( ser_fd, (struct sockaddr*)&newaddr, (socklen_t*)&addrlen);
		if(new_socket <0)
		{
			cout << "Could not accept connection" << endl;
			exit(EXIT_FAILURE);
		}
		cout << "Coonection accepted from new client, IP address:port number " << inet_ntoa(newaddr.sin_addr) <<":"<< newaddr.sin_port << endl;

		if(( childpid = fork() ) == 0 )
		{
			close(ser_fd);

			while(1)
			{
				recv(new_socket, buffer, 1024, 0);
				if(  strncmp(buffer, "exit",sizeof(buffer)) == 0 || strncmp(buffer,"EXIT",sizeof(buffer)) == 0 )
				{
					cout << "Discoonceted from client IP address: port number "<< inet_ntoa(newaddr.sin_addr) <<":"<< newaddr.sin_port << endl;
					break;
				}
				else
				{
					cout << "Client's message to server: " << buffer << endl;
					cout << "Server Reply " << endl;
					bzero(buffer, sizeof(buffer));
					cin.getline(buffer,1024);
					send(new_socket, buffer, strlen(buffer), 0);
					bzero(buffer, sizeof(buffer));
				}
			}
		}
	}
	close(new_socket);
	return 0;
}
