#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <netinet/in.h>
#include <math.h>
#include <signal.h>

#include <iostream>
#include <string>
#include <fstream>

#define __THROW_SEND_ERROR 1
#define __THROW_SOCKET_ERROR 2
#define __THROW_SET_SOCK_OPT_ERROR 3
#define __THROW_BIND_ERROR 4
#define __THROW_LISTEN_ERROR 5
#define __THROW_ACCEPT_ERROR 6

class mJpegServer{
	private :
	int client;
	int sock;
	struct sockaddr_in addr;
	struct sockaddr_in client_addr;
	socklen_t addr_len = sizeof(addr);
	int bind_port = 8080;
	int on = 1;
	char buffer[1024] = {0,};
	size_t read_len;
	const char * header_packet = "HTTP/1.0 200 OK\r\nServer: EMPOSIII\r\n Connection: close\r\nContent-Type:multipart/x-mixed-replace; boundary=myBoundary\r\n\r\n";
	const char * mjpegHeaderPacket = "--myBoundary\r\nContent-Type: image/jpeg\r\n\r\n";
	char * jpegData;
	unsigned long jpegSize;
	std::ifstream fileJpeg;

	public:
	void cpJpegData(char * Data, unsigned long Size);
	void openJpegData(std::string fileName);

	void initSocket(int port=8080);
	void createConnetion();
	void closeConnection();
	void closeSocket();

	void sendFrame();
	
};