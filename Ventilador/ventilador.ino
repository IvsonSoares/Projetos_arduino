// C++ code
//
//


#include <Adafruit_NeoPixel.h>

#define PIN 3
#define STRIPSIZE 12
int btn = 2;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIPSIZE, PIN, NEO_GRB + NEO_KHZ800);


enum {
    DESLIGADO,
	HORARIO,
  	ANTIHORARIO,
    C_G_HORARIO,
  	C_G_ANTIHORARIO,
    P_G_HORARIO,
  	P_G_ANTIHORARIO
  	
} estado = DESLIGADO;


int red = 0;
int green = 0;
int blue = 0;

int ddelay = 1000;

int pa = 0;

bool mudagirando = false;

bool b_desligado = false;


void setup()
{
  strip.begin();
  strip.setBrightness(25);  // Lower brightness and save eyeballs!
  strip.show(); // Initialize all pixels to 'off'
  
  pinMode(btn, INPUT_PULLUP);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(btn), printBtn, FALLING);

}

void loop()
{
  Serial.println(estado);
  
  switch(estado)
  {
    case DESLIGADO:
        f_desliga();
    	break;
    case HORARIO:
    	f_horario();
    	break;
    case ANTIHORARIO:
        f_antiHorario();
    	break;
    case C_G_HORARIO:
    	f_c_g_horario();
    	break;
    case C_G_ANTIHORARIO:
    	f_c_g_antihorario();
    	break;
    case P_G_HORARIO:
    	f_p_g_horario();
    	break;
    case P_G_ANTIHORARIO:
    	f_p_g_antihorario();
    	break;
    default: Serial.println("Estado invalido");
      
  }
  
  
  
}

void printBtn(){
  
  int read = analogRead(A5);
  
  switch(read){
  
    //Desliga
  	case 136:
    Serial.println("oi");
    if(estado==C_G_HORARIO){
      Serial.println("pg");
      estado=P_G_HORARIO;
      mudagirando = false;
      break;
    }
    if(estado==C_G_ANTIHORARIO){
      estado=P_G_ANTIHORARIO;
      mudagirando = false;
      break;
    }
    if(estado==P_G_HORARIO){
      mudagirando = false;
      break;
    }
    if(estado==P_G_ANTIHORARIO){
      mudagirando = false;
      break;
    }
    if(estado==HORARIO){
      Serial.println("boi");
      estado=P_G_HORARIO;
      mudagirando = false;
      break;
    }
    if(estado==ANTIHORARIO){
      estado=P_G_ANTIHORARIO;
      mudagirando = false;
      break;
    }

    	break;
    //Horario
    case 95:
    if(estado==DESLIGADO){
      estado = C_G_HORARIO;
      break;
    }
    if(estado==C_G_ANTIHORARIO){
      estado = P_G_ANTIHORARIO;
      mudagirando = true;
      break;
    }
    if(estado==P_G_ANTIHORARIO){
      mudagirando = true;
      break;
    }
    if(estado==ANTIHORARIO){
      estado = P_G_ANTIHORARIO;
      mudagirando = true;
      break;
    }
    if(estado==P_G_HORARIO){
      estado = C_G_HORARIO;
      break;
    }
    	break;
    //AntiHorario
    case 50:
    if(estado==DESLIGADO){
      estado = C_G_ANTIHORARIO;
      break;
    }
    if(estado==C_G_HORARIO){
      estado = P_G_HORARIO;
      mudagirando = true;
      break;
    }
    if(estado==P_G_HORARIO){
      mudagirando = true;
      break;
    }
    if(estado==HORARIO){
      estado = P_G_HORARIO;
      mudagirando = true;
      break;
    }
    if(estado==P_G_ANTIHORARIO){
      estado = C_G_ANTIHORARIO;
      break;
    }
    	break;
  
  }

  
}

void f_desliga(){
  Serial.println("Desligado");
  strip.setPixelColor(pa,strip.Color(0,0,255));
  strip.show();
}

void f_horario(){
Serial.println("Horario");
  while (estado==HORARIO){
      	strip.setPixelColor(pa,strip.Color(0,0,0));
    if(pa+1 == 12)pa=-1;
    	strip.setPixelColor(pa+1,strip.Color(0,0,255));
    strip.show();
    delay(ddelay);
    pa++;
  }
}

void f_antiHorario(){
  Serial.println("Anti-horario");
   while(estado==ANTIHORARIO){
  		strip.setPixelColor(pa,strip.Color(0,0,0));
    if(pa-1 == -1){pa=12;}
    	strip.setPixelColor(pa-1,strip.Color(0,0,255));
    strip.show();
    delay(ddelay);
    pa--;
  }
 
}

void f_c_g_horario(){
  Serial.println("começa horario");
  for(int i=0;ddelay>10;i++){
    if(estado!=C_G_HORARIO){return;}
  		strip.setPixelColor(pa,strip.Color(0,0,0));
    if(pa+1 == 12){pa=-1;}
    	strip.setPixelColor(pa+1,strip.Color(0,0,255));
    strip.show();
    delay(ddelay);
    pa++;
    ddelay = ddelay - 55;
  }
  
  estado = HORARIO;

}

void f_c_g_antihorario(){
  Serial.println("começa anti-horario");
    for(int i=0;ddelay>10;i++){
    if(estado!=C_G_ANTIHORARIO){return;}
  		strip.setPixelColor(pa,strip.Color(0,0,0));
    if(pa-1 == -1){pa=12;}
    	strip.setPixelColor(pa-1,strip.Color(0,0,255));
    strip.show();
    delay(ddelay);
    pa--;
    ddelay = ddelay - 55;
  }
  estado = ANTIHORARIO;

}

void f_p_g_horario(){
  Serial.println("termina horario");
    while(ddelay<1000){
      if(estado!=P_G_HORARIO)return;
    Serial.println("for");
  		strip.setPixelColor(pa,strip.Color(0,0,0));
    if(pa+1 == 12){pa=-1;}
    	strip.setPixelColor(pa+1,strip.Color(0,0,255));
    strip.show();
    delay(ddelay);
    pa++;
    ddelay = ddelay + 55;
  }
  if(mudagirando){
    estado = C_G_ANTIHORARIO;
    mudagirando=false;
    return;
  }
  estado = DESLIGADO;
}


void f_p_g_antihorario(){
  Serial.println("termina anti-horario");
    for(int i=0;ddelay<1000;i++){
      if(estado!=P_G_ANTIHORARIO)return;
  		strip.setPixelColor(pa,strip.Color(0,0,0));
    if(pa-1 == -1){pa=12;}
    	strip.setPixelColor(pa-1,strip.Color(0,0,255));
    strip.show();
    delay(ddelay);
    pa--;
    ddelay = ddelay + 55;
  }
  if(mudagirando){
    estado =C_G_HORARIO;
    mudagirando=false;
    return;
  }
  estado = DESLIGADO;
}
