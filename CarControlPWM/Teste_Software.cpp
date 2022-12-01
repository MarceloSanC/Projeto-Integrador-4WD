#include "Teste_Software.h"

Teste::Teste(){
  cont_entrada=20;
  cont_repeticao=0;  
}

Teste::~Teste(){}

int Teste::entradas_Ordem(){
  
  if(cont_repeticao<REP_MAX){
    cont_repeticao++;
    return cont_entrada;
  }

  else if(cont_entrada<80){
    cont_entrada++;
    cont_repeticao=0;
  }
  else{
    cont_entrada=20;
    cont_repeticao=0;
  }


}

int Teste::entradas_Aleatorio(){
  return random(0, 100);
}
