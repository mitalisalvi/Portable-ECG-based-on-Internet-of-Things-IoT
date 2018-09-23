#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <plotly_streaming_cc3000.h>

#define WLAN_SSID       "wifi-name"
#define WLAN_PASS       "password"
#define WLAN_SECURITY   WLAN_SEC_WPA2

// Sign up to plotly here: https://plot.ly
// View your API key and streamtokens here: https://plot.ly/settings
#define nTraces 3
// View your tokens here: https://plot.ly/settings
// Supply as many tokens as data traces
// e.g. if you want to ploty A0 and A1 vs time, supply two tokens
char *tokens[nTraces] = {"fawkojc1a7"};
// arguments: username, api key, streaming token, filename
plotly graph = plotly("sagarsam", "f2ni5ozmpx", tokens, "filename", nTraces);

int tpin=0;


void wifi_connect(){
  /* Initialise the module */
  Serial.println(F("\n... Initializing..."));
  if (!graph.cc3000.begin())
  {
    Serial.println(F("... Couldn't begin()! Check your wiring?"));
    while(1);
  }

  // Optional SSID scan
  // listSSIDResults();

  if (!graph.cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("Failed!"));
    while(1);
  }

  Serial.println(F("... Connected!"));

  /* Wait for DHCP to complete */
  Serial.println(F("... Request DHCP"));
  while (!graph.cc3000.checkDHCP())
  {
    delay(100); // ToDo: Insert a DHCP timeout!
  }
}


void setup() {
  graph.maxpoints = 100;

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // Initialize pinMode for UV Sensor

 

  wifi_connect();

  graph.fileopt="overwrite"; // See the "Usage" section in https://github.com/plotly/arduino-api for details
  bool success;
  success = graph.init();
  if(!success){while(true){}}
  graph.openStream();
}

void loop() {

  int uvLevel =analogRead(tpin);
  
 
  Serial.print("heart beat \n");
  Serial.print(uvLevel);
  graph.plot(millis(), uvLevel, tokens[0]);
  delay(500);

 

}

