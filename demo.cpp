
#include <iostream>
#include <string>
#include <fstream>
#include "mJpegServer.h"

int main(int argc, char *args[]) {

	mJpegServer server;
	server.initSocket(8080);
	server.createConnetion();

    int count = 1;
	while(1){
		usleep(300000);
		server.openJpegData(std::to_string(count)+".jpg");
		count++;
		if(count == 3){
			count = 1;
		}
		try{
			server.sendFrame();
		}catch(int x){
			server.closeConnection();
#ifdef DEBUG
			std::cout<<"connection out"<< std::endl;
#endif //DEBUG
			server.createConnetion();
		}
	}
	server.closeSocket();
	//정상적인 경우 여기로 오면 안됨.
	printf("quit mjpeg server");
    return 0;
}