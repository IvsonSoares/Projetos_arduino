#include <IRremote.h>
#include <Wire.h>

int RECV_PIN = 12;

#define MEU_ENDERECO 3


double senha[3] = {4010852096,3994140416,3977428736};
int senhaPasso = 0;

const int infra = 12;

IRrecv irrecv(RECV_PIN);
decode_results results;
bool Aguardando_Mensagem;
bool Ativado_Controle;

enum {
  		Alarme_Desligado,
  		Alarme_Ligado,
  		Esperando_Senha,
  		Alarme_Disparado
	  } estado = Alarme_Desligado;

String estado_anterior;

void setup()
{
  pinMode(4,OUTPUT);
  Wire.begin(MEU_ENDERECO);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  irrecv.enableIRIn();
}

void loop()
{
  switch (estado)
  {
    case Alarme_Desligado: alarme_desligado(); break;
    case Alarme_Ligado: alarme_ligado(); break;
    case Esperando_Senha: esperando_senha(); break;
    case Alarme_Disparado: alarme_disparado(); break; 
  }
}

void alarme_desligado()
{
  digitalWrite(4,LOW);
  if(Ativado_Controle) transmitirDado('C',1);
  transmitirDado('D',1);
        
  estado_anterior = "Desligado";
  Aguardando_Mensagem = true;
  while (Aguardando_Mensagem) {
    recebe_controle();
  }
}

void recebe_controle(){
	if (IrReceiver.decode()) {
      double dado = IrReceiver.decodedIRData.decodedRawData;
      if(dado == 4244815616){
        if(Ativado_Controle) transmitirDado('F',1);
        if(!Ativado_Controle) transmitirDado('C',1);
      		Ativado_Controle = !Ativado_Controle;
      }
   		if(dado == 4278238976){
          estado = Esperando_Senha;
          Aguardando_Mensagem = false;
          Serial.println("Esperando Senha:");
        }
    irrecv.resume(); //AGUARDA O RECEBIMENTO DE UM NOVO SINAL IR
  }
}

void esperando_senha(){

  if (IrReceiver.decode()) {
      double dado = IrReceiver.decodedIRData.decodedRawData;
      if(dado != 0){
      if(senha[senhaPasso] == dado){
          
      senhaPasso += 1;
      }else{
      senhaPasso = 0;
      }
        
      if(senhaPasso == 3){
      //Senha certa
        if(estado_anterior == "Ligado"){ estado = Alarme_Desligado;Serial.println("Desligando");}
        if(estado_anterior == "Desligado"){estado = Alarme_Ligado;Serial.println("Ligando");}
          if(estado_anterior == "Disparado")
        	{ 
              Ativado_Controle = true;
              transmitirDado('D',2);
              transmitirDado('C',1);
              transmitirDado('D',1);
         	  estado = Alarme_Desligado;
            }
 
      }
      }
    irrecv.resume(); 
  }
  delay(100);
}

void alarme_ligado()
{
  digitalWrite(4,HIGH);
  if(Ativado_Controle){
    transmitirDado('C',1);
  }
  transmitirDado('L',1);
        
  estado_anterior = "Ligado";
  Aguardando_Mensagem = true;
  while (Aguardando_Mensagem) {
    recebe_controle();
  }
}

void alarme_disparado()
{
  transmitirDado('L',2);
  estado_anterior = "Disparado";
  Aguardando_Mensagem = true;
  while (Aguardando_Mensagem) {
    recebe_controle();
  }
}


void receiveEvent(int howMany)
{
  Serial.println("Recebi");
  char Mensagem = Wire.read();
  if(Mensagem == 'N')estado = Alarme_Ligado;
  if(Mensagem == 'D')estado = Alarme_Desligado;
  if(Mensagem == 'P')estado = Alarme_Disparado;
  Aguardando_Mensagem = false;
}
  
void transmitirDado(byte dado,int slave)
{
    Serial.println(" (Transmitindo dado ...)");
    Wire.beginTransmission(slave);
    Wire.write(dado);  
    Wire.endTransmission();
}
  
    
  
  
  
  
  