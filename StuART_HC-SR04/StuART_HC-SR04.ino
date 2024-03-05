/*
ADAPTED BY 51M30N
contact@lacon.dev
*/


#include <Wire.h>

char DEVICE_NAME[] = "StuART_HC-SR04/2";                                   ////note ici le nom du code et de la version ESP8266_$capteur-$version
String completeAddress = String("/HCSR04/") + DEVICE_NAME; 
//OSCErrorCode error;
#include "secret_ssid.h" //
#include "StuART_OSC.h" //


// INTEGRATION DES HC-SR04
#define TRIG_PIN_1 D2       // Pin pour le trigger du HC-SR04 (DIGITAL OUT)
#define ECHO_PIN_1 D1       // Pin pour l'echo du HC-SR04 (digital IN)
/* #define TRIG_PIN_2 D3       // Pin pour le trigger du HC-SR04 (DIGITAL OUT)
#define ECHO_PIN_2 D0       // Pin pour l'echo du HC-SR04 (digital IN)
#define TRIG_PIN_3 D4       // Pin pour le trigger du HC-SR04 (DIGITAL OUT)
#define ECHO_PIN_3 D5       // Pin pour l'echo du HC-SR04 (digital IN)
#define TRIG_PIN_4 D7       // Pin pour le trigger du HC-SR04 (DIGITAL OUT)
#define ECHO_PIN_4 D6       // Pin pour l'echo du HC-SR04 (digital IN) */
const int NUMBER_OF_SONARS_HCSR04 = 1;                              // NOMBRE DE CAPTEURS
int TRIG_PINS[NUMBER_OF_SONARS_HCSR04] = {TRIG_PIN_1};//, TRIG_PIN_2, TRIG_PIN_3, TRIG_PIN_4};//
int ECHO_PINS[NUMBER_OF_SONARS_HCSR04] = {ECHO_PIN_1};//, ECHO_PIN_2, ECHO_PIN_3, ECHO_PIN_4};//
float distance_cm[NUMBER_OF_SONARS_HCSR04] = {0};//, 0, 0, 0};

void setup() { ///////////////////////////////SETUP///////////////////////
// SETUP BUILTINLED
  pinMode(BUILTIN_LED, OUTPUT);  
/// SERIAL INITIALISATION
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens
    
  wificonnection();
  openOSCport();
 
// INTEGRATION D'UN HC-SR04
  pinMode(TRIG_PIN_1, OUTPUT);  // Le pin TRIG est en sortie
  /* pinMode(TRIG_PIN_2, OUTPUT);  // Le pin TRIG est en sortie
  pinMode(TRIG_PIN_3, OUTPUT);  // Le pin TRIG est en sortie
  pinMode(TRIG_PIN_4, OUTPUT);  // Le pin TRIG est en sortie */
  pinMode(ECHO_PIN_1, INPUT);   // Le pin ECHO est en entrée
  /* pinMode(ECHO_PIN_2, INPUT);   // Le pin ECHO est en entrée
  pinMode(ECHO_PIN_3, INPUT);   // Le pin ECHO est en entrée
  pinMode(ECHO_PIN_4, INPUT);   // Le pin ECHO est en entrée */
}
///////////////////////////////END SETUP///////////////////////






///////////////////////////////LOOP///////////////////////
void loop() {  
oscreceiver(); // écoute du port OSC d'entree
count_time(); // calcul du temps écoulé depuis la dernière mesure  

delay(TIME_LOOP);

SEND_HCSR04_MESURE();   // LIT LA MESURE SU SONAR ET L'ENVOIE EN OSC

} 
///////////////////////////////END LOOP///////////////////////

void SEND_HCSR04_MESURE(){  // GESTION DES SONAR
    for(int i = 0; i < 1; i++) {
                                                                            // CETTE FONCTION RETOURNE LA DISTANCE MESUREE PAR UN SONAR
  // Réinitialise le pin TRIG
    digitalWrite(TRIG_PINS[i], LOW);
    delayMicroseconds(2);
  // Active le sonar pendant 10 microsecondes
    digitalWrite(TRIG_PINS[i], HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PINS[i], LOW);
    
    int timebreak = 1000000;    // en microsecondes 30 ms ~= 3m Aller Retour
    long duration = pulseIn(ECHO_PINS[i], HIGH, timebreak);
    if (duration == 0) {
      //Serial.println("Pas de retour d'information!");
      distance_cm[i] = 10000;
    } 
    else {
      distance_cm[i] = duration * 0.0343 / 2;
    }
    }                         //FIN FONCTION RETOURNE LA DISTANCE MESUREE PAR UN SONAR               

    
  // SEND A OSC-UDP MESSAGE
    //String completeAddress = String("/HCSR04/") + DEVICE_NAME; //move int setup
    OSCMessage msg(completeAddress.c_str());
    msg.add(TIME_COUNT);
    for(int i = 0; i < NUMBER_OF_SONARS_HCSR04; i++) {
      msg.add(distance_cm[i]);
      }
    Udp.beginPacket(IP_CIBLE, OUT_PORT);
    msg.send(Udp);
    Udp.endPacket();
    msg.empty();
  }

  void oscreceiver(){                                 
  OSCBundle bundle;
  int size = Udp.parsePacket();
  if (size > 0) {
    while (size--) {
      bundle.fill(Udp.read());
    }
    if (!bundle.hasError()) {                    // ROUTE OSC
      bundle.dispatch("/led", led);
      bundle.dispatch("/time_loop", time_loop);
      bundle.dispatch("/POOL_REQUEST", PoolREQUEST);
      bundle.dispatch("/IPCIBLE", ipcible);
    } 
    else {
      // error = bundle.getError();
      Serial.println("error: ");
      //Serial.println(error); 
    }
  }
 }                                          //END RECEPTION OSC
