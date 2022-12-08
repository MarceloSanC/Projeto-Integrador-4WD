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
unsigned long timeold;

//Altere o numero abaixo de acordo com o seu disco encoder
unsigned int pulsos_por_volta = 20;


// ========================================================================================================
// --- Interrupção ---
void contador()
{
  //Incrementa contador
  pulsos++;
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
  attachInterrupt(1, contador, FALLING);
  pulsos = 0;
  rpm = 0;
  timeold = 0;

  Serial.println("----NEW----");

  motordriver.init();
  motordriver.setSpeed(255,MOTORA);
  motordriver.setSpeed(255,MOTORB);

  motordriver.goForward();
  
} //end setup


// ========================================================================================================
// --- Loop Infinito ---
void loop()
{
  //Atualiza contador a cada segundo
  if (millis() - timeold >= 100)
  {
    //Desabilita interrupcao durante o calculo
    detachInterrupt(1);
    rpm = (60 * 1000 / pulsos_por_volta ) / (millis() - timeold) * pulsos;
    timeold = millis();
    pulsos = 0;

    //Mostra o valor de RPM no serial monitor
    //Serial.print("RPM = ");
    if(rpm != 0){
      Serial.println(rpm, DEC);
    }
    //Habilita interrupcao
    attachInterrupt(1, contador, FALLING);
  }

   if (millis() >= 1000){
    motordriver.stop();
   }
  
} //end loop




