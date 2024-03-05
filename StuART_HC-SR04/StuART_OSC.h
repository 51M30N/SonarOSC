#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
WiFiUDP Udp;
/// DEFINE BUILTIN LED TEST CONTROL  
unsigned int ledState = LOW;              // LOW means led is *on*
#define BUILTIN_LED LED_BUILTIN           // LED de control



// VARIABLES TIME
int TIME_LOOP = 50;             // delay default de la boucle loop
int TIME_COUNT = 0;             // temp mesure entre chaque envoi
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

void openOSCport(){//////// SETUP UDP LISTEN
  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port listen : ");
/* #ifdef ESP32
  Serial.println(localPort);
#else */
  Serial.println(Udp.localPort());
/* #endif */
}

// FONCTION POOL REQUEST
void PoolREQUEST(OSCMessage &msg){ 
//Serial.println("Pool_Request"); 
    if (msg.getInt(0) == 1){
    OSCMessage msg("/Pool_REQUEST");
      msg.add(DEVICE_NAME);
      msg.add("IN");
      msg.add(MY_IP);
      msg.add(localPort);
      msg.add("OUT");
      msg.add(IP_CIBLE);
      msg.add(OUT_PORT);
        Udp.beginPacket(IP_CIBLE, OUT_PORT);
        msg.send(Udp);
        Udp.endPacket();
      msg.empty();
    }
}

void led(OSCMessage &msg) {         // control de la Led BUILTIN pour test CONNECTION
  ledState = msg.getInt(0);
  digitalWrite(BUILTIN_LED, ledState);
  Serial.print("/led: ");
  Serial.println(ledState);
}

void time_loop(OSCMessage &msg) {     //REDEFINIT LA DUREE DE LA BOUCLE LOOP
  TIME_LOOP = msg.getInt(0);
  Serial.print("/time_loop: ");
  Serial.println(TIME_LOOP);
}

void count_time(){ //compteur d'interval de temps REEL DE LOOP
  currentMillis = millis();
  TIME_COUNT = currentMillis - previousMillis; // temp mesur� entre chaque envoi
  previousMillis = currentMillis;
}

void ipcible(OSCMessage &msg) {     //REDEFINIT LA cible des messages OSC
  char ipcible[16];
  for(int i = 0; i < 4 ; i++){ipcible[i]=msg.getInt(i);}
  snprintf(IP_CIBLE, sizeof(IP_CIBLE), "%d.%d.%d.%d", ipcible[0], ipcible[1], ipcible[2], ipcible[3]); // 
}
