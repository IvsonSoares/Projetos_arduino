//---------------------------------------
// Display
//---------------------------------------
#include <LiquidCrystal.h>

LiquidCrystal lcd_2(12,3,A3, A2,A1,A0);

//---------------------------------------
// Led NeoPixel
//---------------------------------------
#include <Adafruit_NeoPixel.h>

#define PIN 2	 // input pin Neopixel is attached to
#define NUMPIXELS      12 // number of neopixels in strip

Adafruit_NeoPixel pixels_2 = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//---------------------------------------
// Wire Connection
//---------------------------------------

#include <Wire.h>

#define MEU_ENDERECO 2
#define OUTRO_ENDERECO 1

bool aguardando_mensagem_2;

//---------------------------------------
// Keypad
//---------------------------------------

#include <Keypad.h>

char key_2; 
const byte ROWS_2 = 4; //four rows
const byte COLS_2 = 4; //three columns
char keys_2[ROWS_2][COLS_2] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins_2[ROWS_2] = {11, 10, 9, 8}; //connect to the row pinouts of the keypad
byte colPins_2[COLS_2] = {7, 6, 5, 4}; //connect to the column pinouts of the keypad


Keypad keypad_2 = Keypad( makeKeymap(keys_2), rowPins_2, colPins_2, ROWS_2, COLS_2 );


//Conexao Serial
char c_2;
String recData_2;

//Jogadas
char jogadas_2[9];
int pos_2 = 0;

//Jogadas individuais
bool jogadas_ind_2[9] = {false,false,false,
                         false,false,false,
                         false,false,false};

//Estados do Jogo
enum{
	START_2,
  	WIN_2,
  	LOSE_2,
  	DRAW_2

} estado_2 = START_2;

//Flag de Derrota
bool loseFinish_2 = false;
bool drawFinish_2 = false;


void setup()
{ 
  //NeoPixel led
  pixels_2.begin();
  
  Wire.begin(MEU_ENDERECO);
  Wire.onReceive(receiveEvent);
  Serial.begin(1156200);
  Serial.flush();
  //Serial.begin(9600); 
  
  //display
  lcd_2.begin(16, 2);
}

void espera_mensagem()
{
  aguardando_mensagem_2 = true;
  lcd_2.clear();
  lcd_2.setCursor(0, 0);
  lcd_2.print("Aguarde!");
  while (aguardando_mensagem_2) { delay(100); }
  lcd_2.clear();
}

void envia_mensagem(char key_2)
{
  delay(random(300, 700));
  Wire.beginTransmission(OUTRO_ENDERECO); // transmit to device #1
  Wire.write(key_2);              // sends one byte  
  Wire.endTransmission();    // stop transmitting
}

void loop()
{
  
  switch(estado_2){
  	case START_2:
      gameStart();
      break;
    case WIN_2:
      gameFinished();
      break;
    case LOSE_2:
     lcd_2.clear();
     lcd_2.setCursor(0, 0);
     lcd_2.print("Voce perdeu!");
   	 lcd_2.setCursor(0, 1);
     lcd_2.print("O jogo Acabou!");
     while(true){
      not_playing();
    	if(estado_2 == START_2){break;}
  	}
     break;
    case DRAW_2:
     drawGame();
     break;
  }
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany_2)
{
  for (int count = 1; count <= howMany_2; count++){ 
    char c_2 = Wire.read(); // receive byte as a character
    if(nonRepeat(c_2)){
    	setColor(c_2, 1);
      	aguardando_mensagem_2 = false;
      	youLose(c_2);
      	return;
    }   
  }
  
  aguardando_mensagem_2 = true;
}

// setColor()
// picks random values to set for RGB
void setColor(char key_2, int origem){
  
  int redColor_2 = 0;
  int greenColor_2 = 255;
  int blueColor_2 = 0;
 
  char charLed_2 = key_2 - '0';
  
  if(origem){
    redColor_2 = 255;
	greenColor_2 = 0;
	blueColor_2 = 0;
  }
  pixels_2.setPixelColor(filterCmd(charLed_2), pixels_2.Color(redColor_2, greenColor_2, blueColor_2));

  // This sends the updated pixel color to the hardware.
  pixels_2.show();

}


//Verificar porque esta dando
//valores inconsistentes
bool nonRepeat(char key){
  //Serial.println(key + '9');

  for(int i = 0; i < sizeof(jogadas_2); i++){
    if(jogadas_2[i] == key){
      lcd_2.clear();
      lcd_2.setCursor(0, 1);
      lcd_2.print("Jogada Invalida");
      return false;
    }
  }
  if(pos_2 < 9){
  	jogadas_2[pos_2] = key;
    pos_2 = pos_2 + 1;
    return true;
  }
  return false;
  
}

//Essa funcao verifica a validade da entrada
//fornecida pelo keypad
bool validKey(char key){
  int keyVal = key - '0';
  switch(keyVal){
  	case 1 ... 9:
    	return true;
  }
  
  return false;
}


//Essa funcao seta os comandos na ordem visual
int filterCmd(int cmd){
  int cmdFinal = 12;
  //Serial.println(cmd);
  switch(cmd){
  	case 1:
    	//8
    	cmdFinal = 8;
    	break;
    case 2:
    	//9
       	cmdFinal = 9;
    	break;
    case 3:
       //10
        cmdFinal = 10;
    	break;
    case 4:
       //4
    	cmdFinal = 4;
    	break;
    case 5:
       //5
        cmdFinal = 5;
    	break;
    case 6:
       //6
    	cmdFinal = 6;
    	break;
    case 7:
      //1
    	cmdFinal = 0;
    	break;
    case 8:
       //2
    	cmdFinal = 1;
    	break;
    case 9:
     	//3
    	cmdFinal = 2;
    	break;
  }
  
  return cmdFinal;

}

//Essa funcao checa as possiveis condicoes de vitoria
bool check(bool keys[9]){
  	bool win = false;
	//row
  	//000 000 111
  	//000 111 000
  	//111 000 000
  	if(keys[8] && keys[7] && keys[6]){win = true;}
  	if(keys[5] && keys[4] && keys[3]){win = true;}
  	if(keys[2] && keys[1] && keys[0]){win = true;}
  	//col
  	//100 100 100
  	//010 010 010
  	//001 001 001
  	if(keys[0] && keys[3] && keys[6]){win = true;}
  	if(keys[1] && keys[4] && keys[7]){win = true;}
  	if(keys[2] && keys[5] && keys[8]){win = true;}
  	//diag
  	//100 010 001
  	//001 010 100
  	if(keys[0] && keys[4] && keys[8]){win = true;}
  	if(keys[2] && keys[4] && keys[6]){win = true;}
  
  	return win;
}

void gameStart(){
  if(loseFinish_2){estado_2 = LOSE_2;}
  if(drawFinish_2){estado_2 = DRAW_2;}

  key_2 = keypad_2.getKey();
  lcd_2.setCursor(0, 0);
  lcd_2.print("Sua vez!");
  if (pos_2 == 9) { 
     	//drawFinish_2 = true;
        estado_2 = DRAW_2;
      	setColor(key_2, 0);
      	envia_mensagem(key_2);
   		return;
    }
  if(key_2 && validKey(key_2) && nonRepeat(key_2)){
    jogadas_ind_2[key_2 - '1'] = true;
    if(check(jogadas_ind_2)){
      estado_2 = WIN_2;
      //NeoPixel
  	  setColor(key_2, 0);
      envia_mensagem(key_2);
      return;
    }
   
     envia_mensagem(key_2);
     //NeoPixel
  	 setColor(key_2, 0);
 	 espera_mensagem();  
  }  
}

void gameFinished(){
    lcd_2.clear();
    lcd_2.setCursor(0, 0);
    lcd_2.print("Voce Ganhou!");
    lcd_2.setCursor(0, 1);
    lcd_2.print("O jogo Acabou!");
  
  	//Enviar Mensagem que o outro perdeu
  	char win = 'Y';
    //char draw = 'E';
    envia_mensagem(win);
  	while(true){
      not_playing();
    	if(estado_2 == START_2){break;}
  	}
}


//Essa funcao verifica se o adversario
//envio a flag de vitoria
void youLose(char lose){
   //condicao de derrota
  if(lose == 'Y'){loseFinish_2 = true;}
  if(lose == 'E'){drawFinish_2 = true;}
  if(lose == '#'){restart_game();}
}

void drawGame(){
	lcd_2.clear();
    lcd_2.setCursor(0, 0);
    lcd_2.print("Jogo Empatado!");
    lcd_2.setCursor(0, 1);
    lcd_2.print("O jogo Acabou!");
  
  	//Enviar Mensagem que o outro empatou
    char draw = 'E';
    envia_mensagem(draw);
  	while(true){
      not_playing();
    	if(estado_2 == START_2){break;}
  	}
	
}

bool not_playing(){
	key_2 = keypad_2.getKey();
  	if(key_2 == '#'){
  		restart_game();
        envia_mensagem('#');
      	return true;
    }
  return false;
}

void restart_game(){
  for(int i = 0; i<12; i++){
  	pixels_2.setPixelColor(i,pixels_2.Color(0,0,0));
    if(i<9){
    	jogadas_ind_2[i] = false;
     	jogadas_2[i] = '*';
    }
  }
  lcd_2.clear();
  pos_2 = 0;
  loseFinish_2 = false;
  drawFinish_2 = false;
  estado_2 = START_2;
  aguardando_mensagem_2 = false;
  pixels_2.show();
}