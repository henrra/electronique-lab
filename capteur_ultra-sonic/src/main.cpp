#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Configuration de l'écran OLED (Taille standard 128x64)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Définition des broches GPIO pour l'ESP32
const int trigPin = 5;
const int echoPin = 18;
const int buzzerPin = 25; // Broche D25 pour le buzzer

// Seuils de distance (en cm)
const float SEUIL_DANGER = 10.0;  // Son continu
const float SEUIL_ALERTE = 30.0;  // Bips intermittents

void setup()
{
  // Initialisation du moniteur série (aligné sur platformio.ini)
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  digitalWrite(buzzerPin, LOW); // Éteindre le buzzer au démarrage

  // Initialisation de l'écran OLED à l'adresse 0x3C (adresse standard)
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("Erreur : Écran OLED non détecté !"));
    for (;;)
      ; // Bloquer le programme si l'écran n'est pas trouvé
  }

  // Message d'accueil sur l'OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 20);
  display.println("Demarrage...");
  display.display();
  delay(1500);
}

void loop()
{
  // Impulsion ultrason
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.0343 / 2;

  // Effacer l'écran avant d'écrire la nouvelle valeur
  display.clearDisplay();

  // Titre en haut de l'écran
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("MESURE DISTANCE");
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE); // Petite ligne de séparation

    // Variables pour gérer le comportement du buzzer dans ce cycle
  int tempsAttenteBuzzer = 0;

  // Affichage du résultat
  if (duration == 0 || distance > 400)
  {
    display.setTextSize(2);
    display.setCursor(10, 30);
    display.print(distance, 1);
    display.println(" Erreur / HW");

    digitalWrite(buzzerPin, LOW); // Pas de son en cas d'erreur
    tempsAttenteBuzzer = 300;     // Pause standard de rafraîchissement
  }
  else
  {
    // Affichage de la distance en gros au milieu
    display.setTextSize(2);
    display.setCursor(10, 30);
    display.print(distance, 1); // 1 chiffre après la virgule
    display.println(" cm");

    // --- LOGIQUE ENRICHIE DU BUZZER ---
    if (distance <= SEUIL_DANGER) {
      // Danger : Obstacle à moins de 10 cm -> Son continu
      digitalWrite(buzzerPin, HIGH);
      tempsAttenteBuzzer = 300; // Maintient le son et l'écran stable pendant 300ms
    }
    else if (distance <= SEUIL_ALERTE) {
      // Alerte : Obstacle entre 10 et 30 cm -> Bips intermittents
      // Plus la distance est petite, plus le silence entre les bips diminue (de 300ms à 30ms)
      int intervalleBip = map(distance, SEUIL_DANGER, SEUIL_ALERTE, 30, 300);

      digitalWrite(buzzerPin, HIGH);
      delay(40); // Durée flash du bip
      digitalWrite(buzzerPin, LOW);

      tempsAttenteBuzzer = intervalleBip;
    }
    else {
      // Zone sûre : Plus de 30 cm -> Silence total
      digitalWrite(buzzerPin, LOW);
      tempsAttenteBuzzer = 300;
    }
  }

  // Envoyer les données physiques à l'écran
  display.display();

  // Appliquer le délai calculé pour stabiliser le rythme sonore et visuel
  delay(tempsAttenteBuzzer);
}
