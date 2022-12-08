#include "MotorDriver.h"

// ========================================================================================================
// --- Mapeamento de Hardware ---
#define   vcc      5
#define   gnd      4
#define   pino_D0  3
#define   analog   2

 
// ========================================================================================================
// --- Variáveis Globais --- 
int           rpm;
volatile byte pulsos;
unsigned long timeold, currentTime;

//Altere o numero abaixo de acordo com o seu disco encoder
unsigned int pulsos_por_volta = 20;


// ========================================================================================================
// --- Interrupção ---
void contador()
{
  //Incrementa contador
  timeold = currentTime;
  currentTime = millis();
  Serial.print(currentTime-timeold);

}


// ========================================================================================================
// --- Configurações Iniciais ---
void setup()
{
  Serial.begin(9600);

  pinMode(vcc,    OUTPUT);
  pinMode(gnd,    OUTPUT);
  pinMode(pino_D0, INPUT);
  pinMode(analog,  INPUT);

  digitalWrite(vcc, HIGH);
  digitalWrite(gnd,  LOW);
  
  //Interrupcao 1 - pino digital 3
  //Aciona o contador a cada pulso
  attachInterrupt(2, contador, FALLING);
  pulsos = 0;
  rpm = 0;
  timeold = 0;

  Serial.println("----NEW----");

  motordriver.init();
  motordriver.setSpeed(100,MOTORA);
  motordriver.setSpeed(100,MOTORB);

  motordriver.goForward();
  
} //end setup


// ========================================================================================================
// --- Loop Infinito ---
void loop()
{ 
  Serial.println(currentTime);
  Serial.println(timeold);
  Serial.println(currentTime-timeold);
  delay(100);
} //end loop




