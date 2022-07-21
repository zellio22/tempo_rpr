


#include <LiquidCrystal_I2C.h> 	//Lib I2C en focntion de l'ecrant
#include <ParallaxLCD.h>		//Lib ParallaxLCD pour LCD serial 
#include <arduino.h>
#include <SoftwareSerial.h>		//Voir la necessitée d'include cette LIB normalement non 

//reste a faire 
//une gestion du relai d'exitation tempo 
//une raz de toutes les variables global car je code comme un porc 
//la mesure des tempos a la d'esexitation
//lib paralax https://github.com/iamthechad/parallax_lcd/blob/master/examples/LCD_2x16/LCD_2x16.ino


#define ROWS 4
#define COLS 10
//ParallaxLCD lcd(2,ROWS,COLS); // choix pin TX, Lignes, colone


LiquidCrystal_I2C lcd(0x27,20,4);	// adresse du LCD I2C

// def des variable global 

unsigned long tempo_debut;  	 	//DEbut	
unsigned long tempo_fin;			//Fin voir pour supp la variable
unsigned long delay_aff;			//delay pour affichage
float temp_tempo;					//Temps
int tempo = 0;						//Variable a 1 pour tenmpo en mesure

//def des pin Entrée /sortie

int EX = 0; 					//pin comutateur choix Exit d'ezexit
int D_EX = 1;					//pin comutateur choix Exit d'ezexit
int in_tempo_409 = 2;			//defini la pin utiliser pour le 409
int in_tempo_401 = 3;			//defini la pin utiliser pour le 401
int out_relay = 4;



int choix()
{
	if (digitalRead(EX)==1 && digitalRead(D_EX)==0){ return 1;} // CHOIX TEMPO exitation 
	if (digitalRead(EX)==0 && digitalRead(D_EX)==1){ return 2;} // CHOIX TEMPO dezesxit
	else return 0;
}

void setup() // la boucle setup  est executer unique a la mise en service de l'arduino
{
	//Set du LCD i2c
	lcd.init();			// initialisation de l'ecrant LCD 
	lcd.backlight();	// allumage du retro-eclairage 
	lcd.setCursor(0,0); // positionement du curseur 
	Serial.begin(9600); // initialise la connexion série à 9600 bauds
	
	//Set du LCD serial 
	//lcd.setup();
	//lcd.backLightOn();//retro-eclairage
	//delay(1000);//Wait du LCD 
	//lcd.empty();//Clear de l'ecran
	
	//definition du roles des E/S 
	pinMode(in_tempo_409, INPUT); // 409 
	pinMode(in_tempo_401, INPUT); // 401
	pinMode(EX, INPUT);	// choix Exitation
	pinMode(D_EX, INPUT); // choix Desexciation

}

void loop() 
{

	switch (choix()) // voir comme en dessou
	/*
	La structure de controle Switch est une structure de controle qui permet de faire une seule action en fonction 
	du return de la fonction choix
	la fonction Choix return 0 pour une selection incorect 
	la fonction Choix return 1 pour une selection exitation
	la fonction Choix return 2 pour une selection desexitation
	*/

	{
		case 0: // corespond a un " if choix ==0 "et nexecute pas la suite du switch

		{
			
			lcd.setCursor(0,0);
			lcd.print("Un seul choix   ");//quand tu vien ici c'est que tu a choisi un choix qui n'existe pas ... merci l'ia
			//lcd.at(0,0,"Choix Incorect");//Pour LCD Serial
			break;
		}
		
		case 1: // CHOIX TEMPO exitation 
		{
			lcd.setCursor(0,0);// on positionement du curseur a la ligne 0 colonne 0
			lcd.print("TEMPO exitation ");// on affiche le message
			//lcd.at(0,0,"TEMPO exitation ");
			
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
				lcd.setCursor(0,2);// on positionne le curseur sur la 3em ligne
				lcd.print("                  ");// on efface la 3eme ligne
			}

			if(tempo==1){
				if(millis()-delay_aff>100){ //delay discret de 100ms
					delay_aff=millis(); // on enregistre le temps
					lcd.setCursor(0,1);// on positionne le curseur sur la 2em ligne
					lcd.print("Tempo en cours: ");// on affiche le texte
					lcd.setCursor(0,2);// on positionne le curseur sur la 3em ligne
					lcd.print(float(millis()-tempo_debut)/1000);// on affiche le temps en temp reel in real time 
				}
			}


			break;//Fuck the system ...
		}
		
		case 2: // CHOIX TEMPO de-esxitation reste a coder ici 
		{
			
			lcd.setCursor(0,0); 
			lcd.print("TEMPO de-zexit  ");
			break;
		}
		
	}
}






















