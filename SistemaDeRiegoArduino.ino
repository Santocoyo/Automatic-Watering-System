#include <Adafruit_Sensor.h>  //LAMADA A LIBRERÍA DE FAMILIA DE SENSORES
#include <DHT.h>  //LLAMADA A LIBRERÍA DEL SENSOR DHT11
#include <LiquidCrystal.h>  //LLAMADA A LA LIBRERÍA DEL DISPLAY

DHT dht(7, DHT11);  //DECLARACIÓN DE LA VARIABLE DEL SENSOR DHT11
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int riego = 8; //DEFINICIÓN DE PIN PARA LA ELECTROVÁLVULA
int htierra_min = 10; //DEFINICIÓN DE HUMEDAD DE TIERRA MÍNIMA
int haire_min = 30; //DEFINICIÓN DE HUMEDAD DE AIRE MÍNIMA
int temp_max = 35;  //DEFINICIÓN DE TEMPERATURA DE AIRE MÁXIMA
byte grados[8] = {
  B00111,
  B00101,
  B00111,
  B00000,
  B00000,
  B00000,
  B00000,
};
bool TempAlta = false;
bool HumAireBaja = false;
bool HumTierraBaja = false;

void setup() {
  pinMode(riego, OUTPUT); //CONFIGURACIÓN DE SALIDA PARA LA ELECTROVÁLVULA
  Serial.begin(9600); //INICILIZACIÓN DEL PUERTO SERIAL
  lcd.begin(16,2); //INICIALIZACIÓN DEL DISPLAY
  lcd.createChar(1, grados);  //CREANDO EL SÍMBOLO DE GRADOS
  Serial.println("Prueba del sensor DHT11");  //MENSAJE DE INICIO
  dht.begin();  //INICIO DE LA CLASE DHT
}

void loop() {
  bool regar = false;
  HumTierraBaja=false;
  HumAireBaja=false;
  TempAlta=false;
  delay(2000);  //TIEMPO ENTRE CADA LECTURA EN MILISEGUNDOS
  int htierra=map(analogRead(A0), 0, 1023, 100, 0); //DECLARACIÓN DE VARIABLE DE HUMEDAD DE TIERRA
  int haire= dht.readHumidity();  //DECLARACIÓN DE VARIABLE DE HUMEDAD DE AIRE
  int temp=dht.readTemperature(); //DECLARACIÓN DE VARIABLE DE TEMPERATURA DEL AIRE
  if(isnan(haire)||isnan(temp)){
    Serial.println("Falla al leer el sensor DHT11!!"); //MENSAJE DE ERROR SI DHT11 NO SE DETECTA
    return;
  }

  if(htierra<htierra_min){
    regar = true;
    HumTierraBaja=true;
  }
  if(haire<haire_min){
    if(htierra<60){
      regar = true;
      HumAireBaja=true;
    }
  }
  if(temp>temp_max){
    if(htierra<60){
      regar = true;
      TempAlta=true;
    }
  }

  Serial.print(haire);    //IMPRESIÓN AL PUERTO SERIAL
  Serial.print("\t");     //EL ORDEN DE DATOS ES:
  Serial.print(htierra);  //HUMEDAD DEL AIRE
  Serial.print("\t");     //HUMEDAD DE LA TIERRA
  Serial.print(temp);   //TEMPERATURA DEL AIRE
  Serial.print("\t");   //ACTIVACIÓN DE ELECTROVALVULA
  if(regar == true){
    Serial.println("1");
    digitalWrite(riego, HIGH);
  }
  else{
    Serial.println("0");
    digitalWrite(riego, LOW);
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("HAir:");
  lcd.setCursor(5,0);
  lcd.print(haire);
  lcd.setCursor(8,0);
  lcd.print("%");
  lcd.setCursor(10,0);
  lcd.print("T:");
  lcd.setCursor(12,0);
  lcd.print(temp);
  lcd.setCursor(14,0);
  lcd.write(1);
  lcd.setCursor(15,0);
  lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("HGnd:");
  lcd.setCursor(5,1);
  lcd.print(htierra);
  lcd.setCursor(8,1);
  lcd.print("%");
  lcd.setCursor(10,1);
  lcd.print("EV:");
  lcd.setCursor(13,1);
  if(regar == true){
    lcd.print("ON");
  }
  else{
    lcd.print("OFF");
  }
  
  if(HumTierraBaja==true){
    delay(6000);
  }
  if(HumAireBaja==true){
    delay(3000);
  }
  if(TempAlta==true){
    delay(2000);
  }
}
