#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <OneWire.h>                
#include <DallasTemperature.h>  
int adc_id = 0;
int HistoryValue = 0;
char printBuffer[128];
LiquidCrystal_I2C lcd(0x27,16,2);
int valor=0;
double voltaje;
//byte sensorpir =3;
//byte led =2;
#define led2 10
float valor2;
OneWire ourWire(12); 
DallasTemperature sensors(&ourWire);

int pulsePin=0;
volatile int BPM;
volatile int Signal;
volatile int IBI=600;
volatile boolean Pulse=false;
volatile boolean QS=false;

const byte FILAS = 4;     
const byte COLUMNAS = 4;    
char keys[FILAS][COLUMNAS] = {    
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte pinesFilas[FILAS] = {9,8,7,6};   
byte pinesColumnas[COLUMNAS] = {5,4,3,2}; 
Keypad teclado = Keypad(makeKeymap(keys), pinesFilas, pinesColumnas, FILAS, COLUMNAS);  
char TECLA;       
int frecuencia=220;

void setup() {                
  Serial.begin (9600); 
  lcd.init();
  Wire.begin();
  lcd.begin(16,2);
  lcd.backlight();
  pinMode(led2,OUTPUT);
  pinMode(13, OUTPUT); 
  pinMode(11,OUTPUT);
  sensors.begin();
  }
  
void loop() {
  int value = analogRead(adc_id);
  //comandos
  TECLA = teclado.getKey();   
  if (TECLA != NO_KEY)        
  {
    if(TECLA == '1'){
      lcd.clear();
      lcd.print("Niveles de oxigeno");
      if(value<500){
        lcd.setCursor(0,1);
        lcd.print("BAJOS");
      }
      if(value>=500 &&value<=600){
        lcd.setCursor(0,1);
        lcd.print("MEDIO");
      }
      if(value>600){
        lcd.setCursor(0,1);
        lcd.print("ALTOS-NORMALES");
      }
      delay(3000);
      lcd.clear();
    }if(TECLA =='2'){
      lcd.clear();
      lcd.print("Temperatura ambiente");
      delay(3000);
      lcd.clear();
    }if(TECLA =='3'){
      lcd.clear();
      lcd.print("Temperatura p");
      lcd.setCursor(0,1);
      sensors.requestTemperatures();
      float temp= sensors.getTempCByIndex(0);
      lcd.print(temp);
      delay(3000);
      lcd.clear();
    }if(TECLA=='4'){
      lcd.clear();
      lcd.print("Ritmo cardiaco");
      int pulso = analogRead(A2);          
      if (pulso >= 530) {                  
        digitalWrite(13, HIGH);
      }  
      else{
        digitalWrite(13, LOW);
      }
      lcd.setCursor(0,1);
      lcd.print(pulso);                          
      delay(3000);
      lcd.clear();
    }if(TECLA=='A'){
      lcd.clear();
      do{
        TECLA = teclado.getKey();  
        lcd.setCursor(0,0);  
        lcd.print("CODIGO ROJO");
        lcd.setCursor(0,1);
        lcd.print("ALERTA-ALERTA");
        tone(11,700);
      }while(TECLA != 'D');
      noTone(11);
      lcd.clear();
    }if(TECLA=='B'){
      lcd.clear();
      do{
        TECLA = teclado.getKey();
        lcd.setCursor(0,0);  
        lcd.print("CODIGO AZUL");
        lcd.setCursor(0,1);
        lcd.print("ALERTA-ALERTA");
        tone(11,500);
      }while(TECLA !='D');
      noTone(11);
      lcd.clear();
    }
    if(TECLA=='C'){
      lcd.clear();
      do{
        TECLA = teclado.getKey(); 
        lcd.setCursor(0,0);   
        lcd.print("CODE VIOLETA");
        lcd.setCursor(0,1);
        lcd.print("ALERTA-ALERTA");
        tone(11,300);  
      }while(TECLA != 'D');
      noTone(11);
      lcd.clear();
    }
    if(TECLA=='D'){
      lcd.clear();
      noTone(11);
    }
  }                       
  //QDR
  valor2 = analogRead(A1);
  valor2=(valor2*5.0)/1023.0;
  if(valor2 < 3.3){ 
    digitalWrite(led2,HIGH);
    lcd.clear();
    lcd.print("Paciente 1");
    lcd.setCursor(0,1);
    lcd.print("ha llamado");
    tone(11,1000);
    delay(5000);
    lcd.clear();
    noTone(11);
  }else{
    digitalWrite(led2,LOW);   
  }

  Serial.println(valor2); 
  delay(100);
   //pantalla
  lcd.setCursor(0,0);
  lcd.print("Hola doctor");
  lcd.setCursor(0,1);
  lcd.print("que desea hacer?");
  
   //sensor de nivel de agua
  if(((HistoryValue>=value) && ((HistoryValue - value) > 10)) || ((HistoryValue<value) && ((value - HistoryValue) > 10)))
  {
    sprintf(printBuffer,"ADC%d level is %d\n",adc_id, value);
    Serial.print(printBuffer);
    HistoryValue = value;
  }  
}
