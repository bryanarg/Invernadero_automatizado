//---watchdog---

#include <avr/wdt.h>
long intervalWDT = 500;
unsigned long previousMillisWDT=0;
long intervalled = 1000;
unsigned long previousMillisled=0;

//---multitarea---

#include<pt.h>
struct pt ptlcd;
struct pt ptaht10;
struct pt ptlux;
struct pt ptdatos;
struct pt ptprint;
struct pt ptwdog;
struct pt ptrele;

//---luminosidad---
#define DIVISOR_PRECISION 1.2 // valor por el que dividir la lectura para calcular la luminosidad 1.2 si no hay cambios en la ventana óptica
#define ESPERA_BH1750_0 1000
#define TIMEOUT_I2C 10
#define MEDIDA_CONTINUA_UN_LUX_BH1750 0x10
#define DIRECCION_BH1750_1 0x23 //direccion de sensor 1 de lux BH1750
#define DIRECCION_BH1750_0 0x5C //direccion de sensor 2 de lux BH1750
#include "BH1750.h" // libreria para sensor de lux BH1750

unsigned int lectura_BH1750_0;
unsigned int lectura_BH1750_1;
unsigned int Lx1;
unsigned int Lx2;

long cronometro_lecturas=0;
long tiempo_transcurrido;
long cronometro_timeout_i2c;

//---ethernet---

#include <UIPEthernet.h> //libreria para enc28j60 para sustituir la libreria ethernet

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFF, 0xEE}; // Direccion MAC
byte ip[] = { 192,168,1,121 }; // Direccion IP del Arduino


char server[] = { "www.inverka.com" }; // ***Aqui debemos introducir el dominio de la pagina***


EthernetClient cliente;//objeto del ethernet
String recibido[3] = {"max=","min=","cme="};

//---humedad_suelo---

int cant = 6; // cantidad de sensores
int sensores [16]={A0,A2,A4,A6,A8,A10}; // entradas analogicas
int hum [16]; //variable para humedad
int m [8];
int hummax=75;
int hummin=35;
int recib[3];

//---lcd---
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
byte gota[] = {B00100,B01110,B11111,B11111,B11111,B11111,B01110,B00000};// bytes para lcd
byte sol[] = {B00100,B10101,B01110,B11111,B01110,B10101,B00100,B00000};//bytes para lcd
byte temperatura[] = {B00100,B01010,B01010,B01010,B01110,B11111,B11111,B01110};// bytes para lcd

//---humedad_temperatura---
#include  "AHT10.h" // libreria para sensor de humead y temperatura
#include <Wire.h> //libreria para i2c
float humd ;
float temp ;

AHT10 humtemp;//objeto de sensor AHT10

//---rele---
int rele [16]={23,27,31,35,39,43,47,49}; // pines de salida


      
void setup()
{
  //--watchdog--

  wdt_disable();

  Serial.begin(9600);

  Wire.begin();

//--BH1750--

PT_INIT(&ptlux);

//--lcd--
PT_INIT(&ptlcd);

//--aht10--

PT_INIT(&ptaht10);

//--print--
PT_INIT(&ptprint);

//--rele--
PT_INIT(&ptrele);

//conexion
    PT_INIT(&ptdatos);

//watchdog
PT_INIT(&ptwdog);


wdt_enable(WDTO_8S);
}

void loop()
{
  //-watchdog-
  wadog(&ptwdog);
  
  //-electrovalvulas-humedad de suelo-  
  relays(&ptrele);
  
  //-aht10-
  sensoraht10(&ptaht10);

  //-lux-
  sensorlux(&ptlux);  

  // -Serialprint-
  serialprint(&ptprint);  

  //-lcd-
  lcd1(&ptlcd);
  
  //-conexion-
  //datos(&ptdatos);

}


//-conexion-

void datos(struct pt *pt){

  PT_BEGIN(pt);//aqui inicia protothread
  static long t=0;
  //void setup(){

  Ethernet.begin(mac, ip); // Inicializamos el Enc28j60    
    
  //}

  //void loop(){
  do{

    Serial.println("Envio de datos, conectando...");

  if (cliente.connect(server, 80)>0) {  // Conexion con el servidor
    cliente.print("GET /Invernadero/conexion_invernadero.php?hum="); // Enviamos los datos por GET
    cliente.print(humd);
    cliente.print("&temp=");
    cliente.print(temp);
    cliente.print("&mesa1=");
    cliente.print(hum[0]);
    cliente.print("&mesa2=");
    cliente.print(hum[1]);
    cliente.print("&mesa3=");
    cliente.print(hum[2]);
    cliente.print("&mesa4=");
    cliente.print(hum[3]);
    cliente.print("&mesa5=");
    cliente.print(hum[4]);
    cliente.print("&mesa6=");
    cliente.print(hum[5]);
    cliente.print("&lux1=");
    cliente.print(Lx1);
    cliente.print("&lux2=");
    cliente.print(Lx2);
    
    cliente.println(" HTTP/1.1");
    cliente.println("host:" server);
    cliente.println("User-Agent: Arduino 1.1");
    cliente.println();
    Serial.println("Envio con exito ");

    
    cliente.flush();
    t=millis();
    PT_WAIT_UNTIL(pt, (millis()-t)>=500);
    } 
  else {
    Serial.println("Fallo en la conexion");
    
    t=millis();
    PT_WAIT_UNTIL(pt, (millis()-t)>=500);
  }

  t=millis();
    PT_WAIT_UNTIL(pt, (millis()-t)>=500);



  //-recibir datos-
  
  Serial.println("ingreso de Datos, conectando...");

  
  if (cliente.connect(server, 80)>0) {  // Conexion con el servidor
    cliente.print("GET /Invernadero/leerdatos.php"); 
        
    cliente.println(" HTTP/1.1");
    cliente.println("host: www.inverka.com");
    cliente.println("User-Agent: Arduino 1.1");
    cliente.println();
    
    while(!cliente.available()){} 
    Serial.println("leyendo ");
  }
    else {
      Serial.println("Fallo en la conexion");
      t=millis();
      PT_WAIT_UNTIL(pt, (millis()-t)>=500);
    }
  
    String nombre;
    String codigo;
    while (cliente.available()) {
    char c = cliente.read();
    codigo += c;
    }
    
    if (codigo!="") {
      for (int a = 0; a < 3; a++)
      {
        int longitud = codigo.length();
        int posicion = codigo.indexOf(recibido[a]);
        for (int i = posicion + 4; i < longitud; i ++)
        {
          if (codigo[i] == ';') i = longitud;
          else nombre += codigo[i];
          
        }
        Serial.println(nombre);
        recib[a]=nombre.toInt();
        nombre="";
        
      }
      Serial.println("datos recibidos con exito ");
    hummax=recib[0];
    hummin=recib[1];
    cant=recib[2];
    }
    Serial.println("humedad maxima: ");
    Serial.println(hummax);
    Serial.println("humedad minima: ");
    Serial.println(hummin);
    Serial.println("cantidad de mesas: ");
    Serial.println(cant);
    

  if (!cliente.connected()) {
    Serial.println("Desconectando");
    
    t=millis();
    PT_WAIT_UNTIL(pt, (millis()-t)>=50);//espera por un segundo
  }
  
  cliente.stop();
  cliente.flush();
  Serial.println();
  
  }while(true);
  PT_END(pt);
  
  //}
  
}


//-bh1750-

void sensorlux(struct pt *pt){

  PT_BEGIN(pt);//aqui inicia protothread
  static long t=0;
  //void setup(){
  
    Wire.beginTransmission(DIRECCION_BH1750_0);
    Wire.write(MEDIDA_CONTINUA_UN_LUX_BH1750);
    Wire.endTransmission();
    
    Wire.beginTransmission(DIRECCION_BH1750_1);
    Wire.write(MEDIDA_CONTINUA_UN_LUX_BH1750);
    Wire.endTransmission();
    

  //}

  //void loop(){
  do{

  t=millis();
  PT_WAIT_UNTIL(pt, (millis()-t)>=100);

  //Lx1=0;
  
  Wire.beginTransmission(DIRECCION_BH1750_0);
  Wire.write(MEDIDA_CONTINUA_UN_LUX_BH1750);
  Wire.endTransmission();
  
  tiempo_transcurrido=millis()-cronometro_lecturas;
  if(tiempo_transcurrido>ESPERA_BH1750_0)
  {
  cronometro_lecturas=millis();
  Wire.beginTransmission(DIRECCION_BH1750_0);
  Wire.requestFrom(DIRECCION_BH1750_0,2);
  do
  {
      tiempo_transcurrido=millis()-cronometro_timeout_i2c;
  }
  while(Wire.available()<2&&tiempo_transcurrido<TIMEOUT_I2C);
  if(Wire.available()>1)
  {
    lectura_BH1750_0=Wire.read()<<8; // Leer el primer byte y rotarlo 8 bits
    lectura_BH1750_0|=Wire.read(); // Leer el segundo byte y «juntarlo» con el anterior on OR
    
    Lx1=long(100.0*(float)lectura_BH1750_0/DIVISOR_PRECISION)/100; // Resultado corregido y sin decimales
  }
  
  
  t=millis();
  PT_WAIT_UNTIL(pt, (millis()-t)>=100);//espera por un segundo;

  //Lx2=0;
  
  Wire.beginTransmission(DIRECCION_BH1750_1);
  Wire.write(MEDIDA_CONTINUA_UN_LUX_BH1750);
  Wire.endTransmission();
  
  Wire.beginTransmission(DIRECCION_BH1750_1);
  Wire.requestFrom(DIRECCION_BH1750_1,2);
  
  {
      tiempo_transcurrido=millis()-cronometro_timeout_i2c;
  }
  while(Wire.available()<2&&tiempo_transcurrido<TIMEOUT_I2C);
  if(Wire.available()>1)
      {
      lectura_BH1750_1=Wire.read()<<8; // Leer el primer byte y rotarlo 8 bits
      lectura_BH1750_1|=Wire.read(); // Leer el segundo byte y «juntarlo» con el anterior on OR
  
      Lx2=long(100.0*(float)lectura_BH1750_1/DIVISOR_PRECISION)/100; // Resultado corregido y sin decimales
      
      }

      t=millis();
  PT_WAIT_UNTIL(pt, (millis()-t)>=100);// Espera inicial para estabilizar el BH1750
        
  }

 
  
  }while(true);
  PT_END(pt);
  
  //}
  
}

//-aht10-


void sensoraht10(struct pt *pt){

  PT_BEGIN(pt);//aqui inicia protothread
  static long t=0;
  //void setup(){

    humtemp.begin();
    Serial.print("llegue");

  //}

  //void loop(){
  do{
  humd = humtemp.readHumidity();
  temp = humtemp.readTemperature();
  
  
  
  t=millis();
  PT_WAIT_UNTIL(pt, (millis()-t)>=50);
  
  }while(true);
  PT_END(pt);
  
  //}
  
}



//-lcd-


void lcd1(struct pt *pt){

  PT_BEGIN(pt);//aqui inicia protothread
  static long t=0;
  //void setup(){

    lcd.begin (16, 2);
    lcd.init();
    lcd.backlight();
    
    lcd.createChar(0, gota);
    lcd.createChar(1, sol);
    lcd.createChar(2, temperatura);

    

  //}

  //void loop(){
  do{
    
      lcd.setCursor(4,0);
      lcd.write (byte (0));
      
      lcd.setCursor(5,0);
      lcd.print(humd +String("%"));

      lcd.setCursor(4,1);
      lcd.write (byte (2));
      
      lcd.setCursor(5,1);
      lcd.print(temp +String("C"));
      
      lcd.setCursor(13 ,0);
      lcd.write (byte (1));
      
      lcd.setCursor(14,0);
      lcd.print(Lx1 +String("Lx"));
      
      
      lcd.setCursor(13,1);
      lcd.write (byte (1));
      
      lcd.setCursor(14,1);
      lcd.print(Lx2 +String("Lx"));
      
      
      lcd.setCursor(24,0);
      lcd.print("1:"+String(hum[0]));
      
      lcd.setCursor(24,1);
      lcd.print("2:"+String(hum[1]));
      
      lcd.setCursor(30,0);
      lcd.print("3:"+String(hum[2]));
      
      lcd.setCursor(30,1);
      lcd.print("4:"+String(hum[3]));
      
      lcd.setCursor(36,0);
      lcd.print("5:"+String(hum[4]));
      
      lcd.setCursor(36,1);
      lcd.print("6:"+String(hum[5]));

      

      lcd.scrollDisplayLeft(); 
      t=millis();
      PT_WAIT_UNTIL(pt, (millis()-t)>=500);
      lcd.setCursor(0,0);
      lcd.print("                                  ");
      lcd.setCursor(0,1);
      lcd.print("                                  ");
      
  
  
  }while(true);
  PT_END(pt);
  
  //}
  
}



void serialprint(struct pt *pt){

  PT_BEGIN(pt);//aqui inicia protothread
  static long t=0;
  //void setup(){

    
  //}

  //void loop(){
  do
  {
    Serial.println("inicio");
    for (int a = 0; a < cant; a++)
    {
      Serial.println(hum[a]);
    }

    //Serial.print("LUX 1: ");
    Serial.println(Lx1);
    //Serial.print("LUX 2: ");
    Serial.println(Lx2);

    //Serial.print("temperatura: ");
    Serial.println(temp, 1);
   // Serial.print("humedad: ");
    Serial.println(humd, 1);

    Serial.println(hummax);
    Serial.println(hummin);

    Serial.flush ();

    t=millis();
    PT_WAIT_UNTIL(pt, (millis()-t)>=1000);//espera por un segundo
    
  }while(true);
  PT_END(pt);
  
  //}
  
}

void wadog (struct pt *pt){

  PT_BEGIN(pt);//aqui inicia protothread
  static long t=0;
  
  unsigned long currentMillis = millis();
  do{
  if(((currentMillis)-previousMillisWDT) >= intervalWDT)
  {
    previousMillisWDT=currentMillis;
    wdt_reset();
  }
  t=millis();
  PT_WAIT_UNTIL(pt, (millis()-t)>=5000);
  }  while(true);
  PT_END(pt);
}

void relays (struct pt *pt){

  PT_BEGIN(pt);//aqui inicia protothread
  static long t=0;
  for (size_t i = 0; i <= cant ; i++)
  {
    pinMode(rele[i], OUTPUT);
  }

  for (int a = 0; a < cant; a++)
  {   
        digitalWrite(rele[a], HIGH);
  }
  
  do{

  digitalWrite(rele[6], HIGH);//se activa el rele que corta la señal de comun para evitar encender regadores al encender microcontrolador
      
  for (int a = 0; a < cant; a++)
  {
    hum [a]=analogRead(sensores[a]);
    
    hum[a] = map(hum[a],1,1023,100,0);
  
    if (hum[a]<hummin) 
      {
        digitalWrite(rele[a], LOW);
      }//else{digitalWrite(rele[a], HIGH);}
    
      if (hum[a]>hummax)
      {
        digitalWrite(rele[a], HIGH);
        
      }
  }
  
  t=millis();
  PT_WAIT_UNTIL(pt, (millis()-t)>=1000);
  }  while(true);
  PT_END(pt);
}
