#include <IRremote.h>
#include <Wire.h>

#define MEU_ENDERECO 1

bool Ativado_Controle;

enum{VERIF_TMP, VERIF_PRES, ESPERA}
estado = VERIF_TMP;

bool aguardandoMensagem = false;
const int ldr = A0;
const int sensor = 12;

void setup()
{
  Wire.begin(MEU_ENDERECO);
  Wire.onReceive(receiveEvent);
  pinMode(ldr,INPUT);
  Serial.begin(9600);
}

void loop()
{
switch(estado){
    case VERIF_TMP:verificaTempo(); break;
    case VERIF_PRES:verificaPresenca(); break;
    case ESPERA:espera(); break;
}
}  
  void verificaTempo(){
    if(Ativado_Controle)return;
    if(analogRead(ldr) > 680){
      transmitirDado('N',3);
      aguardandoMensagem = true;
      while(aguardandoMensagem){}
    }
  }
  
  void verificaPresenca(){
    delay(100);
    pinMode(sensor, OUTPUT);
    digitalWrite(sensor,LOW);
    delayMicroseconds(2);
    digitalWrite(sensor,HIGH);
    delayMicroseconds(10);
    digitalWrite(sensor,LOW);
    pinMode(sensor,INPUT);
    if(pulseIn(sensor,HIGH)>19440){
      if(Ativado_Controle)return;
      if(analogRead(ldr) < 680){
        transmitirDado('D',3);
      estado = VERIF_TMP;
      }
     
    }else{
      transmitirDado('P',3);
    estado = ESPERA;
    }
  }
  
  void espera(){
  aguardandoMensagem = true;
    while(aguardandoMensagem){
    
    }
  } 


void receiveEvent(int howMany)
{
  Serial.println("Recebi");
  char Mensagem = Wire.read();
  if(Mensagem == 'L') estado = VERIF_PRES;
  if(Mensagem == 'D') estado = VERIF_TMP;
  if(Mensagem == 'C') Ativado_Controle = true;
  if(Mensagem == 'F') Ativado_Controle = false;
  aguardandoMensagem = false;
}

void transmitirDado(byte dado,int slave)
{
    Serial.println(" (Transmitindo dado ...)");
    Wire.beginTransmission(slave);
    Wire.write(dado);  
    Wire.endTransmission();
}
  
    
  