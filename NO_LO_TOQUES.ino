// Llamamos a las librerias que vamos a necesitar
#include <Wire.h> // Librería para comunicación I2C
#include <LiquidCrystal_I2C.h> // Librería para controlar el LCD I2C
#include "DHT.h" // Cargamos la librería DHT
#include "RTClib.h" // Librería del reloj real

// CONFIGURACIONES
#define DHTPIN 2 // Seleccionamos el pin en el que se conectará el sensor
#define DHTTYPE DHT22 // Se selecciona el DHT11 (hay otros DHT)
#define VOLTAGE_DIVIDER_RATIO 2.0 // El divisor de voltaje utilizado
#define ANEMOMETER_PIN A3 // El pin del puerto analógico al que está conectado el anemómetro

// DECLARACIÓN DE OBJETOS
RTC_DS1307 RTC; // Objeto para acceder al reloj real
DHT dht(DHTPIN, DHTTYPE); // Se inicia una variable que será usada por Arduino para comunicarse con el sensor
LiquidCrystal_I2C lcd(0x27, 16, 2); // Configuramos el display

void setup () {
  Serial.begin(9600);
  // Inicializar el LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Iniciando...");

  delay(1000);
  dht.begin(); //iniciar el sensor de temperatura y humedad
  //Iniciar el reloj
  Wire.begin(); // Inicializamos la librería wire
  RTC.begin(); // Inicializamos el módulo reloj
  if (! RTC.isrunning()) {
    Serial.println("RTC no esta funcionando, se establecera la hora actual!");
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__))); // Poniendo el reloj en hora
  }

  DateTime now = RTC.now();
  DateTime DATE(now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
  RTC.adjust(DateTime(DATE));
  Serial.println("La hora se ha establecido correctamente.");

}


void loop() {
  double sensorTemp = dht.readTemperature();
  double sensorRain = analogRead(A0);
  double sensorHumidity = dht.readHumidity();
  double sensorAir = analogRead(A1);
  double voltage = analogRead(ANEMOMETER_PIN) * (3.3 / 1023.0); // Leer el voltaje de la señal analógica y convertirlo a voltios
  double sensorWind = (3.0 * PI * 0.42 / 0.6) * 0.7 / VOLTAGE_DIVIDER_RATIO * voltage; // Calcular la velocidad del viento en m/s

  Serial.print(sensorTemp, 2);
  Serial.print(",");
  Serial.print(sensorRain, 2);
  Serial.print(",");
  Serial.print(sensorHumidity, 2);
  Serial.print(",");
  Serial.print(sensorAir, 2);
  Serial.print(",");
  Serial.print(sensorWind, 2);
  Serial.println(); // Agrega un salto de línea al final
  delay(500);

  DateTime now = RTC.now(); // Creamos un objeto que contiene la hora y fecha actual
  lcd.clear();
  lcd.setCursor(0, 0);

  // FUNCION PARA MOSTRAR LA FECHA 
  lcd.print("Fecha:");
  lcd.print(now.year(), DEC); // A§o
  lcd.print('/');
  lcd.print(now.month(), DEC); // Mes
  lcd.print('/');
  lcd.print(now.day(), DEC); // Dia
  lcd.print(' ');
  // FUNCION PARA MOSTRAR LA HORA
  lcd.setCursor(0, 1);
  lcd.print("Hora: ");
  lcd.print(now.hour(), DEC);
  lcd.print(":");
  lcd.print(now.minute(), DEC);

  delay(2500); //ESPERAR 5 SEGUNDOS PARA CAMBIAR PANTALL

  // FUNCION PARA MOSTRAR LA TEMPERATURA 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(sensorTemp, 2);
  lcd.print(" C");

  // FUNCION PARA EL SENSOR DE LLUVIA
  if (sensorRain > 950)
    {
      lcd.setCursor(0, 1);
      lcd.print("Clima Despejado");
    }    
  else if (sensorRain <= 950 && sensorRain >600 )
  { 
    lcd.setCursor(0, 1);
    lcd.print("Hay Chubascos");
  }
  else if (sensorRain <= 600 && sensorRain > 300)
  {
    lcd.setCursor(0, 1);
    lcd.print("Lluvia Moderada");
  }
  else
  {
    lcd.setCursor(0, 1);
    lcd.print("Lluvias Fuertes!");
  }
  
  delay(2500); // Esperamos 5 segundo para actualizar la pantalla

  // FUNCION PARA MOSTRAR LA HUMEDAD
  lcd.clear(); // Limpia el LCD
  lcd.setCursor(0, 0);
  lcd.print("Hume: ");
  lcd.print(sensorHumidity, 2);
  lcd.print(" %");

  // SENSOR DE CALIDAD DEL AIRE 
  lcd.setCursor(0, 1); // Establece el cursor en la primera columna de la primera fila
  lcd.print("Aire: ");
  lcd.print(sensorAir, 2);
  lcd.print(" PPM");

  delay(2500);

  // FUNCION PARA LA VELOCIDAD DEL VIENTO
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Vel. del viento:");
  lcd.setCursor(0, 1);
  lcd.print(sensorWind, 2);
  lcd.print(" m/s");

  delay(2500);

}
