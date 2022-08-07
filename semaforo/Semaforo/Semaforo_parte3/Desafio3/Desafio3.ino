int fluxo = 0;
int btn_pedestre_var = 0;
String c_verde_4 = "A0";
String c_amarelo_4 = "A1";
String c_vermelho_4 = "A2";
String c_verde_3 = "A3";
String c_amarelo_3 = "A4";
String c_vermelho_3 = "A5";
int c_verde_2 = 0;
int c_amarelo_2 = 1;
int c_vermelho_2 = 4;
int c_verde_1 = 5;
int c_amarelo_1 = 6;
int c_vermelho_1 = 7;
int p_verde_1 = 8;
int p_vermelho_1 = 9;
int p_verde_2 = 10;
int p_vermelho_2 = 11;
int p_verde_3 = 12;
int p_vermelho_3 = 13;

boolean botao;

void setup() {
  
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  pinMode(c_verde_2, OUTPUT);
  pinMode(c_amarelo_2, OUTPUT);
  pinMode(c_vermelho_2, OUTPUT);
  pinMode(c_verde_1, OUTPUT);
  pinMode(c_amarelo_1, OUTPUT);
  pinMode(c_vermelho_1, OUTPUT);
  pinMode(p_verde_1, OUTPUT);
  pinMode(p_vermelho_1, OUTPUT);
  pinMode(p_verde_2, OUTPUT);
  pinMode(p_vermelho_2, OUTPUT);
  pinMode(p_verde_3, OUTPUT);
  pinMode(p_vermelho_3, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), btn_pedestre_A, RISING);
  attachInterrupt(digitalPinToInterrupt(3), btn_pedestre_B, RISING);
}

void loop() {
  // put your main code here, to run repeatedly:
  
 
  
  switch(btn_pedestre_var){
    case 0:
    desliga(c_vermelho_1);
    analogWrite(A2,0);
    liga(p_vermelho_1);
    liga(p_vermelho_2);
    liga(p_vermelho_3);
    liga(c_verde_1);
    analogWrite(A0,255);
    liga(c_vermelho_2);
    analogWrite(A5,255);
    delay(5000);
    desliga(c_verde_1);
    analogWrite(A0,0);
    liga(c_amarelo_1);
    analogWrite(A1,255);
    delay(2500);
    desliga(c_amarelo_1);
    analogWrite(A1,0);
    liga(c_vermelho_1);
    analogWrite(A2,255);
   desliga(c_vermelho_2);
    analogWrite(A5,0);
    liga(c_verde_2);
    analogWrite(A3,255);
    liga(p_verde_3);
    desliga(p_vermelho_3);
    delay(5000);
    desliga(c_verde_2);
    analogWrite(A3,0);
    liga(c_amarelo_2);
    analogWrite(A4,255);
    desliga(p_verde_3);
    for(int i=0;i<7;i++){
      desliga(p_vermelho_3);
      delay(2500/12);
      liga(p_vermelho_3);
      delay(2500/12);
    }
    
    desliga(c_amarelo_2);
    analogWrite(A4,0);
    
    

      break;
    
    case 1:
    desliga(p_vermelho_1);
    analogWrite(A5,255);
    liga(c_vermelho_1);
    liga(c_verde_2);
    analogWrite(A0,255);
    analogWrite(A2,0);
    liga(p_verde_1);
    delay(5000);
    desliga(c_verde_2);
    liga(c_amarelo_2);
    desliga(p_verde_1);
    for(int i=0;i<7;i++){
      desliga(p_vermelho_1);
      delay(2500/12);
      liga(p_vermelho_1);
      delay(2500/12);
    }
    desliga(c_amarelo_2);
    liga(c_vermelho_2);
    desliga(c_vermelho_1);
  btn_pedestre_var=0;
      break;
  
    case 2:
    desliga(p_vermelho_2);
    liga(c_vermelho_2);
    analogWrite(A5,255);
    liga(c_verde_1);
    desliga(c_vermelho_1);
    liga(p_verde_2);
    delay(5000);
    desliga(p_verde_2);
    for(int i=0;i<7;i++){
      desliga(p_vermelho_2);
      delay(2500/12);
      liga(p_vermelho_2);
      delay(2500/12);
    }
 
    analogWrite(A2,0);
    btn_pedestre_var=0;
      break;
  }
  
}
  

void btn_pedestre_A(){
  btn_pedestre_var = 1;
}
void btn_pedestre_B(){
  btn_pedestre_var = 2;
}
void liga(int i){
  digitalWrite(i,HIGH);
}

void desliga(int i){
    digitalWrite(i,LOW);
}

          
 
