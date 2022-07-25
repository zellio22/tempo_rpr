


#include <LiquidCrystal_I2C.h> 	//Lib I2C en focntion de l'ecrant
//#include <ParallaxLCD.h>		//Lib ParallaxLCD pour LCD serial //PARALLAX
//#include <arduino.h>
//#include <SoftwareSerial.h>		//Voir la necessitée d'include cette LIB normalement non //PARALLAX

//reste a faire 

//voire commen gerer la cmd du relai pour EX/D_EX
//la mesure des tempos a la d'esexitation a verifier 

#define ROWS 4				//Nombre de lignes Uniquement avec la lib paralax
#define COLS 10				//Nombre de colonnes Uniquement avec la lib paralax
//ParallaxLCD lcd(2,ROWS,COLS); // choix pin TX, Lignes, colone //PARA


LiquidCrystal_I2C lcd(0x27,20,4);	// adresse du LCD I2C //I2C

//def des variable global 

unsigned long tempo_debut;  	 	//DEbut	
unsigned long tempo_fin;			//Fin voir pour supp la variable
unsigned long delay_aff;			//delay pour affichage
float temp_tempo;					//Temps
int tempo = 0;						//Variable a 1 pour tenmpo en mesure

//def des pin Entrée

int EX = 4; 					//pin comutateur choix Exit d'ezexit
int D_EX = 5;					//pin comutateur choix Exit d'ezexit
int in_tempo_409 = 2;			//defini la pin utiliser pour le 409
int in_tempo_401 = 3;			//defini la pin utiliser pour le 401
int in_start = 6;				//defini la pin utiliser pour le start
int in_raz = 7;					//defini la pin utiliser pour le raz

//def des pin sortie

int out_relay = 8;				//defini la pin utiliser pour le relai 3 de la platine 



int choix()
{
	if (digitalRead(EX)==1 && digitalRead(D_EX)==0){ return 1;} // CHOIX TEMPO exitation 
	if (digitalRead(EX)==0 && digitalRead(D_EX)==1){ return 2;} // CHOIX TEMPO dezesxit
	else return 0;
}

void setup() // la boucle setup  est executer unique a la mise en service de l'arduino
{
	//Set du LCD i2c
	lcd.init();			// initialisation de l'ecrant LCD 	//I2C
	lcd.backlight();	// allumage du retro-eclairage 		//I2C
	lcd.setCursor(0,0); // positionement du curseur 		//I2C

	Serial.begin(9600); // initialise la connexion série "USB" à 9600 bauds 
	
	//Set du LCD serial 
	//lcd.setup();	//PARA
	//lcd.backLightOn();//retro-eclairage	//PARALLAX
	//delay(1000);//Wait du LCD 			//PARALLAX
	//lcd.empty();//Clear de l'ecran		//PARALLAX
	
	//definition du roles des E/S 
	pinMode(in_tempo_409, INPUT_PULLUP); 		// 409 
	pinMode(in_tempo_401, INPUT_PULLUP); 		// 401
	pinMode(EX, INPUT_PULLUP);					// choix Exitation
	pinMode(D_EX, INPUT_PULLUP); 				// choix Desexciation
	pinMode(in_start,INPUT_PULLUP);		// start avec pullup
	pinMode(in_raz,INPUT_PULLUP);		// raz avec pullup

	pinMode(out_relay,OUTPUT);			// relai

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
			//lcd.at(0,0,"Choix Incorect");		//PARALLAX
			break;
		}
		
		case 1: // CHOIX TEMPO exitation 
		{
			lcd.setCursor(0,0);// on positionement du curseur a la ligne 0 colonne 0
			lcd.print("TEMPO exitation ");// on affiche le message

			if (tempo==0 && digitalRead(out_relay)==HIGH){
				digitalWrite(out_relay, LOW);
				delay(50);
			}

			if (tempo==0 && digitalRead(in_start)==1){// si la tempo est off et que le start est en HIGH exitation du 401
				digitalWrite(out_relay, HIGH);// on allume le relai
			}
			//lcd.at(0,0,"TEMPO exitation ");	//PARALLAX
			
			if(digitalRead(in_tempo_409)==1 && tempo==0 ) // si l'entrée est en HIGH et que la tempo est off
			{
				tempo = 1; // on active le tempo
				tempo_debut = millis(); // on enregistre le temps de début
			}
			if(digitalRead(in_tempo_401)== 1 && tempo==1 ) // si l'entrée est en LOW et que la tempo est en cour
			{

				tempo_fin = millis(); // on enregistre le temps de fin
				temp_tempo = (float)(tempo_fin - tempo_debut)/1000; // on calcule le temps
				tempo = 0; // on desactive le tempo
				lcd.setCursor(0,1);// on positionne le curseur sur la 2em ligne //I2C
				lcd.print("Temps : ");// on affiche le texte					//I2C
				//lcd.at(0,1,"Temps : ");	//PARALLAX
				lcd.print(temp_tempo);// on affiche le temps					//I2C
				lcd.print(" s      ");// on affiche l'unité et on rajouter des espace pour fair joly'//I2C
				//lcd.at(0,1,"Temps : ");	//PARALLAX
				lcd.setCursor(0,2);// on positionne le curseur sur la 3em ligne	//I2C
				lcd.print("                  ");// on efface la 3eme ligne
				//lcd.at(0,2,"                  ");	//PARALLAX
				delay(200);						// on attend 200ms pour repos de la tempo
				digitalWrite(out_relay, LOW);	// on etein le relai
			}

			break;//Fuck the system ...
		}
		
		case 2: // CHOIX TEMPO de-esxitation reste a coder ici 
		{
			
			lcd.setCursor(0,0); 		//I2C
			lcd.print("TEMPO de-zexit  ");//I2C

			if (tempo==0 && digitalRead(out_relay)==LOW){
				digitalWrite(out_relay, HIGH);
				delay(50);
			}

			if (tempo==0 && digitalRead(in_start)==1){// 
				digitalWrite(out_relay, LOW);// 
			}
			
			if(digitalRead(in_tempo_409)==0 && tempo==0 ) //
			{
				tempo = 1; // on active le tempo
				tempo_debut = millis(); // on enregistre le temps de début
			}

			if(digitalRead(in_tempo_401)== 1 && tempo==1 ) // si l'entrée est en LOW et que la tempo est en cour
			{
				tempo_fin = millis(); // on enregistre le temps de fin
				temp_tempo = (float)(tempo_fin - tempo_debut)/1000; // on calcule le temps
				tempo = 0; // on desactive le tempo
				lcd.setCursor(0,1);// on positionne le curseur sur la 2em ligne //I2C
				lcd.print("Temps : ");// on affiche le texte					//I2C
				//lcd.at(0,1,"Temps : ");	//PARALLAX
				lcd.print(temp_tempo);// on affiche le temps					//I2C
				lcd.print(" s      ");// on affiche l'unité et on rajouter des espace pour fair joly'//I2C
				//lcd.at(0,1,"Temps : ");	//PARALLAX
				lcd.setCursor(0,2);// on positionne le curseur sur la 3em ligne	//I2C
				lcd.print("                  ");// on efface la 3eme ligne
				//lcd.at(0,2,"                  ");	//PARALLAX
				delay(200);						// on attend 200ms pour repos de la tempo
				digitalWrite(out_relay, LOW);	// on etein le relai
			}
			break;//Fu
		}


		
	}

if(tempo==1){		//Affichage chrono
	if(millis()-delay_aff>100){ //delay discret de 100ms
		delay_aff=millis(); // on enregistre le temps
		lcd.setCursor(0,1);// on positionne le curseur sur la 2em ligne	//I2C
		lcd.print("Tempo en cours: ");// on affiche le texte			//I2C
		//lcd.at(0,1,"Tempo en cours: ");			//PARALLAX
		lcd.setCursor(0,2);// on positionne le curseur sur la 3em ligne	//I2C
		lcd.print(float(millis()-tempo_debut)/1000);// on affiche le temps en temp reel in real time	//I2C 
		//lcd.at(0,2,float(millis()-tempo_debut)/1000);	//PARALLAX
	}
}


//raz
if (digitalRead(in_raz)==1){		// si le raz est en HIGH
	digitalWrite(out_relay, LOW);	// on etein le relai
	temp_tempo=0;					//Reset du timer
	tempo = 0;						//Reset de l'etat tempo
	lcd.setCursor(0,0); 
	lcd.print("                    ");		//I2C
	//lcd.at(0,0,"                    ");	//PARALLAX
	lcd.setCursor(0,1); 
	lcd.print("                    ");		//I2C
	//lcd.at(0,1,"                    ");	//PARALLAX
	lcd.setCursor(0,2);
	lcd.print("                    ");		//I2C
	//lcd.at(0,2,"                    ");	//PARALLAX
	lcd.setCursor(0,3);
	lcd.print("                    ");		//I2C	
	//lcd.at(0,3,"                    ");	//PARALLAX
	}




}






















