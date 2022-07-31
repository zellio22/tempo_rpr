
#include <LiquidCrystal_I2C.h> 	//Lib I2C en focntion de l'ecrant
#include <ParallaxLCD.h>		//Lib ParallaxLCD pour LCD serial //PARALLAX

#include <SoftwareSerial.h>		//Voir la necessitée d'include cette LIB normalement non //PARALLAX


#define ROWS 4				//Nombre de lignes Uniquement avec la lib paralax
#define COLS 20				//Nombre de colonnes Uniquement avec la lib paralax

ParallaxLCD lcds(9,ROWS,COLS); // choix pin TX, Lignes, colone //PARA
//ligne Colone

//def des variable global 

unsigned long tempo_debut;  	 	//DEbut	
unsigned long tempo_fin;			//Fin voir pour supp la variable
unsigned long delay_aff;			//delay pour affichage
float temp_tempo;					//Temps
int tempo = 0;						//Variable a 1 pour tenmpo en mesure

//def des pin Entrée
//1er com Arduino Mega // 2 em Arduino UNO
int EX = 5;//30 //5					//pin comutateur choix Exit d'ezexit
int D_EX = 6;//31 //6					//pin comutateur choix Exit d'ezexit
int in_tempo_409 = 2;//32 //2			//defini la pin utiliser pour le 409
int in_tempo_401 = 3;//33 //3			//defini la pin utiliser pour le 401
int in_start = 10;//34 //10				//defini la pin utiliser pour le start
int in_raz = 11;//35//11				//defini la pin utiliser pour le raz

//def des pin sortie

int out_relay = 8;//36 //8				//defini la pin utiliser pour le relai 3 de la platine 
int led_13 =13; // led sur la board

/**
 * void ParallaxLCD::playTone(int duration, int scale, int note)
 * Duration: 208 - 214 (1/64th note - 1 whole note[2 secs])
 * Scale   : 215 - 219 (3rd - 7th scale)
 * Note    : 220 - 232 (A, A#, B, etc..., F#, F, G#)
 */

void impmarch()  //declaration de la fonction pour jouer de la musique 
{
lcds.playTone(212, 215, 229);	//LA 3
lcds.playTone(212, 215, 229);	//LA 3
lcds.playTone(212, 215, 229);	//LA 3
lcds.playTone(211, 215, 225);	//FA 3
lcds.playTone(212, 216, 220);	//DO 1 *1
lcds.playTone(212, 215, 229);	//LA 3 
lcds.playTone(211, 215, 225);	//FA 3
lcds.playTone(213, 216, 220);	//DO 1 *1
lcds.playTone(212, 215, 229);	//LA 6
lcds.playTone(212, 216, 224);	//MI*1 3
lcds.playTone(212, 216, 224);	//MI*1 3
lcds.playTone(212, 216, 224);	//MI*1 3
lcds.playTone(211, 216, 225);	//FA*1 3
lcds.playTone(212, 216, 220);	//DO*1 3
lcds.playTone(212, 215, 227);	//SOL# 3
lcds.playTone(211, 216, 225);	//FA*1 3
lcds.playTone(213, 216, 220);	//DO 1 *1
lcds.playTone(212, 215, 229);	//LA 6

}

int choix()
{
	if (digitalRead(EX)==0 && digitalRead(D_EX)==1){ return 1;} // CHOIX TEMPO exitation 
	if (digitalRead(EX)==1 && digitalRead(D_EX)==0){ return 2;} // CHOIX TEMPO dezesxit
	else return 0;
}

void timeSplit(unsigned long ms){


    int heure   = ms / 60 / 60000 % 24;
    int minute  = ms / 60000 % 60;
    int seconde = ms / 1000 % 60;
    int miliseconde = ms %1000;

	int lenH = String(heure).length();
	int lenM = String(minute).length();
	int lenS = String(seconde).length();
	int lenMS = String(miliseconde).length();

	lcds.at(2,0,"T: ");//T: //3
    Serial.print("Time: ");
	lcds.at(2,3,heure);
    Serial.print(heure);
	lcds.at(2,3+lenH,"h ");
    Serial.print("h ");
	lcds.at(2,5+lenH,minute);
    Serial.print(minute);
	lcds.at(2,5+lenH+lenM,"m ");
    Serial.print("m ");
	lcds.at(2,7+lenH+lenM,seconde);
    Serial.print(seconde);
	lcds.at(2,7+lenH+lenM+lenS,"s ");
    Serial.print("s ");
	lcds.at(2,9+lenH+lenM+lenS,miliseconde);
    Serial.print(miliseconde);
	lcds.at(2,9+lenH+lenM+lenS+lenMS,"ms");
    Serial.print("ms   ");
    Serial.println(ms);
}

void setup() // la boucle setup  est executer unique a la mise en service de l'arduino
{

	Serial.begin(9600); // initialise la connexion série "USB" à 9600 bauds non utilisé
	
	//Set du LCD serial 
	lcds.setup();	//PARA
	lcds.backLightOn();//retro-eclairage	//PARALLAX
	delay(1000);//Wait du LCD 			//PARALLAX
	lcds.empty();//Clear de l'ecran		//PARALLAX
	
	//definition du roles des E/S 
	/*Nota le INPUL_PULLUP connecte une resistance entre l'entré de l'arduino et le +5V interne 
	* Donc l'entrée est vrai quand elle et reliée au GND de l'arduino
	* if (digitalRead(EX)==0){ return 1;} // Return 1 quand l'entrée est à 0
	* fonctionne comme une logique a manque 
	*/
	pinMode(in_tempo_409, INPUT_PULLUP); 		// 409 
	pinMode(in_tempo_401, INPUT_PULLUP); 		// 401
	pinMode(EX, INPUT_PULLUP);					// choix Exitation
	pinMode(D_EX, INPUT_PULLUP); 				// choix Desexciation
	pinMode(in_start,INPUT_PULLUP);		// start avec pullup
	pinMode(in_raz,INPUT_PULLUP);		// raz avec pullup

	pinMode(out_relay,OUTPUT);			// relai
	pinMode(led_13,OUTPUT);			// relai
}

void loop() //fonction Main Bloucle principale 
{

	switch (choix()) // voir en dessou
	/*
	La structure de controle Switch est une structure de controle qui permet de faire une seule action en fonction 
	du return de la fonction choix
	la fonction Choix return 0 pour une selection incorect 
	la fonction Choix return 1 pour une selection exitation
	la fonction Choix return 2 pour une selection desexitation
	*/

	{
		case 0: // corespond a un " if choix ==0 "et nexecute pas la suite du switch quand tu vien ici c'est que tu a choisi un choix qui n'existe pas

		{
			
			lcds.at(0,0,"Choix Incorrect     ");		//PARALLAX
			digitalWrite(out_relay, LOW);	//mise a 0 du relai quand on choisi un choix qui n'existe pas
			digitalWrite(led_13, LOW);		//mise a 0 de la led 13 quand on choisi un choix qui n'existe pas
			break;
		}
		
		case 1: // CHOIX TEMPO exitation 
		{
			lcds.at(0,0,"TEMPO excitation ");		//PARALLAX

			if (tempo==0 && digitalRead(out_relay)==HIGH){	
				digitalWrite(out_relay, LOW); //mise à 0 du relai
				digitalWrite(led_13, LOW);
				delay(50);
			}

			if (tempo==0 && digitalRead(in_start)==0){// si la tempo est off et que le start est en HIGH exitation du 401
				digitalWrite(out_relay, HIGH);// on allume le relai
				digitalWrite(led_13, HIGH);
			}
			
			if(digitalRead(in_tempo_409)==1 && tempo==0 ) // si l'entrée est en HIGH et que la tempo est off
			{
				tempo = 1; // on active le tempo
				tempo_debut = millis(); // on enregistre le temps de début
			}
			if(digitalRead(in_tempo_401)== 0 && tempo==1 ) // si l'entrée est en LOW et que la tempo est en cour
			{

				tempo_fin = millis(); // on enregistre le temps de fin
				temp_tempo = (float)(tempo_fin - tempo_debut)/1000; // on calcule le temps
				timeSplit(tempo_fin - tempo_debut);
				tempo = 0; // on desactive le tempo
				lcds.at(1,0,"Temps : ");	//PARALLAX
				String temp_tempo_str = String(temp_tempo);					//PARALLAX
				lcds.at(1,8,temp_tempo_str);	//PARALLA
				int len = temp_tempo_str.length();							//PARALLAX
				lcds.at(1,8+len," s      ");
				//lcds.at(2,0,"                  ");							//PARALLAX
				delay(200);						// on attend 200ms pour repos de la tempo
				digitalWrite(out_relay, LOW);	// on etein le relai
				digitalWrite(led_13, LOW);
				//impmarch();		//play la musique
				
			}

			break;//Fuck the system ...
		}
		
		case 2: // CHOIX TEMPO de-esxitation reste a coder ici 
		{

			lcds.at(0,0,"TEMPO Desexcitation");		//PARALLAX

			if (tempo==0 && digitalRead(out_relay)==LOW){
				digitalWrite(out_relay, HIGH);
				digitalWrite(led_13, HIGH);
				delay(50);
			}

			if (tempo==0 && digitalRead(in_start)==0){// 
				digitalWrite(out_relay, LOW);// 
				digitalWrite(led_13, LOW);
			}
			
			if(digitalRead(in_tempo_409)==0 && tempo==0 ) //
			{
				tempo = 1; // on active le tempo
				tempo_debut = millis(); // on enregistre le temps de début
			}

			if(digitalRead(in_tempo_401)== 0 && tempo==1 ) // si l'entrée est en LOW et que la tempo est en cour
			{
				tempo_fin = millis(); // on enregistre le temps de fin
				temp_tempo = (float)(tempo_fin - tempo_debut)/1000; // on calcule le temps
				timeSplit(tempo_fin - tempo_debut);
				tempo = 0; // on desactive le tempo
				lcds.at(1,0,"Temps : ");	//PARALLAX
				
				String temp_tempo_str = String(temp_tempo);					//PARALLAX
				lcds.at(1,8,temp_tempo_str);	//PARALLA
				int len = temp_tempo_str.length();							//PARALLAX
				lcds.at(1,8+len," s      ");
				//lcds.at(2,0,"                  ");	//PARALLAX
				delay(200);						// on attend 200ms pour repos de la tempo
				digitalWrite(out_relay, LOW);	// on etein le relai
				digitalWrite(led_13, LOW);
				//impmarch();
			}
			break;//Fu
		}


		
	}

if(tempo==1){		//Affichage chrono
	if(millis()-delay_aff>100){ //delay discret de 100ms
		delay_aff=millis(); // on enregistre le temps
		lcds.at(1,0,"Tempo en cours:        ");			//PARALLAX
		lcds.at(2,4,"                       ");	//PARALLAX
		lcds.at(2,0,String(float(millis()-tempo_debut)/1000));	//PARALLAX
	}
}

//raz
if (digitalRead(in_raz)==0){		// si le raz est en HIGH
	temp_tempo=0;					//Reset du timer
	tempo = 0;						//Reset de l'etat tempo
	lcds.at(0,0,"                    ");	//PARALLAX
	lcds.at(1,0,"                    ");	//PARALLAX
	lcds.at(2,0,"                    ");	//PARALLAX	
	lcds.at(3,0,"                    ");	//PARALLAX
	}

}






















