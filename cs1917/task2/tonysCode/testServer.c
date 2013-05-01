    /*
    * bmpServer.c
    * 1917 serve that 3x3 bmp from lab3 Image activity
    *
    * Created by Tim Lambert on 02/04/12.
    * Containing code created by Richard Buckland on 28/01/11.
    * Copyright 2012 Licensed under Creative Commons SA-BY-NC 3.0.
    *
    */

#define SIMPLE_SERVER_VERSION 1.0
#define REQUEST_BUFFER_SIZE 1000
#define DEFAULT_PORT 1919
#define NUMBER_OF_PAGES_TO_SERVE 10000

#define BMP_HEADER_SIZE 54

#define ITER 1020

#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

int waitForConnection (int serverSocket);
int makeServerSocket (int portno);
void serveBMP (int socket, double startX, double startY, int zoom1);
int mandelbrot(double xC,double yC);

    // after serving this many pages the server will halt

int main (int argc, char *argv[]) {

    printf ("************************************\n");
    printf ("Starting simple server %f\n", SIMPLE_SERVER_VERSION);
    printf ("Serving bmps since 2012\n");

    int serverSocket = makeServerSocket (DEFAULT_PORT);

    printf ("Access this server at http://localhost:%d/\n", DEFAULT_PORT);
    printf ("************************************\n");

    char request[REQUEST_BUFFER_SIZE];
    int numberServed = 0;
    double x = 0.0;
    double y = 0.0;
    int z = 1;

    while (numberServed < NUMBER_OF_PAGES_TO_SERVE) {
         printf ("*** So far served %d pages ***\n", numberServed);
         int connectionSocket = waitForConnection (serverSocket);
    // wait for a request to be sent from a web browser, open a new
    // connection for this conversation
    // read the first line of the request sent by the browser
        int bytesRead;
        bytesRead = read (connectionSocket, request, (sizeof request)-1);
        assert (bytesRead >= 0);
    // were we able to read any data from the connection?
    // print entire request to the console
        sscanf(request,"GET /X-%lf-%lf-%d",&x,&y,&z);
        printf (" *** Received http request ***\n %s\n", request);
    //send the browser a simple html page using http
        printf (" *** Sending http response ***\n");
        serveBMP(connectionSocket,x,y,z);
      // close the connection after sending the page- keep aust beautiful
      close(connectionSocket);

    printf("1:%lf 2:%lf 3:%d\n",x,y,z);

      numberServed++;
   }

    // close the server connection after we are done- keep aust beautiful
    printf ("** shutting down the server **\n");
    close (serverSocket);
    return EXIT_SUCCESS;
}

void serveBMP (int socket, double startX, double startY, int zoom1) {

    double zoom = zoom1;
    double x,y;
    char* message;
    // first send the http response header
    // (if you write stings one after another like this on separate
    // lines the c compiler kindly joins them togther for you into
    // one long string)
    message = "HTTP/1.0 200 OK\r\n"
    "Content-Type: image/bmp\r\n"
    "\r\n";
    printf ("about to send=> %s\n", message);
    write (socket, message, strlen (message));
    // now send the BMP
    //unsigned char bmp[262198];
    unsigned char header[] = {
     0x42,0x4d,0x36,0x00,0x04,0x00,0x00,0x00,
     0x00,0x00,0x36,0x00,0x00,0x00,0x28,0x00,
     0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x02,
     0x00,0x00,0x01,0x00,0x18,0x00,0x00,0x00,
     0x00,0x00,0x24,0x00,0x00,0x00,0x13,0x0B,
     0x00,0x00,0x13,0x0B,0x00,0x00,0x00,0x00,
     0x00,0x00,0x00,0x00,0x00,0x00};
    unsigned char hex[] = {
     0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
     0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
     0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,
     0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
     0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,
     0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
     0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,
     0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
     0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,
     0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
     0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,
     0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,
     0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,
     0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
     0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,
     0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x7F,
     0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,
     0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
     0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,
     0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,
     0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,
     0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
     0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,
     0xB8,0xB9,0xBA,0xBB,0xBC,0XBD,0xBE,0xBF,
     0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,
     0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,
     0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,
     0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,
     0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,
     0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,
     0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,
     0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF};
    unsigned char colour[] = {0x00,0x00,0x00,};
//    write (socket, bmp, sizeof(bmp));
    write (socket, header, BMP_HEADER_SIZE);

    for (y=(startY-2.00/zoom); y<(startY+2.00/zoom); y+=1/(128*zoom)) {
        for (x=(startX-2.00/zoom); x<(startX+2.00/zoom); x+=1/(128*zoom)) {
            if (mandelbrot(x,y)==ITER) {
                colour[0] = 0x00;
                colour[1] = 0x00;
                colour[2] = 0x00;
            } else {
                colour[0] = hex[mandelbrot(x,y)/2];
                colour[1] = hex[mandelbrot(x,y)/2];
                colour[2] = hex[mandelbrot(x,y)];
            }
            write (socket, colour, sizeof(colour));
        }
    }
}

    // start the server listening on the specified port number
int makeServerSocket (int portNumber) {
    // create socket
    int serverSocket = socket (AF_INET, SOCK_STREAM, 0);
    assert (serverSocket >= 0);
    // error opening socket
    // bind socket to listening port
    struct sockaddr_in serverAddress;
    bzero ((char *) &serverAddress, sizeof (serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons (portNumber);
    // let the server start immediately after a previous shutdown
    int optionValue = 1;
    setsockopt (
    serverSocket,
    SOL_SOCKET,
    SO_REUSEADDR,
    &optionValue,
    sizeof(int)
    );

    int bindSuccess =
    bind (
    serverSocket,
    (struct sockaddr *) &serverAddress,
    sizeof (serverAddress)
    );
    assert (bindSuccess >= 0);
    // if this assert fails wait a short while to let the operating
    // system clear the port before trying again
    return serverSocket;
}

    // wait for a browser to request a connection,
    // returns the socket on which the conversation will take place
int waitForConnection (int serverSocket) {
   // listen for a connection
   const int serverMaxBacklog = 10;
   listen (serverSocket, serverMaxBacklog);
   
   // accept the connection
   struct sockaddr_in clientAddress;
   socklen_t clientLen = sizeof (clientAddress);
   int connectionSocket = 
      accept (
         serverSocket, 
         (struct sockaddr *) &clientAddress, 
         &clientLen
      );
   
   assert (connectionSocket >= 0);
   // error on accept
   
   return (connectionSocket);
}

int mandelbrot(double xC,double yC) {

    double x = 0;
    double y = 0;
    double tempX=0;
    double tempY=0;
    int iterations=0;

    while((x<2) && (y<2) && (iterations<ITER)){
        tempX = x;
        tempY = y;

        x = tempX*tempX - tempY*tempY + xC;
        y = 2*tempX*tempY + yC;

        iterations++;
    }

    return iterations;
}

char hexOne(int dec) {
  
    char conv[]= {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    char first = conv[dec/16];    

    return first;
}

char hexTwo(int dec) {
  
    char conv[]= {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    char second = conv[dec%16];    

    return second;
}
