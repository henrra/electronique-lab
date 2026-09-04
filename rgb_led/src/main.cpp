#include <Arduino.h>

// www.elegoo.com
// 2016.12.8

// Definition des broches
#define BLUE 2
#define GREEN 5
#define RED 4

void setup()
{
	pinMode(RED, OUTPUT);
	pinMode(GREEN, OUTPUT);
	pinMode(BLUE, OUTPUT);
	digitalWrite(RED, HIGH);
	digitalWrite(GREEN, LOW);
	digitalWrite(BLUE, LOW);
}

// Definition des variables
int redValue;
int greenValue;
int blueValue;

// Boucle principale
void loop()
{
#define delayTime 10 // Duree de transition entre les couleurs

	redValue = 255; // Choisir une valeur entre 1 et 255 pour modifier la couleur.
	greenValue = 0;
	blueValue = 0;

	// Cette instruction est inutile car la LED rouge a deja ete allumee dans setup()
	// ou lors de la boucle precedente. Dans tous les cas, elle eteint la LED rouge.
	// analogWrite(RED, 0);
	// delay(1000);

	for (int i = 0; i < 255; i += 1) // Diminue le rouge et augmente le vert jusqu'a i = 255
	{
		redValue -= 1;
		greenValue += 1;
		// Les instructions suivantes etaient inversees et comptaient dans le mauvais sens.
		// analogWrite(RED, 255 - redValue);
		// analogWrite(GREEN, 255 - greenValue);
		analogWrite(RED, redValue);
		analogWrite(GREEN, greenValue);
		delay(delayTime);
	}

	redValue = 0;
	greenValue = 255;
	blueValue = 0;

	for (int i = 0; i < 255; i += 1) // Diminue le vert et augmente le bleu jusqu'a i = 255
	{
		greenValue -= 1;
		blueValue += 1;
		// Les instructions suivantes etaient inversees et comptaient dans le mauvais sens.
		// analogWrite(GREEN, 255 - greenValue);
		// analogWrite(BLUE, 255 - blueValue);
		analogWrite(GREEN, greenValue);
		analogWrite(BLUE, blueValue);
		delay(delayTime);
	}

	redValue = 0;
	greenValue = 0;
	blueValue = 255;

	for (int i = 0; i < 255; i += 1) // Diminue le bleu et augmente le rouge jusqu'a i = 255
	{
		// Le code suivant a ete reorganise pour correspondre aux deux autres sections similaires.
		blueValue -= 1;
		redValue += 1;
		// Les instructions suivantes etaient inversees et comptaient dans le mauvais sens.
		// analogWrite(BLUE, 255 - blueValue);
		// analogWrite(RED, 255 - redValue);
		analogWrite(BLUE, blueValue);
		analogWrite(RED, redValue);
		delay(delayTime);
	}
}