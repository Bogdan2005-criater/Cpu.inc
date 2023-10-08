// Подключение библиотек
#include <Arduino.h>
#include <DHT.h>
#include <GyverBME280.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
// объявляем объект, для управления дисплеем указываем пины
LiquidCrystal lcd(2,3,4,5,6,7);


SoftwareSerial gsmSerial(8, 9); 
GyverBME280 bme;

#define DHTPIN 10     // Пин, к которому подключен выход датчика KY-015 (может быть другой пин)
#define DHTTYPE DHT11 // Используемый тип датчика (DHT11 или DHT22)
#define PhP A1 // Пин, к которому подключен фоторезистор
#define WS A2
#define PressureL 12
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(PhP, INPUT_PULLUP); 
  pinMode(WS, INPUT_PULLUP);

  lcd.begin(16, 2);      // указываем количество строк и столбцов
  lcd.setCursor(0,0);              // Установка курсора в начало первой строки
  lcd.print("Hello");       // Набор текста на первой строке
  lcd.setCursor(0,1);              // Установка курсора в начало второй строки
  lcd.print("WeatherWise");  
  if (bme.begin(0x76)) Serial.println("Sucsess!");
 // Набор текста на второй строке
   // Подключение к сети GSM
  gsmSerial.println("AT"); // Отправляем команду AT для проверки связи с GSM модулем
  delay(1000);
  
  // Проверяем, есть ли сеть
  gsmSerial.println("AT+CREG?"); 
  delay(1000);
  
  // Устанавливаем GPRS соединение (замените APN, username и password на данные вашего оператора)
  gsmSerial.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  delay(1000);
  gsmSerial.println("AT+SAPBR=3,1,\"APN\",\"your_apn_here\"");
  delay(1000);
  gsmSerial.println("AT+SAPBR=3,1,\"USER\",\"your_username_here\"");
  delay(1000);
  gsmSerial.println("AT+SAPBR=3,1,\"PWD\",\"your_password_here\"");
  delay(1000);
  gsmSerial.println("AT+SAPBR=1,1");
  delay(2000);
  
  // Установка HTTP параметров
  gsmSerial.println("AT+HTTPINIT");
  delay(1000);
  gsmSerial.println("AT+HTTPPARA=\"CID\",1");
  delay(1000);
  delay(2000);
}
void loop(){
  float humidity = dht.readHumidity();     // Считываем влажность
  float temperature = dht.readTemperature(); // Считываем температуру в градусах Цельсия
    // Отправка данных на сервер Flask

  lcd.begin(16, 2); 
  lcd.clear();  
  lcd.setCursor(0,0); 
  if (isnan(humidity) || isnan(temperature)) {
    lcd.print("Error #1");
  } else {
    lcd.print("Humidity: ");
    lcd.print(humidity);
    lcd.setCursor(0,1); 
    lcd.print("TempC: ");
    lcd.println(temperature);
    lcd.print("C");
  }

  delay(2000);
  lcd.clear(); 
  lcd.setCursor(0,0);
  lcd.print("GroundH: ");
  lcd.print(analogRead(WS));

  delay(500);
  lcd.setCursor(0,1);
  lcd.print("LightS: ");
  lcd.print(analogRead(PhP));
  
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Pressure: ");
  lcd.print(bme.readPressure());
  delay(5000);
}
