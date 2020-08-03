# Just mjpeg server
## Description
Simple mjpeg server by c++. Linux only.   
Just read jpeg file on memory and send jpeg by socket. Capable with web browser, openCV, VLC player and etc.  
## How to use
If you want demo, then type in terminal
```
g++ mJpegServer.cpp demo.cpp -o demo
./demo
```  
and turn on the web browser, enter `localhost:8080`. Then 1.jpg and 2.jpg will be shortly switching. 
## Use with rgb raw data 
You can also use this server to send raw rgb data video(like webcam). Dependent on libjpeg upper 8a.
```
g++ mJpegServer.cpp demo2.cpp withlibjpeg.cpp -L/usr/lib -ljpeg -o demo2
./demo2
```
turn on the web browser, enter `localhost:8080`. Then black to white video will be playing.
