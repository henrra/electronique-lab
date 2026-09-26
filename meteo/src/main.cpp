#include <Arduino.h>
#include <DHT.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

// Configuration du capteur DHT
#define DHTPIN 14
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Configuration de l'écran OLED (Taille standard 128x64)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup()
{
    Serial.begin(115200);
    dht.begin();

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
    display.println(F("Demarrage..."));
    display.display();
    delay(1500);
}

void loop()
{
    delay(1000);
    
    // Effacer l'écran avant d'écrire la nouvelle valeur
    display.clearDisplay();
    
    // Titre en haut de l'écran
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println(F("METEO"));
    display.drawLine(0, 10, 128, 10, SSD1306_WHITE); // Petite ligne de séparation

    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    if (isnan(humidity) || isnan(temperature))
    {
        Serial.println(F("Échec de la lecture du capteur DHT !"));
        return;
    }

     // Affichage Température
    display.setTextSize(1);
    display.setCursor(0, 25);
    display.print("Temp: ");
    display.setTextSize(2);      // Plus grande taille pour la valeur
    display.print(temperature, 1); // 1 chiffre après la virgule
    display.setTextSize(1);
    display.write(167);          // Symbole du degré (°)
    display.println("C");

    // Affichage Humidité
    display.setTextSize(1);
    display.setCursor(0, 48);
    display.print("Hum : ");
    display.setTextSize(2);
    display.print(humidity, 1);
    display.setTextSize(1);
    display.println(" %");

    // Envoyer les données physiques à l'écran
    display.display();
}