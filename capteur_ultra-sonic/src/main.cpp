#include <Arduino.h>

#define TRIG_PIN 12
#define ECHO_PIN 11

// Définition des broches GPIO pour l'ESP32
const int trigPin = 5;
const int echoPin = 18;

void setup() {
  // Initialisation du moniteur série (aligné sur platformio.ini)
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
    // Impulsion de 10 microsecondes pour déclencher le capteur
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // pulseIn() renvoie la durée de l'impulsion en microsecondes
  long duration = pulseIn(echoPin, HIGH);

  // Calcul de la distance en cm (Vitesse du son ~ 0.0343 cm/µs)
  float distance = duration * 0.0343 / 2;

  // Diagnostic
  if (duration == 0) {
    Serial.println("Erreur : Aucun écho reçu (Vérifiez le câblage ou le pont diviseur).");
  } else if (distance > 400) {
    Serial.println("Hors de portée (> 400 cm).");
  } else {
    Serial.print("Distance : ");
    Serial.print(distance);
    Serial.println(" cm");
  }

  delay(1000); // Pause entre deux mesures
}
