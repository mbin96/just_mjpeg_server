#include "withlibjpeg.h"
#include <stdio.h>
#include <stdlib.h>
#include "mJpegServer.h"
#define DEBUG

void loadRGB(std::string fileName, unsigned char * inputBuffer,int img_width, int img_height, unsigned char rgbval)
{
    // FILE *fp = fopen(fileName.c_str(), "rb");
    // fread(inputBuffer, img_width*img_height*3, 1, fp);
    // fclose(fp);
    for(int i =0; i < img_height; i++){
        for(int j =0; j < img_width*3; j++){
            inputBuffer[i * img_width *3 + j] = rgbval;
        }    
    }
}


int main(){
    int img_height = 240;
    int img_width = 320;

    u_long jpegSize = 0;
    char * imageBuffer = (char *)malloc(img_width*img_height*3);
    char * jpegBuffer[1];
    mJpegServer server;

	server.initSocket(8080);
	server.createConnetion();
    unsigned char rgbval = 0;
    while (true) 
    {  
        rgbval += 10;
        loadRGB("rgbimage1", (unsigned char *)imageBuffer, img_width, img_height, rgbval);
         jpegCompress(imageBuffer, jpegBuffer, &jpegSize, img_width, img_height);
            
        #ifdef DEBUG
        printf("compress");
        #endif //Debug
      
        try{
            server.cpJpegData(*jpegBuffer, (int)jpegSize);
            server.sendFrame();
            #ifdef DEBUG
            printf("sendFrame");
            #endif //Debug
            
		}catch(int x){
			server.closeConnection();
#ifdef DEBUG
			std::cout<<"connection out"<< std::endl;
#endif //DEBUG
			server.createConnetion();
		}
        usleep(30000);
        // }else{
        //     sendFrame(*camCloseImg, camCloseImgSize);
        //     
        // }
        
        
    }

    server.closeConnection();
    server.closeSocket();
    free(imageBuffer);
    free(jpegBuffer[0]);
}