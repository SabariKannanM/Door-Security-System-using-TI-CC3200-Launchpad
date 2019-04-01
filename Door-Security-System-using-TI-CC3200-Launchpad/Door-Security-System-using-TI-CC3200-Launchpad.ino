/*
  Title 		    : Door-Security-System-using-TI-CC3200-Launchpad
  Programmed by	: Sabari Kannan M
  Place			    : Coimbatore, Tamil Nadu, India.
  Created on	  : 28 March 2019
  Last edit 	  : 01 April 2019
*/

#include <Wire.h>
#include <BMA222.h>
#include <WiFi.h>

// your network name also called SSID
char ssid[] = "xxx";      //replace "xxx" with your SSID
// your network password
char password[] = "xxx";  //replace "xxx" with your password
char server[] = "maker.ifttt.com";
#define URL "GET xxx HTTP/1.1"  // replace "xxx" with the URL generated on the IFTTT website starting from "/trigger/.../.../..."
                                // eg "GET /trigger/TICC3200/with/key/abcdefghijklmnopqrstuvwxyz HTTP/1.1"
WiFiClient client;

uint8_t oldX, oldY, oldZ, newX, newY, newZ;
uint8_t sensitivity = 30;
BMA222 mySensor;

void setup()
{
  //begins the communication between CC3200 ans BMA222 accelerometer sensor
  mySensor.begin();

  //connects to the network
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED);
  while (WiFi.localIP() == INADDR_NONE);

  delay(60000);
  //read X,Y,Z sensor values
  oldX = mySensor.readXData();
  oldY = mySensor.readYData();
  oldZ = mySensor.readZData();
}

void loop()
{
  //read X,Y,Z sensor values
  newX = mySensor.readXData();
  newY = mySensor.readYData();
  newZ = mySensor.readZData();

  //check for movement of the board
  if ((newX - oldX > sensitivity) || (newX - oldX < (sensitivity * -1)) ||
      (newY - oldY > sensitivity) || (newY - oldY < (sensitivity * -1)) ||
      (newZ - oldZ > sensitivity) || (newZ - oldZ < (sensitivity * -1))) {

    //trigger the URL
    if (client.connect(server, 80)) {
      // Make a HTTP request:
      client.println(URL);
      client.println("Host: maker.ifttt.com");
      client.println("Connection: close");
      client.println();
    }
    while (true);
  }

  delay(100);
}
