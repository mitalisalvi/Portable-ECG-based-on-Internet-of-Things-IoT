/****************************************************************
M2X_CC3000_Post.ino
Post temperature data to AT&T's M2X
Shawn Hymel @ SparkFun Electronics
August 19, 2014

Manually connects to a WiFi network and an M2X stream. Reads 
temperature data from an HTU21D temperature and posts it to the
M2X stream.

Change AP_SSID, AP_PASSWORD, AP_SECURITY, FEED_ID, STREAM_NAME,
and M2X_KEY to match your WiFi and M2X parameters.

The security mode is defined by one of the following:
WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA, WLAN_SEC_WPA2

Resources:
Include SPI.h, SFE_CC3000.h, SFE_CC3000_Client.h, jsonlite.h,
M2XStreamClient.h, Wire.h, HTU21D.h

Development environment specifics:
Written in Arduino 1.0.5
Tested with Arduino UNO R3

This code is beerware; if you see me (or any other SparkFun 
employee) at the local, and you've found our code helpful, please
buy us a round!

Distributed as-is; no warranty is given.
****************************************************************/

#include <SPI.h>
#include <SFE_CC3000.h>
#include <SFE_CC3000_Client.h>
#include <jsonlite.h>
#include <M2XStreamClient.h>
#include <Wire.h>


// Parameters
#define POST_DELAY_MS   10000 // Post to stream every 10 seconds
#define DEGREE_SYMBOL   176   // Degree symbol for Serial.write

// Pins
#define CC3000_INT      3   // Needs to be an interrupt pin (D2/D3)
#define CC3000_EN       5   // Can be any digital pin
#define CC3000_CS       10  // Preferred is pin 10 on Uno

// Connection info data lengths
#define IP_ADDR_LEN     4   // Length of IP address in bytes

// WiFi Constants
#define AP_SSID         "Wi-fi"      // SSID of network
#define AP_PASSWORD     "password"  // Password of network
#define AP_SECURITY     WLAN_SEC_WPA2 // Security of network
#define TIMEOUT         30000         // Milliseconds

// M2X Constants
#define FEED_ID         "4c84c07213f02f4ed965ab503bdf36f2"
#define STREAM_NAME     "heart"
#define M2X_KEY         "e5f6ea5316d018457c96b09559ba3182"

// Global Variables
SFE_CC3000 wifi = SFE_CC3000(CC3000_INT, CC3000_EN, CC3000_CS);
SFE_CC3000_Client client = SFE_CC3000_Client(wifi);
M2XStreamClient m2x_client(&client, M2X_KEY);

#define tpin 0 //analog pin 0
int temp,g_response;

// Setup. Configure HTU21D, WiFi, and M2X.
void setup() {

  g_response = 0;

  // Initialize UART for debugging
  Serial.begin(9600);
  Serial.println();
  Serial.println(F("M2X Post"));

  

  // Initialize CC3000 (configure SPI communications)
  if ( wifi.init() ) {
    Serial.println(F("CC3000 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during CC3000 init!"));
  }

  // Connect using DHCP
  Serial.print(F("Connecting to SSID: "));
  Serial.println(AP_SSID);
  if(wifi.connect(AP_SSID, AP_SECURITY, AP_PASSWORD, TIMEOUT)) {
    Serial.println(F("Connected!"));
  } else {
    Serial.println(F("Error: Could not connect to AP"));
  }
}

// Main loop. Post sensor readings at regular intervals.
void loop() {

  // Read sensor
  temp = analogRead(tpin);

  // Print reading to console with degree symbol and 'C'
  Serial.print(F("Temperature: "));
  ;

  // Post data to your stream
  g_response = m2x_client.post(FEED_ID, STREAM_NAME, temp);

  // If we fail to receive a response, stop running
  Serial.print(F("Post response: "));
  Serial.println(g_response);
  if ( g_response == -1 ) {
    while(1);
  }

  // Wait to post to stream again
  delay(POST_DELAY_MS);
}
