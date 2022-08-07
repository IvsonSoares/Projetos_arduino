//---------------------------------------
// Display
//---------------------------------------
#include <LiquidCrystal.h>

LiquidCrystal lcd_1(12,3,A3, A2,A1,A0);

//---------------------------------------
// Led NeoPixel
//---------------------------------------
#include <Adafruit_NeoPixel.h>

#define PIN 2	 // input pin Neopixel is attached to
#define NUMPIXELS      12 // number of neopixels in strip

Adafruit_NeoPixel pixels_1 = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int redColor_1 = 255;
int greenColor_1 = 0;
int blueColor_1 = 0;

//---------------------------------------
// Wire Connection
//---------------------------------------

#include <Wire.h>

#define MEU_ENDERECO 1
#define OUTRO_ENDERECO 2

bool aguardando_mensagem_1;

//---------------------------------------
// Keypad Test
//---------------------------------------

#include <Keypad.h>

char key_1; 
const byte ROWS_1 = 4; //four rows
const byte COLS_1 = 4; //three columns
char keys_1[ROWS_1][COLS_1] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins_1[ROWS_1] = {11, 10, 9, 8}; //connect to the row pinouts of the keypad
byte colPins_1[COLS_1] = {7, 6, 5, 4}; //connect to the column pinouts of the keypad

Keypad keypad_1 = Keypad( makeKeymap(keys_1), rowPins_1, colPins_1, ROWS_1, COLS_1 );


//Conexao Serial
char c_1;
String recData_1;

// Lista de Jogadas
char jogadas_1[9];
int pos_1 = 0;

// Lista de Jogada Individual;
bool jogadas_ind_1[9] = {false,false,false,
                         false,false,false,
                         false,false,false};

//Estados do Jogo
enum{
	START_1,
  	WIN_1,
  	LOSE_1,
  	DRAW_1

} estado_1 = START_1;

//Flag de Derrota
bool loseFinish_1 = false;
bool drawFinish_1 = false;


void setup()
{ 
  // Initialize the NeoPixel library.
  pixels_1.begin();

  Wire.begin(MEU_ENDERECO);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600); 
  
  //Display
  lcd_1.begin(16, 2);
}

void espera_mensagem()
{
  aguardando_mensagem_1 = true;
  lcd_1.clear();
  lcd_1.setCursor(0, 0);
  lcd_1.print("Aguarde!");
  while (aguardando_mensagem_1){ delay(100); }
  lcd_1.clear();
}

void envia_mensagem(char key_1)
{
  delay(random(300, 700));
  Wire.beginTransmission(OUTRO_ENDERECO); // transmit to device #1
  Wire.write(key_1);              // sends one byte  
  Wire.endTransmission();    // stop transmitting
}

void loop()
{
  
  switch(estado_1){
  	case START_1:
      gameStart();
      break;
    case WIN_1:
      gameFinished();
      break;
    case LOSE_1:
      lcd_1.clear();
      lcd_1.setCursor(0, 0);
      lcd_1.print("Voce perdeu!");
      lcd_1.setCursor(0, 1);
      lcd_1.print("O jogo acabou!");
      while(true){
     	not_playing();
    	if(estado_1 == START_1){break;}
  	}
      break;
    case DRAW_1:
     drawGame();
     break;
  }
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany_1)
{
  for (int count = 1; count <= howMany_1; count++){ 
    char c_1 = Wire.read(); // receive byte as a character
    if(nonRepeat(c_1)){
    	//Serial.print(c_1);         // print the character
    	setColor(c_1, 1);
      	aguardando_mensagem_1 = false;
      	youLose(c_1);
      	return;
    }   
  }
  
  aguardando_mensagem_1 = true;
}


//Essa funcao mostra nas fitas de led a jogada selecionada
void setColor(char key_1, int origem){
  
  int redColor_1 = 255;
  int greenColor_1 = 0;
  int blueColor_1 = 0;
  
  if(origem){
  	redColor_1 = 0;
    greenColor_1 = 255;
    blueColor_1 = 0;
  }
  
  char charLed_1 = key_1 - '0';
  
  pixels_1.setPixelColor(filterCmd(charLed_1), pixels_1.Color(redColor_1, greenColor_1, blueColor_1));

  // This sends the updated pixel color to the hardware.
  pixels_1.show();

}


//Essa evita repetir a mesma jogada
//Verificar inconsistencia de valores
bool nonRepeat(char key){
  //int keyValue = key - '1';
  for(int i = 0; i < sizeof(jogadas_1); i++){
    if(jogadas_1[i] == key){
      lcd_1.clear();
      lcd_1.setCursor(0, 1);
      lcd_1.print("Jogada Invalida!");
      return false;
    }
  }
  if(pos_1 <= 9){
  	jogadas_1[pos_1] = key;
    pos_1 = pos_1 + 1;
    return true;
  }
  return false;
  
}


//essa funcao verifica se a entrada
//do keypad eh valida para o jogo
bool validKey(char key){
  int keyVal = key - '0';
  switch(keyVal){
  	case 1 ... 9:
    	return true;
  }
  
  return false;
}


//essa funcao reordena a entrada
// de acordo com a posicao dos botoes com
// os leds da fita
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
  if(loseFinish_1){estado_1 = LOSE_1;}  
  if(drawFinish_1){estado_1 = DRAW_1;}
  
  key_1 = keypad_1.getKey();
    //lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("Sua vez!");
  	if(pos_1 == 9){
        //Serial.print(pos_1);
        estado_1 = DRAW_1;
        setColor(key_1, 0);
		envia_mensagem(key_1);
      	//drawFinish_1 = true;
        return;
      }
  	//lcd_1.clear();
    if(key_1 && validKey(key_1) && nonRepeat(key_1)){
       jogadas_ind_1[key_1 - '1'] = true;
      Serial.println(pos_1);
       if(check(jogadas_ind_1)){
          estado_1 = WIN_1;
          //NeoPixel
  		  setColor(key_1, 0);
          envia_mensagem(key_1);
          return;
      }
      envia_mensagem(key_1);
      //NeoPixel
      setColor(key_1, 0);
 	  espera_mensagem(); 
  }
}


//Essa funcao envia a flag de vitoria para o adversario
void gameFinished(){
    lcd_1.clear();
  	lcd_1.setCursor(0, 0);
    lcd_1.print("Voce Ganhou!!");
    lcd_1.setCursor(0, 1);
    lcd_1.print("O jogo Acabou!");
  	//Enviar Mensagem que o outro perdeu
  	char win = 'Y';
    envia_mensagem(win);
  	while(true){
      not_playing();
    	if(estado_1 == START_1){break;}
  	}
}

//Essa funcao verifica se o adversario
//envio a flag de vitoria
void youLose(char lose){
   //condicao de derrota
  if(lose == 'Y'){loseFinish_1 = true;}
  if(lose == 'E'){drawFinish_1 = true;}
  if(lose == '#'){restart_game();}
}


void drawGame(){
	lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("Jogo Empatado!");
    lcd_1.setCursor(0, 1);
    lcd_1.print("O jogo Acabou!");
  
  	//Enviar Mensagem que o outro empatou
    char draw = 'E';
    envia_mensagem(draw);
	 while(true){
      not_playing();
    	if(estado_1 == START_1){break;}
  	}
}
bool not_playing(){
	key_1 = keypad_1.getKey();
  	if(key_1 == '#'){
  		restart_game();
        envia_mensagem('#');
      	return true;
    }
  return false;
}

void restart_game(){
  for(int i = 0; i<12; i++){
  	pixels_1.setPixelColor(i,pixels_1.Color(0,0,0));
    if(i<9){
    	jogadas_ind_1[i] = false;
      jogadas_1[i] = '*';
    }
  }
  lcd_1.clear();
  pos_1 = 0;
  loseFinish_1 = false;
  drawFinish_1 = false;
  estado_1 = START_1;
  aguardando_mensagem_1 = false;
  pixels_1.show();
}