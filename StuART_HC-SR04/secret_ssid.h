#include <ESP8266WiFiMulti.h> //#include <ESP8266WiFi.h>
ESP8266WiFiMulti wifiMulti;

// WiFi connect timeout per AP. Increase when connecting takes longer.
const uint32_t connectTimeoutMs = 5000;

// VARIABLES RESEAU
char IP_CIBLE[] = "255.255.255.255";
char MY_IP[16];

int OUT_PORT = 8889;            // local port to send
unsigned int localPort = 8888;  // local port to listen


void wificonnection(){
/// WIFI CONNECTION  
  WiFi.persistent(true);                              // Save WiFi configuration in flash - optional
  Serial.println("SETUP WIFI CONNECTION");            // Set WiFi to station modes
  wifiMulti.addAP("ssid_from_AP_1", "your_password_for_AP_1");      //AP-1
  wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2");      //AP-2
// Maintain WiFi connection
  while (wifiMulti.run(connectTimeoutMs) != WL_CONNECTED) {
    Serial.println("WAITING CONNECTION");
    delay(1000);
    }
    Serial.print("WiFi connected: ");
    Serial.print(WiFi.SSID());
    Serial.print(" ");
    Serial.println(WiFi.localIP());
    IPAddress ip = WiFi.localIP();
    snprintf(MY_IP, sizeof(MY_IP), "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]); // PRINT L'ADRESSE IP SUR COM
}
