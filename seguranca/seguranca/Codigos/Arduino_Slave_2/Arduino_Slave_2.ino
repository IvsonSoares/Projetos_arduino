// C++ code
//
//Wire
#include <Wire.h>
#define ID_SLAVE 2
//servo motor
#include <Servo.h>

Servo myservo;
int value;
int angle = 360;
bool fecha_portao = false;
bool abre_portao = true;
const int PORTA_PIEZO = 12;
// FREQUÊNCIAS DOS TONS EM HERTZ

#define NOTE_B2  123
#define NOTE_A4  440
#define NOTE_F6  1397
#define NOTE_D8  4699

enum { LIGADO, DESLIGADO} modo = DESLIGADO;

void setup()
{
  Wire.begin(ID_SLAVE);
  Wire.onReceive(receberComando);
  pinMode(PORTA_PIEZO, OUTPUT);
  Serial.begin(9600);
  myservo.attach(A0);
}

void loop()
{
  switch(modo)
    {
    	case LIGADO: modo_ligado(); break;
      	case DESLIGADO: modo_desligado(); break;
    }
}

void modo_ligado()
{
  soa_alarme();
  //servo motor
  if(fecha_portao){
    for (int i = 0; i < angle; i = i + 4) { 
      myservo.write(i);                    
    } 
    fecha_portao = false;
  }
}

void modo_desligado()
{
  if(abre_portao){
    for (int i = 0; i < angle; i = i + 4) { 
      myservo.write(360-i);                    
    } 
    abre_portao = false;
  }
  	noTone(PORTA_PIEZO);
  	delay(500);
}

void soa_alarme(){
  	tone(PORTA_PIEZO, NOTE_B2);
  	delay(500);
  	noTone(PORTA_PIEZO);
}

void receberComando(int numBytesRecebidos)
{
  char comando = Wire.read();
  Serial.print("Recebido comando: ");
  if (comando == 'L'){
    modo = LIGADO;
    fecha_portao = true;
  }
  if(comando =='D'){
    abre_portao = true;
    modo = DESLIGADO;}
}
