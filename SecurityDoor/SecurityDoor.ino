#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

ESP8266WiFiMulti WiFiMulti;
LiquidCrystal_I2C lcd(0x27,16,2);

#define SERVER "www.rafa23.com/conexion/"
#define USER "rafamx"
#define PASS "E59J7Ua4fVp#!f"

const byte FILAS = 4;
const byte COLUMNAS = 3;

char matriz[FILAS][3]={
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'},
};

char clave[5] = {'1','1','1','1','1'};
char claveUsuario[6];


byte pinesFilas[FILAS] = {D0,D3,D4,D5};
byte pinesColumnas[COLUMNAS] = {D6,D7,10};

Keypad teclado = Keypad(makeKeymap(matriz),pinesFilas,pinesColumnas,FILAS,COLUMNAS);

int cerradura = 3;

int cont = 0;
int iden = 0;
int con2 = 5;
String s;
char Tecla;

void setup(){
  Serial.begin(9600);
  lcd.init(); // inicializar pantalla
  lcd.backlight(); //luz de fondo
  lcd.clear();
  pinMode(cerradura,OUTPUT);
  digitalWrite(cerradura,LOW); 

  Serial.println("Conectado...");
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("Wifi", "123456789");
}


void loop(){
  lcd.setCursor(0,0);
  lcd.print("INGRESE LA CLAVE");
  //se recolpila el digito seleccionado en la variable tecla
  Tecla = teclado.getKey();
  
  Serial.print(Tecla);

  if(Tecla){
    claveUsuario[cont] = Tecla;
    cont++;
  lcd.setCursor(con2,1);
  lcd.print("*");
  con2++;

  if(cont == 5){
  
  for(int i = 0; i<=4;i++){
    
    if(claveUsuario[i] != clave[i]){
      iden = 1;
    }
    
  }
  
    if(iden==1){
      if ((WiFiMulti.run() == WL_CONNECTED)) {
      Serial.println("Conexion exitosa");
      login();
   }
    }
   if(iden ==1){
    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print("CLAVE");
    lcd.setCursor(3,1);
    lcd.print("INCORRECTA"); 
    delay(3000);
    pinMode(cerradura,OUTPUT);
    digitalWrite(cerradura,LOW);
    cont = 0;
    iden = 0;
    con2 =5;
    lcd.clear();
  }
  else{
    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print("CLAVE");
    lcd.setCursor(4,1);
    lcd.print("CORRECTA <3");
    pinMode(cerradura,OUTPUT); 
    digitalWrite(cerradura,HIGH);
    delay(5000);
    pinMode(cerradura,OUTPUT);
    digitalWrite(cerradura,LOW);
    
    con2 =5;
    cont= 0;
      lcd.clear();
  }

    
  }

  
}
  
}


void login(){
  WiFiClient client;
  HTTPClient http;
  claveUsuario[5] = '\0';
  String pass = String(claveUsuario);
  if(http.begin(client, "http://" SERVER "login.php?p=" + pass)){
    int httpCode = http.GET();
    if(httpCode > 0){
      if(httpCode == HTTP_CODE_OK){
        String json = http.getString();
        Serial.println(json);
        if(json.indexOf("si")!=-1)
          iden = 0;
        else
          iden = 1;
      }
    }else{
      Serial.println("Error GET");
    }
  }
}
