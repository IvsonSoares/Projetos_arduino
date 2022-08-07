enum { INOPERANTE,
       OCIOSO,
       ALINHADO_S,
       MOVENDO_S,
       ESTACIONADO_S,
       FIM_S,
       ALINHADO_D,
       MOVENDO_D,
       ESTACIONADO_D,
       FIM_D} estado = INOPERANTE;

byte displaySeteSegmentos[10][7] = {   
 { 1,1,1,1,1,1,0 },  //DIGITO 0
 { 0,1,1,0,0,0,0 },  //DIGITO 1
 { 1,1,0,1,1,0,1 },  //DIGITO 2
 { 1,1,1,1,0,0,1 },  //DIGITO 3
 { 0,1,1,0,0,1,1 },  //DIGITO 4
 { 1,0,1,1,0,1,1 },  //DIGITO 5
 { 1,0,1,1,1,1,1 },  //DIGITO 6
 { 1,1,1,0,0,0,0 },  //DIGITO 7
 { 1,1,1,1,1,1,1 },  //DIGITO 8
 { 1,1,1,1,0,1,1 }   //DIGITO 9
};

boolean Desce [10] = {
  false, // T
  false, // 1
  false, // 2
  false, // 3
  false, // 4
  false, // 5
  false, // 6
  false, // 7
  false, // 8
  false  // 9
};

boolean Sobe [10] = {
  false, // T
  false, // 1
  false, // 2
  false, // 3
  false, // 4
  false, // 5
  false, // 6
  false, // 7
  false,  // 8
  false  // 9
};

boolean Elevador [14] = {
  false, // T
  false, // 1
  false, // 2
  false, // 3
  false, // 4
  false, // 5
  false, // 6
  false, // 7
  false, // 8
  false, // 9
  false, // Fecha porta
  false, // Emergencia
  false, // On
  false  // OFF
};


int Andar = 0;


void setup()
{
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  Serial.begin(3600);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), SobeDesce, FALLING);
  attachInterrupt(digitalPinToInterrupt(3), BElevador, FALLING);
}

void loop()
{
  Serial.print("Entrando no loop ");
  Serial.print("[");
  Serial.print(estado);
  Serial.print("] ");
  
  switch (estado)
  {
    case INOPERANTE: estado_INOPERANTE(); break;
    case OCIOSO: estado_OCIOSO(); break;
    case ALINHADO_S: estado_ALINHADO_S(); break;
    case MOVENDO_S: estado_MOVENDO_S(); break;
    case ESTACIONADO_S: estado_ESTACIONADO_S(); break;
    case FIM_S: estado_FIM_S(); break;
    case ALINHADO_D: estado_ALINHADO_D(); break;
    case MOVENDO_D: estado_MOVENDO_D(); break;
    case ESTACIONADO_D: estado_ESTACIONADO_D(); break;
    case FIM_D: estado_FIM_D(); break;
    default: Serial.println("ESTADO INVÁLIDO");
  }
  
  for(int pinos = 0; pinos < 7; pinos++){
  	digitalWrite(pinos+7, displaySeteSegmentos[Andar][6-pinos]);
  }
}


void estado_INOPERANTE()
{
  digitalWrite(A4,HIGH);
  Serial.println("==> INOPERANTE");
  while(!Elevador[12]){
    
  }
 
    estado = OCIOSO;
    return;
  
}

void estado_OCIOSO()
{
  digitalWrite(A4,HIGH);
  digitalWrite(4,HIGH);
  Serial.println("==> OCIOSO");
  
  if(Elevador[10]) Elevador[10] = false;
  if(Elevador[11]) {
    Serial.println("corre");
    digitalWrite(A5,HIGH);
    delay(10000);
    digitalWrite(A5,LOW);
    digitalWrite(4,LOW);
    estado = INOPERANTE;
  }
    Elevador[11] = false;
  if(Elevador[12]) Elevador[12] = false;
  if(Elevador[13]) {
    digitalWrite(4,LOW);
    estado = INOPERANTE;
    Elevador[13] = false;
  }
  
  for (int i=0; i<10; i++){
    if(Elevador[i]){
      if(i>Andar){
      	estado = ESTACIONADO_S;
        return;
      }
      if(i<Andar){
        estado = ESTACIONADO_D;
        return;
      }
      if(i=Andar){
      	Elevador[i] = false;
      }
    }
  }
  for (int i=0; i<10; i++){
    if(Sobe[i]){
      if(i>Andar){
      	estado = ESTACIONADO_S;
        return;
      }
      if(i<Andar){
        estado = ESTACIONADO_D;
        return;
      }
      if(i=Andar){
      	Sobe[i] = false;
      }
    }
  }
  for (int i=0; i<10; i++){
    if(Desce[i]){
      if(i>Andar){
      	estado = ESTACIONADO_S;
        return;
      }
      if(i<Andar){
        estado = ESTACIONADO_D;
        return;
      }
      if(i=Andar){
      	Desce[i] = false;
      }
    }
  }
	delay(1000);
}

void estado_ALINHADO_S()
{
  if(Elevador[11]) {
    Serial.println("corre");
    digitalWrite(A5,HIGH);
    delay(10000);
    digitalWrite(A5,LOW);
    digitalWrite(4,LOW);
    Elevador[11] = false;
    estado = INOPERANTE;
    return;
  }
  Serial.println("==> ALINHADO_S");
  
  if(Elevador[Andar]){
    estado = ESTACIONADO_S;
    return;
  }
  
  if(Sobe[Andar]){
    estado = ESTACIONADO_S;
    return;
  }
  
  if(Andar == 9){
    estado = FIM_S;
    return;
  }
  
  for (int i=9; i>Andar; i--){
    if(Sobe[i] || Elevador[i] || Desce[i] ){
      estado = MOVENDO_S;
      return;
    }
  }
  if(Desce[Andar]){
    estado =FIM_S;
    return;
  }

}

void estado_MOVENDO_S()
{
  digitalWrite(6,HIGH);
  Serial.println("==> MOVENDO_S");
  delay(1000);
  Andar++;
  digitalWrite(6,LOW);
  estado = ALINHADO_S;
}

void estado_ESTACIONADO_S()
{ 
  if(Elevador[11]) {
    Serial.println("corre");
    digitalWrite(A5,HIGH);
    delay(10000);
    digitalWrite(A5,LOW);
    digitalWrite(4,LOW);
    Elevador[11] = false;
    estado = INOPERANTE;
    return;
  }
  digitalWrite(A4,HIGH);
  Serial.println("==> ESTACIONADO_S");
  for (int i=0; i<10; i++){
    if(Elevador[10]){
      Elevador[10] = false;
      break;
    }
  	delay(100);
  }
  digitalWrite(A4,LOW);
  Sobe[Andar] = false;
  Elevador[Andar] = false;
  for (int i=9; i>0; i--){
    if(Sobe[i] || Elevador[i] ||Desce[i]){
      if(i>Andar){
      	estado = MOVENDO_S;
        return;
      }
      if(i<Andar){
        estado = ESTACIONADO_D;
        return;
      }
    }
  }
  
  
  estado = FIM_S;
}

void estado_FIM_S()
{
  Serial.println("==> FIM_S");
  estado = OCIOSO;
}

void estado_ALINHADO_D()
{
  if(Elevador[11]) {
    Serial.println("corre");
    digitalWrite(A5,HIGH);
    delay(10000);
    digitalWrite(A5,LOW);
    digitalWrite(4,LOW);
    Elevador[11] = false;
    estado = INOPERANTE;
    return;
  }
  Serial.println("==> ALINHADO_D");
  
  if(Elevador[Andar]){
    estado = ESTACIONADO_D;
    return;
  }
  
  if(Desce[Andar]){
    estado = ESTACIONADO_D;
    return;
  }
  
  if(Andar == 0){
    estado = FIM_D;
    return;
  }
   for (int i=0; i<Andar; i++){
    if(Sobe[i] || Elevador[i] || Desce[i]){
      estado = MOVENDO_D;
      return;
    }
  }
  if(Desce[Andar]){
    estado =FIM_D;
    return;
  }
 
}

void estado_MOVENDO_D()
{
  digitalWrite(5,HIGH);
  Serial.println("==> MOVENDO_D");
  delay(1000);
  Andar--;
  digitalWrite(5,LOW);
  estado = ALINHADO_D;
}

void estado_ESTACIONADO_D()
{
  if(Elevador[11]) {
    Serial.println("corre");
    analogWrite(A5,HIGH);
    delay(10000);
    analogWrite(A5,LOW);
    digitalWrite(4,LOW);
    Elevador[11] = false;
    estado = INOPERANTE;
    return;
  }
  digitalWrite(A4,HIGH);
  Serial.println("==> ESTACIONADO_D");
  for (int i=0; i<10; i++){
    if(Elevador[10]){
      Elevador[10] = false;
      break;
    }
  	delay(100);
  }
  analogWrite(A4,LOW);
  Desce[Andar] = false;
  Elevador[Andar] = false;
  for (int i=0; i<10; i++){
    if(Desce[i] || Elevador[i] || Sobe[i]){
      if(i>Andar){
      	estado = ESTACIONADO_S;
        return;
      }
      if(i<Andar){
        estado = MOVENDO_D;
        return;
      }
    }
  }
  
  estado = FIM_D;
}

void estado_FIM_D()
{
  Serial.println("==> FIM_D");
  estado = OCIOSO;
}




























void SobeDesce(){
  int btn = analogRead(A0);
  if(INOPERANTE == estado)return;
  switch (btn)
  {
    // desce
  		case 50: //1
    		Serial.print("Desce 1");
    		Desce[1] = true;
    	break;
    
    	case 95: //2
    		Serial.print("Desce 2");
    		Desce[2] = true;
    	break;
    
    	case 136: //3
    		Serial.print("Desce 3");
    		Desce[3] = true;
        break;
    
    	case 173: //4
    		Serial.print("Desce 4");
    		Desce[4] = true;
    	break;
    
    	case 208: //5
    		Serial.print("Desce 5");
    		Desce[5] = true;
    	break;
    
    	case 240: //6
    		Serial.print("Desce 6");
    		Desce[6] = true;
    	break; 
    
    	case 269: //7
    		Serial.print("Desce 7");
    		Desce[7] = true;
    	break;
    
    	case 296: //8
    		Serial.print("Desce 8");
    		Desce[8] = true;
        break;
    
    	case 322: //9
    		Serial.print("Desce 9");
    		Desce[9] = true;
    	break;
    
    // sobe
    	
    	case 346: //T
    		Serial.print("Sobe T");
    		Sobe[0] = true;
    	break;
    
    	case 368: //1
    		Serial.print("Sobe 1");
    		Sobe[1] = true;
    	break;
    	
    	case 388: //2
    		Serial.print("Sobe 2");
    		Sobe[2] = true;
    	break;
    
    	case 408: //3
    		Serial.print("Sobe 3");
    		Sobe[3] = true;
    	break;
    
    	case 426: //4
    		Serial.print("Sobe 4");
    		Sobe[4] = true;
    	break;
    
    	case 443: //5
    		Serial.print("Sobe 5");
        	Sobe[5] = true;
    	break;
    
    	case 447: //6
    		Serial.print("Sobe 6");
        	Sobe[6] = true;
    	break;
    
    	case 450: //7
    		Serial.print("Sobe 7");
        	Sobe[7] = true;
    	break;
    
    	case 460: //8
    		Serial.print("Sobe 8");
        	Sobe[8] = true;
    	break;
    
  }
  
  Serial.println(btn);
}
void BElevador(){
  int btn = analogRead(A1);
  if(btn != 408 && estado == INOPERANTE)return;
  switch (btn)
  {
    case 50: //t
    	Serial.print("Vai T");
    	Elevador[0] = true;
    break;
    
    case 95: //1
    	Serial.print("Vai 1");
    	Elevador[1] = true;
    break;
    
    case 136: //2
    	Serial.print("Vai 2");
    	Elevador[2] = true;
    break;
    
    case 173: //3
      	Serial.print("Vai 3");
    	Elevador[3] = true;
    break;
    
    case 208: //4
      	Serial.print("Vai 4");
    	Elevador[4] = true;
    break;
    
    case 240: //5
      	Serial.print("Vai 5");
    	Elevador[5] = true;
    break;
    
    case 269: //6
      	Serial.print("Vai 6");
    	Elevador[6] = true;
    break;
    	
    case 296: //7
      	Serial.print("Vai 7");
    	Elevador[7] = true;
    break;
    
    case 322: //8
      	Serial.print("Vai 8");
    	Elevador[8] = true;
    break;
    
    case 346: //9
      	Serial.print("Vai 9");
    	Elevador[9] = true;
    break;
    
    case 368: // porta
      	Serial.print("Fecha porta");
    	Elevador[10] = true;
    break;
    
    case 388: //emergencia
      	Serial.print("Emergencia");
    	Elevador[11] = true;
    break;
    
    case 408: //on
      	Serial.print("Liga");
    	Elevador[12] = true;
    break;
    
    case 426: //off
      	Serial.print("Desliga");
    	Elevador[13] = true;
    break;
  }
  Serial.println(btn);
}

