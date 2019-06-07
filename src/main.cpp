#include <Arduino.h>

#include "Nextion.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 7 //Broche 7 connexion au capteur de température
#define POMPE 6 //Broche 6 connexion à la gate du mosfet
#define NSECONDES 14000 //Nombres de millisccondes pour remplir un verre d'eau

OneWire oneWire(ONE_WIRE_BUS); //Bus One Wire sur la pin 2 de l'arduino
DallasTemperature sensors(&oneWire); //Utilistion du bus Onewire pour les capteurs
char temperatureCString[7]; //Buffer de stockage de la valeure de température

DeviceAddress sensorDeviceAddress; //Vérifie la compatibilité des capteurs avec la librairie

unsigned long previousMillis = 0; //Millis pour l'actualisation de température dans la fonction loop()


NexButton bWater = NexButton(0, 3, "bWater"); //Bouton de demande d'un verre d'eau
NexText tTemp = NexText(0, 4, "tTemp"); //Zone de texte qui affiche la température de l'eau en temps réel

/*
 * Liste des évenements à écouter provenant de l'écran Nextion
 */
NexTouch *nex_listen_list[] = { 
  &bWater,
  NULL
};
 
/*
 * Fonction appelé lorsque l'utilisateur appuie sur le verre d'eau
 */
void bOnPopCallback(void *ptr) {
  digitalWrite(POMPE,HIGH); //Allume la pompe
  delay(NSECONDES); //Attend Nsecondes
  digitalWrite(POMPE,LOW); //Eteind la pompe
  
}


void setup(void) {    
  Serial.begin(9600);
  pinMode(POMPE, OUTPUT);
  digitalWrite(POMPE,LOW);


  nexInit(); //Initialisation des objets Nextion
  bWater.attachPop(bOnPopCallback, &bWater); //Appel fonction dans le cas ou l'utilisateur appuie sur le bouton
  sensors.begin(); //Activation des capteurs
    
}

void loop(void) {   
  nexLoop(nex_listen_list);//Loop des callback nextion

unsigned long currentMillis = millis(); //Place une millis pour décompter depuis combien de temps la boucle loop est lancé
  if (currentMillis - previousMillis >= 1000){ //Si cela fait 1 seconde
    
    sensors.requestTemperatures(); //Demande la température aux capteurs
    float tempC = sensors.getTempCByIndex(0); //Stock le float de la température

    //Tranforme le float de la température en string
    char temperature[3];
    dtostrf(tempC, 3,1, temperature);

    //Un court message à afficher sur l'écran
    char msg[20];
    sprintf(msg, "%sC ", temperature);


    //Envoie message
    tTemp.setText(msg);

  previousMillis = millis();
  }
}