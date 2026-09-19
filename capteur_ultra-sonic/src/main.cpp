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

void setup()
{
  // Initialisation du moniteur série (aligné sur platformio.ini)
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

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

  // Affichage du résultat
  if (duration == 0 || distance > 400)
  {
    display.setTextSize(2);
    display.setCursor(10, 30);
    display.println("Erreur / HW");
  }
  else
  {
    // Affichage de la distance en gros au milieu
    display.setTextSize(2);
    display.setCursor(10, 30);
    display.print(distance, 1); // 1 chiffre après la virgule
    display.println(" cm");
  }

  // Envoyer les données physiques à l'écran
  display.display();

  delay(1000); // Rafraîchissement toutes les 300ms
}
