#include <Arduino.h>
#include <SPI.h>     // Bibliothèque pour la communication SPI
#include <MFRC522.h> // Bibliothèque pour le module RFID
#include <Wire.h>
#include <U8g2lib.h>

#define SS_PIN 5      // Broche SDA (Chip Select)
#define RST_PIN 4     // Broche Reset
#define BUZZER_PIN 25 // Broche du buzzer

const byte AUTH_UID[] = {0xA3, 0x98, 0x64, 0x06}; // Remplace par ton vrai UID

MFRC522 rfid(SS_PIN, RST_PIN);

// Configuration de l'écran OLED (SSD1306 128x64 I2C)
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// Définition des broches pour les LED
#define LED_RED_PIN   12
#define LED_GREEN_PIN 14

// Prototypes de fonctions
void displayCardUID();
bool isAuthorizedUID();
void beep(uint8_t repeat, uint16_t duration);
void playAccessGranted();
void playAccessDenied();
void printDefaultInfos();
void printAccessGranted();
void printAccessDenied();

void setup()
{
    Serial.begin(115200);
    SPI.begin();
    rfid.PCD_Init();
        
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LED_GREEN_PIN, OUTPUT);
    pinMode(LED_RED_PIN, OUTPUT);

    digitalWrite(LED_GREEN_PIN, LOW);
    digitalWrite(LED_RED_PIN, LOW);

    u8g2.begin();
    u8g2.enableUTF8Print(); // Active la prise en charge UTF-8 globale pour U8g2

    beep(1, 100); // Bip au démarrage

    printDefaultInfos();
}

void loop()
{
    // Vérifie si une nouvelle carte est présente
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    {
        delay(50); // Petite pause pour alléger le CPU
        return;
    }

    // Vérifie les droits d'accès
    if (isAuthorizedUID())
    {       
        playAccessGranted();
    }
    else
    {
        playAccessDenied();
    }

    // Réinitialise l'affichage par défaut après le traitement
    printDefaultInfos();

    // Halte de la carte
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    
    delay(1000); // Anti-rebond avant de pouvoir relire une carte
}

void displayCardUID()
{
    char buffer[30] = "";
    MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
    String typeText = String(rfid.PICC_GetTypeName(piccType));

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawUTF8(0, 12, "Type:");
    u8g2.drawUTF8(40, 12, typeText.c_str());

    strcpy(buffer, "UID: ");
    for (byte i = 0; i < rfid.uid.size; i++)
    {
        char temp[5];
        snprintf(temp, sizeof(temp), "%02X ", rfid.uid.uidByte[i]);
        strcat(buffer, temp);
    }
    u8g2.drawUTF8(0, 32, buffer);
    u8g2.sendBuffer();
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

void beep(uint8_t repeat, uint16_t duration)
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
    printAccessGranted();
    digitalWrite(LED_GREEN_PIN, HIGH);

    tone(BUZZER_PIN, 1318, 100);
    delay(150);
    tone(BUZZER_PIN, 1760, 200);
    delay(200);
    noTone(BUZZER_PIN);

    delay(1500); // Maintient la LED et le message
    digitalWrite(LED_GREEN_PIN, LOW);
}

void playAccessDenied()
{
    printAccessDenied();
    digitalWrite(LED_RED_PIN, HIGH);

    tone(BUZZER_PIN, 370, 150);
    delay(180);
    tone(BUZZER_PIN, 185, 300);
    delay(300);
    noTone(BUZZER_PIN);

    delay(1500); // Maintient la LED et le message
    digitalWrite(LED_RED_PIN, LOW);
}

void printDefaultInfos()
{
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawUTF8(20, 12, "Présentez badge");
    u8g2.drawLine(0, 18, 127, 18);
    
    u8g2.setFont(u8g2_font_helvB12_tf); // Police avec support complet des accents (_tf)
    u8g2.drawUTF8(20, 45, "Tonga soa !");
    u8g2.sendBuffer();
}

void printAccessGranted()
{
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_helvB10_tf); // Police _tf pour UTF-8
    u8g2.drawUTF8(8, 38, "Accès autorisé");
    u8g2.sendBuffer();
}

void printAccessDenied()
{
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_helvB10_tf); // Police _tf pour UTF-8
    u8g2.drawUTF8(12, 38, "Accès refusé");
    u8g2.sendBuffer();
}