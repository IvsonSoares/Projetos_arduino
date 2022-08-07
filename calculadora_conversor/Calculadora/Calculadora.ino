#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal.h>
enum {
    CALCULADORA_D,
    CALCULADORA_B,
    CALCULADORA_O,
    b_d,
    d_b,
    d_o,
    o_d
  	} estado = CALCULADORA_D;

LiquidCrystal lcd(A0,A1,A2,A3,A4,A5);
boolean mudou = true;
long first = 0;
long second = 0;
double total = 0;
int coluna = 0;

char customKey;
const byte ROWS = 4;
const byte COLS = 4;

int botaomuda = 2;

char keys[ROWS][COLS] = {
  {'1','2','3','+'},
  {'4','5','6','-'},
  {'7','8','9','*'},
  {'C','0','=','/'}
};
byte rowPins[ROWS] = {12,11,10,9}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8,7,6,5}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); 


void setup()
{
  pinMode(botaomuda, INPUT_PULLUP);
  attachInterrupt(botaomuda - 2 , muda, FALLING);
lcd.begin(16, 2);               // start lcd
lcd.setCursor(0,0);
  lcd.print("Experiencia");
  lcd.setCursor(0,1);
  lcd.print("Criativa");
delay(4000);
lcd.clear();
lcd.setCursor(0, 0);
  Serial.begin(9600);
}


void loop()
{

  switch(estado)
  {
    case CALCULADORA_D: calculadora_D(); break;
    case CALCULADORA_B: calculadora_B(); break;
    case CALCULADORA_O: calculadora_O(); break;
    case b_d: B_D(); break;
    case d_b: D_B(); break;
    case d_o: D_O(); break;
    case o_d: O_D(); break;
    default: Serial.println("Estado invalido");
  }
}	

void calculadora_D(){
  lcd.setCursor(0,0);
  lcd.print("CALCULADORA DECIMAL");
  customKey = customKeypad.getKey();
  lcd.setCursor(coluna,1);
  switch(customKey) 
  {
  case '0' ... '9': // This keeps collecting the first value until a operator is pressed "+-*/"
    if(coluna==0)lcd.clear();
    if(coluna==3)break;
    lcd.setCursor(0,1);
    first = first * 10 + (customKey - '0');
    lcd.print(first);
    coluna++;
    break;

  case '+':
    lcd.print("+");
    coluna++;
    lcd.setCursor(coluna,1);
    second = SecondNumber(); // get the collected the second number
    if(second != '1'){
    total = first + second;
    imprimeigual();
    lcd.print(total);
    }
    coluna = 0;
    first = 0, second = 0, total=0; // reset values back to zero for next use
    break;

  case '-':
    lcd.print("-");
    coluna++;
    lcd.setCursor(coluna,1);
    second = SecondNumber();
    if(second != '1'){
    total = first - second;
      imprimeigual();
    lcd.print(total);
    }
    coluna = 0;
    first = 0, second = 0, total=0;
    break;

  case '*':
    lcd.print("*");
    coluna++;
    lcd.setCursor(coluna,1);
    second = SecondNumber();
    if(second != '1'){
    total = first * second;
    imprimeigual();
    lcd.print(total);
    }
    coluna = 0;
    first = 0, second = 0, total=0;
    break;

  case '/':
    lcd.print("/");
    coluna++;
    lcd.setCursor(coluna,1);
    second = SecondNumber();
    if(second != '1'){
    lcd.setCursor(coluna,1);
	
      if(second == 0){
        lcd.print("Invalid");
        first=0,second=0;
        coluna=0;
        break;
      }
    total = (float)first / (float)second;
    imprimeigual();
    lcd.print(total);
    }
    coluna=0;
    first = 0, second = 0, total=0;
    break;

  case 'C':
    total = 0;
    first = 0;
    coluna = 0;
    lcd.clear();
    break;
  }
}

void calculadora_B(){
  lcd.setCursor(0,0);
  lcd.print("CALCULADORA BINARIA");
  customKey = customKeypad.getKey();
  lcd.setCursor(coluna,1);
  switch(customKey){
  	case '0' ... '1':
    	if(coluna==0)lcd.clear();
    	if(coluna==3)break;
    	lcd.setCursor(0,1);
    	first = first * 10 + (customKey - '0');
    	lcd.print(first);
    	coluna++;
    	break;
    
  case '+':
    lcd.print("+");
    coluna++;
    lcd.setCursor(coluna,1);
    second = SecondNumber(); // get the collected the second number
    if(second != '1'){
    //soma
    first = bintodec(first);
      
    second = bintodec(second);
    total = first + second;
    imprimeigual();
    lcd.print(dectobin(total));
    }
    coluna = 0;
    first = 0, second = 0, total=0; // reset values back to zero for next use
    break;

  case '-':
    lcd.print("-");
    coluna++;
    lcd.setCursor(coluna,1);
    second = SecondNumber();
    if(second != '1'){
    first = bintodec(first);
    second = bintodec(second);
    total = first - second;
    imprimeigual();
    lcd.print(dectobin(total));
    }
    coluna = 0;
    first = 0, second = 0, total=0;
    break;

  case '*':
    lcd.print("*");
    coluna++;
    lcd.setCursor(coluna,1);
    second = SecondNumber();
    if(second != '1'){
    first = bintodec(first);
    second = bintodec(second);
    total = first * second;
    imprimeigual();
    lcd.print(dectobin(total));
    }
    coluna = 0;
    first = 0, second = 0, total=0;
    break;

  case '/':
    lcd.print("/");
    coluna++;
    lcd.setCursor(coluna,1);
    second = SecondNumber();
    if(second != '1'){
    lcd.setCursor(coluna,1);
	
      if(second == 0){
        lcd.print("Invalid");
        first=0,second=0;
        coluna=0;
        break;
      }
    first = bintodec(first);
    second = bintodec(second);
    total = (float)first / (float)second;
    imprimeigual();
    lcd.print(dectobin(total));
    }
    coluna=0;
    first = 0, second = 0, total=0;
    break;

  case 'C':
    total = 0;
    first = 0;
    coluna = 0;
    lcd.clear();
    break;
    
    
  }
 
}

void calculadora_O(){
  lcd.setCursor(0,0);
  lcd.print("CALCULADORA OCT");
    customKey = customKeypad.getKey();
  lcd.setCursor(coluna,1);
  switch(customKey){
  	case '0' ... '7':
    	if(coluna==0)lcd.clear();
    	if(coluna==3)break;
        if(first==0)coluna=0;
    	lcd.setCursor(0,1);
    	first = first * 10 + (customKey - '0');
    	lcd.print(first);
    	coluna++;
    	break;
    
  case '+':
    lcd.print("+");
    coluna++;
    lcd.setCursor(coluna,1);
    second = SecondNumber(); // get the collected the second number
    if(second != '1'){
    //soma
    first = octtodec(first);
    second = octtodec(second);
    total = first + second;
    imprimeigual();
    lcd.print(dectooct(total));
    }
    coluna = 0;
    first = 0, second = 0, total=0; // reset values back to zero for next use
    break;

  case '-':
    lcd.print("-");
    coluna++;
    lcd.setCursor(coluna,1);
    second = SecondNumber();
    if(second != '1'){
    first = octtodec(first);
    second = octtodec(second);
    total = first - second;
    imprimeigual();
    lcd.print(dectooct(total));
    }
    coluna = 0;
    first = 0, second = 0, total=0;
    break;

  case '*':
    lcd.print("*");
    coluna++;
    lcd.setCursor(coluna,1);
    second = SecondNumber();
    if(second != '1'){    
    first = octtodec(first);
    second = octtodec(second);
    total = first * second;
    imprimeigual();
    lcd.print(dectooct(total));
    }
    coluna = 0;
    first = 0, second = 0, total=0;
    break;

  case '/':
    lcd.print("/");
    coluna++;
    lcd.setCursor(coluna,1);
    second = SecondNumber();
    if(second != '1'){
    lcd.setCursor(coluna,1);
	
      if(second == 0){
        lcd.print("Invalid");
        first=0,second=0;
        coluna=0;
        break;
      }
    first = octtodec(first);
    second = octtodec(second);
    total = (float)first / (float)second;
    imprimeigual();
    lcd.print(dectooct(total));
    }
    coluna=0;
    first = 0, second = 0, total=0;
    break;

  case 'C':
    total = 0;
    first = 0;
    coluna = 0;
    lcd.clear();
    break;
    
    
  }
  
}

void D_B(){
  lcd.setCursor(0,0);
  lcd.print("DEC PARA BIN");
  customKey = customKeypad.getKey();
  lcd.setCursor(coluna,1);
  switch(customKey){
  	case '0' ... '9':
    	if(coluna==0)lcd.clear();
    	if(coluna==3)break;
    	lcd.setCursor(0,1);
    	first = first * 10 + (customKey - '0');
    	lcd.print(first);
    	coluna++;
    	break;
    case 'C':
        first = 0;
        coluna = 0;
        lcd.clear();
        break;
    case '=':
    	imprimeigual();
        lcd.print(dectobin(first));
        coluna=0;
        first=0;
    	break;
    	
  }
}

void B_D(){
  lcd.setCursor(0,0);
  lcd.print("BIN PARA DEC");
  customKey = customKeypad.getKey();
  lcd.setCursor(coluna,1);
  switch(customKey){
  	case '0' ... '1':
    	if(coluna==0)lcd.clear();
    	if(coluna==3)break;
    	lcd.setCursor(0,1);
    	first = first * 10 + (customKey - '0');
    	lcd.print(first);
    	coluna++;
    	break;
    case 'C':
        first = 0;
        coluna = 0;
        lcd.clear();
        break;
    case '=':
    	imprimeigual();
        lcd.print(bintodec(first));
        coluna=0;
        first=0;
    	break;
    	
  }
 
}

void D_O(){
  lcd.setCursor(0,0);
  lcd.print("DEC PARA OCT");
    customKey = customKeypad.getKey();
  lcd.setCursor(coluna,1);
  switch(customKey){
  	case '0' ... '9':
    	if(coluna==0)lcd.clear();
    	if(coluna==3)break;
    	lcd.setCursor(0,1);
    	first = first * 10 + (customKey - '0');
    	lcd.print(first);
    	coluna++;
    	break;
    case 'C':
        first = 0;
        coluna = 0;
        lcd.clear();
        break;
    case '=':
    	imprimeigual();
        lcd.print(dectooct(first));
        coluna=0;
        first=0;
    	break;
    	
  }
}

void O_D(){
  lcd.setCursor(0,0);
  lcd.print("OCT PARA DEC");
    customKey = customKeypad.getKey();
  lcd.setCursor(coluna,1);
  switch(customKey){
  	case '0' ... '9':
    	if(coluna==0)lcd.clear();
    	if(coluna==3)break;
    	lcd.setCursor(0,1);
    	first = first * 10 + (customKey - '0');
    	lcd.print(first);
    	coluna++;
    	break;
    case 'C':
        first = 0;
        coluna = 0;
        lcd.clear();
        break;
    case '=':
    	imprimeigual();
        lcd.print(octtodec(first));
        coluna=0;
        first=0;
    	break; 	
  }
}
void muda(){
  mudou = false;
  lcd.clear();
  first=0;
  coluna=0;
  second=0;
  total=0;
  if(estado == CALCULADORA_D){
    estado =CALCULADORA_B;
    return;
  }
  if(estado == CALCULADORA_B){
    estado = CALCULADORA_O;
    return;
  }
    if(estado == CALCULADORA_O){
    estado = d_b;
    return;
  }
    if(estado == d_b){
    estado = b_d;
    return;
  }
  	if(estado == b_d){
    estado = d_o;
    return;
  }
  	if(estado == d_o){
    estado = o_d;
    return;
  }
    if(estado == o_d){
    estado = CALCULADORA_D;
    return;
  }
}
long SecondNumber()
{
  mudou = true;
  while( mudou )
  {
    customKey = customKeypad.getKey();
    if(customKey >= '0' && customKey <= '7' && estado==CALCULADORA_O)
    {
      if(second < 100){
      second = second * 10 + (customKey - '0');
      lcd.print(customKey);
      coluna++;
      }
    }
    if(customKey >= '0' && customKey <= '1' && estado==CALCULADORA_B)
    {
      if(second < 100){
      second = second * 10 + (customKey - '0');
      lcd.print(customKey);
      coluna++;
      }
    }
    if(customKey >= '0' && customKey <= '9' && estado==CALCULADORA_D)
    {
      if(second < 100){
      second = second * 10 + (customKey - '0');
      lcd.print(customKey);
      coluna++;
      }
    }
    if(customKey == 'C'){
      lcd.clear();
      return '1';
    }
    if(customKey == '=') break;  //return second;
  }
 return second; 
}
int bintodec(int bin){
  int dec = 0;	
  int tamanho = 0;
  if(bin>=100)tamanho=2;
  else if(bin>=10)tamanho=1;
    
  for(int i = 0; i<=tamanho; i++){
    char oi = String(bin)[i];
    if(oi == '1'){
        
    	dec+= round(pow(2,(tamanho-i)));
    }
  }
  return dec;
}

String dectobin(int dec){
String bin = "";
  if ( dec/2 !=0){
     bin = dectobin( dec/2);
  }
  bin += dec%2;
  return bin; 
}
String dectooct(int dec){
String oct = "";
  if ( dec/8 !=0){
     oct = dectooct( dec/8);
  }
  oct += dec%8;
  return oct;
}

int octtodec(int oct){
  int dec = 0;	
  int tamanho = 0;
  if(oct>=100)tamanho=2;
  else if(oct>=10)tamanho=1;
  for(int i = 0; i<=tamanho; i++){
		int oi = String(oct)[i] - '0';
    	dec += oi*(round(pow(8,(tamanho-i))));
  }
  return dec;
}   
void imprimeigual(){
    lcd.setCursor(coluna,1);
    lcd.print("=");
    coluna++;
    lcd.setCursor(coluna,1);
}