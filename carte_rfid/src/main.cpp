#include <Arduino.h>
#include <SPI.h>     // Bibliothèque pour la communication SPI
#include <MFRC522.h> // Bibliothèque pour le module RFID
#include <Adafruit_SSD1306.h>
#include <Wire.h>

// Importation d'une police spécifique (ex: taille 9pt ou 12pt)
// Adafruit GFX Library
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>

#define SS_PIN 5   // Broche SDA (Chip Select)
#define RST_PIN 4    // Déplacé du GPIO 22 au GPIO 4 pour libérer l'OLED !
#define BUZZER_PIN 25 // Broche du buzzer, à adapter selon le montage

const byte AUTH_UID[] = {0xA3, 0x98, 0x64, 0x06}; // Remplace par le vrai UID autorisé

MFRC522 rfid(SS_PIN, RST_PIN); // Création de l'instance du module


// Configuration de l'écran OLED (Taille standard 128x64)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void displayCardUID();
bool isAuthorizedUID();
void beep(uint8_t repeat, uint16_t duration);
void playAccessGranted();
void playAccessDenied();

void setup()
{
    Serial.begin(115200); // Initialisation du moniteur série
    SPI.begin();          // Initialisation du bus SPI
    rfid.PCD_Init();      // Initialisation du module RC522
        
    pinMode(BUZZER_PIN, OUTPUT); // Définir la broche du buzzer comme sortie
    // digitalWrite(BUZZER_PIN, LOW); // Éteindre le buzzer au démarrage
    beep(1, 100); // Buzzer pour indiquer que le système est prêt

     // Initialisation de l'écran OLED à l'adresse 0x3C (adresse standard)
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("Erreur : Écran OLED non détecté !"));
        for (;;)
            ; // Bloquer le programme si l'écran n'est pas trouvé
    }

    // Message d'accueil sur l'OLED
    display.clearDisplay();
    // 1. Activer la police personnalisée
    display.setFont(&FreeSans9pt7b); 
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(20, 30);
    display.println(F("Tongasoa !"));
    display.display();
    delay(1500);
}

void loop()
{
     
    // Effacer l'écran avant d'écrire la nouvelle valeur
    display.clearDisplay();
    
    // Titre en haut de l'écran
    display.setFont(NULL); // Revenir à la police par défaut pour le titre
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println(F("Votre badge"));
    display.drawLine(0, 10, 128, 10, SSD1306_WHITE); // Petite ligne de séparation
    

    display.setTextSize(1);
    display.setFont(&FreeSans9pt7b);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(20, 30);
    display.println(F("Tonga soa !"));
    display.display();

    // Vérifie si une nouvelle carte est présente
    if (!rfid.PICC_IsNewCardPresent())
    {
        delay(2000);
        return;
    }

    // Vérifie si l'UID de la carte a pu être lu
    if (!rfid.PICC_ReadCardSerial())
    {
        delay(2000);
        return;
    }

    if (isAuthorizedUID())
    {
        display.setCursor(5, 50);
        display.println(F("Acces autorise"));
        display.display();        
        playAccessGranted();
        // Ici tu mets ta vraie action pour le badge autorisé
    }
    else
    {
        display.setCursor(5, 50);
        display.println(F("Acces refuse"));
        display.display();
        playAccessDenied();
        // Ici tu mets ta vraie action pour le badge non autorisé
    }

    delay(2000); // Pause pour permettre à l'utilisateur de voir le résultat

    // Arrête la communication avec la carte actuelle
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
}

void displayCardUID()
{
    // Affichage du type de carte (optionnel)
    display.clearDisplay();
    display.setFont(NULL); 
    display.setCursor(5, 20);
    display.print("Type: ");
    MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
    display.println(rfid.PICC_GetTypeName(piccType));
    display.display();

    display.setCursor(5, 30);
    display.print(F("UID: "));
    for (byte i = 0; i < rfid.uid.size; i++)
    {
        display.print(' ');
        if (rfid.uid.uidByte[i] < 0x10)
            display.print('0');
        display.print(rfid.uid.uidByte[i], HEX);
    }

    display.display();
}

bool isAuthorizedUID()
{
    if (rfid.uid.size != sizeof(AUTH_UID))
    {
        return false;
    }

    for (byte i = 0; i < rfid.uid.size; i++)
    {
        if (rfid.uid.uidByte[i] != AUTH_UID[i])
        {
            return false;
        }
    }

    return true;
}

void beep(uint8_t repeat = 1, uint16_t duration = 200)
{
    for (uint8_t i = 0; i < repeat; i++)
    {
        tone(BUZZER_PIN, 2000, duration);
        delay(duration + 80);
    }
    noTone(BUZZER_PIN);
}

void playAccessGranted()
{
    tone(BUZZER_PIN, 1318, 100); // 1er bip (Mi 6) pendant 100 ms
    delay(150);                  // 100 ms de son + 50 ms de pause

    tone(BUZZER_PIN, 1760, 200); // 2e bip plus aigu (La 6) pendant 200 ms
    delay(200);

    noTone(BUZZER_PIN); // Sécurité pour couper le son
}

void playAccessDenied()
{
    tone(BUZZER_PIN, 370, 150); // Note grave
    delay(180);                 // Durée note + pause

    tone(BUZZER_PIN, 185, 300); // Note encore plus grave
    delay(300);

    noTone(BUZZER_PIN);
}