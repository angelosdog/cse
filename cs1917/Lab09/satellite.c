/*
 * BMP server to return 3D tiels which can be viewed using 3D glasses
 */

#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>


int waitForConnection (int serverSocket);
int makeServerSocket (int portno);
void serveBMP (int socket, double centreX, double centreY, int zoom);
static void serveHTML (int socket);

#define SIMPLE_SERVER_VERSION 1.0
#define REQUEST_BUFFER_SIZE 1000
#define DEFAULT_PORT 1990
#define NUMBER_OF_PAGES_TO_SERVE 2140000000
// after serving this many pages the server will halt
// apparently this is the largest number a int-32 can be
#define MAX_ITERATIONS 256
#define URL_REQUEST_INDEX 5
#define ASCII_FOR_X 88
#define BMP_SIZE 786432 //512 pixels x 512 pixels x 3 bytes per colour
#define HEADER_SIZE 54

int main (int argc, char *argv[]) {
   printf ("************************************\n");
   printf ("Starting simple server %f\n", SIMPLE_SERVER_VERSION);
   printf ("Serving bmps since 2012\n");   
   
   int serverSocket = makeServerSocket (DEFAULT_PORT);   
   printf ("Access this server at http://localhost:%d/\n", DEFAULT_PORT);
   printf ("************************************\n");
   
   char request[REQUEST_BUFFER_SIZE];
   
   int numberServed = 0;
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
      printf (" *** Received http request ***\n %s\n", request);
     
      int centreX = 0;
      int centreY = 0;
      int zoom = 0;
      // read from HTTP GET request
      // if specific tile is request, initiate bmp server
      if (request[URL_REQUEST_INDEX] == ASCII_FOR_X){
         sscanf(request, "GET /X-%d.5-%d.5-%d", &centreX, &centreY, &zoom);
         printf (" *** Sending http response ***\n");
         serveBMP(connectionSocket,centreX,centreY,zoom);
      } else{
      // otherwise use the interactive mandelbrot browser
         serveHTML(connectionSocket);
      }
      
      // close the connection after sending the page
      close(connectionSocket);
      
      numberServed++;
   } 
   
   // close the server connection after we are done
   printf ("** shutting down the server **\n");
   close (serverSocket);
   
   return EXIT_SUCCESS; 
}
static void serveHTML (int socket){
   char* message;

   // first send the http response header
   message =
      "HTTP/1.0 200 Found\n"
      "Content-Type: text/html\n"
      "\n";
   printf ("about to send=> %s\n", message);
   write (socket, message, strlen (message));

   message = "<html><script src=\"https://openlearning.cse.unsw.edu.au/site_media/viewer/tile.min.js\"></script></html>";
//   serveBMP(socket,centreX,centreY,zoom);
   write (socket, message, strlen (message));
   
}

void serveBMP (int socket, double centreX, double centreY, int zoom) {
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
   unsigned char bmp[BMP_SIZE + HEADER_SIZE]

   fscanf("/home/mirror/pub/openlearning/satellite/BlueMountains/%d,%dTexture.bmp",bmp,centreX, centreY); 

   // the web server prints out the 
   write (socket, bmp, sizeof(bmp));
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
   
   serverAddress.sin_family      = AF_INET;
   serverAddress.sin_addr.s_addr = INADDR_ANY;
   serverAddress.sin_port        = htons (portNumber);
   
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
int waitForConnection (int serverSocket){ 
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
