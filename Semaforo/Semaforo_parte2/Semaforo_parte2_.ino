int fluxo = 0;
int btn_pedestre_var = 0;
int c_verde_2 = 6;
int c_amarelo_2 = 7;
int c_vermelho_2 = 8;
int c_verde_1 = 9;
int c_amarelo_1 = 10;
int c_vermelho_1 = 11;
int p_verde = 12;
int p_vermelho = 13;

float G_total = 10;
float G_1;
float G_2;
float f;

boolean botao;

void setup() {

  Serial.begin(9600);
  pinMode(A0, INPUT); 
  
  pinMode(c_verde_2, OUTPUT);
  pinMode(c_amarelo_2, OUTPUT);
  pinMode(c_vermelho_2, OUTPUT);
  pinMode(c_verde_1, OUTPUT);
  pinMode(c_amarelo_1, OUTPUT);
  pinMode(c_vermelho_1, OUTPUT);
  pinMode(p_verde, OUTPUT);
  pinMode(p_vermelho, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), btn_pedestre, RISING);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
 
  
  switch(btn_pedestre_var){
    case 0:
    digitalWrite(p_vermelho,1);
    fluxo = analogRead(A0);
    Serial.println(fluxo);
    if(fluxo>=342){
      f = map( fluxo, 342, 1023, 500, 667) / 1000.0;
       G_1 = G_total * f;
      G_2 = G_total-G_1;
  }
  else{
    f = map( fluxo, 0, 341, 200, 500) / 1000.0;
    G_1 = G_total * f;
    G_2 = G_total-G_1;
    }
    Serial.print("G_1:");
    Serial.println(G_1);
    Serial.print("G_2:");
    Serial.println(G_2);
    digitalWrite(c_vermelho_1, HIGH);
    digitalWrite(c_verde_2, HIGH);
    delay(G_2*1000);
    digitalWrite(c_verde_2, LOW); 
    digitalWrite(c_amarelo_2,  HIGH);
    delay(1000);
    digitalWrite(c_amarelo_2, LOW); 
    digitalWrite(c_vermelho_2, HIGH); 
    delay(1000);
    digitalWrite(c_vermelho_1, LOW);
    digitalWrite(c_verde_1, HIGH);
    delay(G_1*1000);
     digitalWrite(c_verde_1, LOW); 
    digitalWrite(c_amarelo_1,  HIGH);
    delay(1000);
    digitalWrite(c_amarelo_1, LOW); 
    digitalWrite(c_vermelho_1, HIGH); 
    delay(1000);
    digitalWrite(c_vermelho_2, LOW);  
    
      break;
    
    case 1:
      digitalWrite(p_vermelho,0);
      digitalWrite(p_verde,1);
      digitalWrite(c_vermelho_2,1);
    delay(8000);
    digitalWrite(p_verde,0);
    for(int i=0;i<7;i++){
      digitalWrite(p_vermelho,0);
      delay(500);
      digitalWrite(p_vermelho,1);
      delay(500);
      
    }
      digitalWrite(c_vermelho_2,0);
        btn_pedestre_var = 0;
      break;
  }
      
}

void btn_pedestre(){
  btn_pedestre_var = 1;
}
