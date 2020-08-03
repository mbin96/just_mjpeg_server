#include "mJpegServer.h"

//https://tjcplpllog.blogspot.com/2015/12/c-http.html
//http://bbs.nicklib.com/application/1539
//https://stackoverflow.com/questions/47729941/mjpeg-over-http-specification


void mJpegServer::cpJpegData(char * Data, unsigned long Size){
	delete[] jpegData;
	jpegSize = Size;
	jpegData = new char[jpegSize];
	memcpy(jpegData,Data,jpegSize);
}

void mJpegServer::openJpegData(std::string fileName){
	delete[] jpegData;
	fileJpeg.close();


	fileJpeg.open(fileName, std::ios::binary);
	if (!fileJpeg.is_open()){
		// throw std::__throw_ios_failure(fileName.c_str());
	}
	
	fileJpeg.seekg(0, std::ios::end);
	jpegSize = fileJpeg.tellg();
	fileJpeg.seekg(0, std::ios::beg);

	jpegData = new char[jpegSize];
	fileJpeg.read(jpegData,jpegSize);

}

void mJpegServer::createConnetion(){

	client = accept(sock, (struct sockaddr*)&client_addr, &addr_len);
	if (client < 0) {
		perror("accept() error");
		throw __THROW_ACCEPT_ERROR;
	}
	//응답 받아오기
	read_len = recv(client, buffer, sizeof(buffer), 0);
	
	#ifdef DEBUG
	if (read_len > 0) {
		printf("RECV: %s", buffer);
	}
	#endif //DEBUG
	//헤더 패킷
	send(client, header_packet, strlen(header_packet), 0);
}

void mJpegServer::closeConnection(){
	close(client);
#ifdef DEBUG
	printf("closeSocket\n");
#endif //DEBUG

}
void mJpegServer::closeSocket(){
	close(sock);
#ifdef DEBUG
	printf("closeSocket\n");
#endif //DEBUG

}

void mJpegServer::initSocket(int port){
	//매개변수로 포트 설정
	bind_port = port;
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock < 0) {
		perror("socket() error");
	 	throw __THROW_SOCKET_ERROR;
	}
	
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void*)&on, sizeof(on)) != 0) {
		perror("setsockopt() error");
		throw __THROW_SET_SOCK_OPT_ERROR;
	}

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(bind_port);
	if (bind(sock, (struct sockaddr*)&addr, addr_len) != 0) {
		perror("bind() error");
		throw __THROW_BIND_ERROR;
	}

	if (listen(sock, 5) != 0) {
		perror("listen() error");
		throw __THROW_LISTEN_ERROR;
	}
	printf("listening... %d\n", bind_port);
}
		
	


void mJpegServer::sendFrame(){
	int err;

	err = send(client, mjpegHeaderPacket, strlen(mjpegHeaderPacket), MSG_NOSIGNAL);
	if (err < 0){
		throw __THROW_SEND_ERROR;
	}

	err = send(client, jpegData, jpegSize, MSG_NOSIGNAL);
	if (err < 0){
		throw __THROW_SEND_ERROR;
	}

	err = send(client, "\r\n", strlen("\r\n"), MSG_NOSIGNAL);
	if (err < 0){
		throw __THROW_SEND_ERROR;
	}
	
}


