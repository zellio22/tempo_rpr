
#include <LiquidCrystal_I2C.h>
#include <arduino.h>

//reste a faire 
//une gestion du relai d'exitation tempo 
//une raz 
//la mesure des tempos a la d'esexitation

LiquidCrystal_I2C lcd(0x27,20,4);	// adresse du LCD I2C

// def des variable global 

unsigned long tempo_debut;  	 	//DEbut	
unsigned long tempo_fin;			//Fin
float temp_tempo;					//Temps

int in_tempo_409 = 35;			//defini la pin utiliser pour le 409
int in_tempo_401 = 37;			//defini la pin utiliser pour le 401

int EX = 31; 			//pin comutateur choix Exit d'esexit
int D_EX = 33;			//pin comutateur choix Exit d'esexit

//Variable internes
int statein_409 = 0;	//etat actuel de l'entrée 
int lastin_409 = 0;		//etat precedent de l'entrée

int last_choix=0;
int statein_401 = 0;	//etat actuel de l'entrée 
int lastin_401 = 0;		//etat precedent de l'entrée

float tempo = 0;				//Variable a 1 pour tenmpo en mesure

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
	pinMode(in_tempo_409, INPUT); // 409 
	pinMode(in_tempo_401, INPUT); // 401
	pinMode(EX, INPUT);	// choix E
	pinMode(D_EX, INPUT); // choix D

}

void loop() 
{

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
			lcd.setCursor(0,0);// on positionement du curseur a la ligne 0 colonne 0
			lcd.print("TEMPO exitation ");// on affiche le message
			
			if(digitalRead(in_tempo_409)==1 && tempo==0 ) // si l'entrée est en HIGH et que la tempo est off
			{
				tempo = 1; // on active le tempo
				tempo_debut = millis(); // on enregistre le temps de début
			}
			if(digitalRead(in_tempo_401)== 1 && tempo==1 ) // si l'entrée est en LOW et que la tempo est en cour
			{
				tempo = 0; // on desactive le tempo
				tempo_fin = millis(); // on enregistre le temps de fin
				temp_tempo = (float)(tempo_fin - tempo_debut)/1000; // on calcule le temps
				lcd.setCursor(0,1);// on positionne le curseur sur la 2em ligne 
				lcd.print("Temps : ");// on affiche le texte
				lcd.print(temp_tempo);// on affiche le temps
				lcd.print(" s      ");// on affiche l'unité et on rajouter des espace pour fair joly'
			}

			if(tempo==1){
				lcd.setCursor(0,1);// on positionne le curseur sur la 2em ligne
				lcd.print("Tempo en cours: ");// on affiche le texte
				lcd.print(float(millis()-tempo_debut)/1000);// on affiche le temps en temp reel in real time 
			}


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






















