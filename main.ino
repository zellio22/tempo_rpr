
#include <LiquidCrystal_I2C.h>
#include <arduino.h>
LiquidCrystal_I2C lcd(0x27,20,4);	// adresse du LCD I2C

// def des variable global 

unsigned long tempo_debut;  	 	//DEbut	
unsigned long tempo_fin;			//Fin
float temp_t;						//Temps

int in_td = 1;			//defini la pin utiliser pour le 409
int in_tf = 2;			//defini la pin utiliser pour le 401

int EX = 31; 			//pin comutateur choix Exit d'esexit
int D_EX = 33;			//pin comutateur choix Exit d'esexit

//Variable internes
int statein_409 = 0;	//etat actuel de l'entrée 
int lastin_409 = 0;		//etat precedent de l'entrée

int last_choix=0;
int statein_401 = 0;	//etat actuel de l'entrée 
int lastin_401 = 0;		//etat precedent de l'entrée

int t1 = 0;				//Variable a 1 pour tenmpo en mesure

int choix()
{
	if (digitalRead(EX)==0 && digitalRead(D_EX)==1){ return 1;} // CHOIX TEMPO exitation 
	if (digitalRead(EX)==1 && digitalRead(D_EX)==0){ return 2;} // CHOIX TEMPO de-esxitation
	else return 0;
}

void setup() 
{
	lcd.init();			// initialisation de l'ecrant LCD 
	lcd.backlight();	// allumage du retro-eclairage 
	lcd.setCursor(0,0); // positionement du curseur 
	Serial.begin(9600); // initialise la connexion série à 9600 bauds
	
	//definition des entrées 
	pinMode(in_td, INPUT); // 409 
	pinMode(in_tf, INPUT); // 401
	pinMode(EX, INPUT);	// choix E
	pinMode(D_EX, INPUT); // choix D

}

void loop() 
{
	delay(1000);
	Serial.print("choix : ");
	Serial.println(choix());
	Serial.print("EX : ");
	Serial.println(digitalRead(EX));
	Serial.print("D_EX : ");
	Serial.println(digitalRead(D_EX));


	switch (choix()) 
	{
		case 0: // BUG 
		{
			
			lcd.setCursor(0,0);
			lcd.print("Un seul choix   ");
			break;
		}
		
		case 1: // CHOIX TEMPO exitation 
		{
			
			lcd.setCursor(0,0);
			lcd.print("TEMPO exitation ");
			break;
		}
		
		case 2: // CHOIX TEMPO de-esxitation
		{
			
			lcd.setCursor(0,0);
			lcd.print("TEMPO de-zexit  ");
			break;
		}
		
	}
}






















