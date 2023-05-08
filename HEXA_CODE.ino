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
int pinSensor=A0;

// DECLARACIÓN DE OBJETOS
RTC_DS1307 RTC; // Objeto para acceder al reloj real
DHT dht(DHTPIN, DHTTYPE); // Se inicia una variable que será usada por Arduino para comunicarse con el sensor
LiquidCrystal_I2C lcd(0x27, 16, 2); // Configuramos el display

void setup () {
  Serial.begin(9600);
  Serial.println("setup");

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
  Serial.println("Hora actual:");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  DateTime DATE(now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());

  RTC.adjust(DateTime(DATE));
  Serial.println("La hora se ha establecido correctamente.");

}


void loop() {
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

  delay(5000); //ESPERAR 5 SEGUNDOS PARA CAMBIAR PANTALL

  // FUNCION PARA MOSTRAR LA TEMPERATURA 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(dht.readTemperature());
  lcd.print("C");

  // FUNCION PARA EL SENSOR DE LLUVIA
  int valor = analogRead(pinSensor);
  if (valor > 950)
    {
      lcd.setCursor(0, 1);
      lcd.print("Clima despejado");
    }    
  else if (valor <= 950 && valor >600 )
  {
    lcd.setCursor(0, 1);
    lcd.print("Hay Chubascos");
  }
  else if (valor <= 600 && valor > 300)
  {
    
    lcd.setCursor(0, 1);
    lcd.print("Esta Lloviendo");
  }
  else
  {
    lcd.setCursor(0, 1);
    lcd.print("Lluvias Fuertes!");
  }
  
  delay(5000); // Esperamos 5 segundo para actualizar la pantalla

  // FUNCION PARA MOSTRAR LA HUMEDAD
  lcd.clear(); // Limpia el LCD
  lcd.setCursor(0, 0);
  lcd.print("Humedad:");
  lcd.print(dht.readHumidity());
  lcd.print("%");

  // SENSOR DE CALIDAD DEL AIRE 
  int sensorValue = analogRead(A1);
  Serial.print("C. Aire: ");
  Serial.print(sensorValue, DEC);
  Serial.println(" PPM");

  lcd.setCursor(0, 1); // Establece el cursor en la primera columna de la primera fila
  lcd.print("C. Aire:");
  lcd.print(sensorValue, DEC);
  lcd.print(" PPM");

  delay(5000);

  // FUNCION PARA LA VELOCIDAD DEL VIENTO

  float voltage = analogRead(ANEMOMETER_PIN) * (3.3 / 1023.0); // Leer el voltaje de la señal analógica y convertirlo a voltios
  float speed = (3.0 * PI * 0.42 / 0.6) * 0.7 / VOLTAGE_DIVIDER_RATIO * voltage; // Calcular la velocidad del viento en m/s

  lcd.clear(); // Borrar la pantalla
  lcd.setCursor(0, 0); // Colocar el cursor en la primera línea
  lcd.print("Velocidad del viento:"); // Mostrar el texto en la pantalla
  lcd.setCursor(0, 1); // Colocar el cursor en la segunda línea
  lcd.print(speed, 2); // Mostrar la velocidad en la pantalla con dos decimales
  lcd.print(" m/s");

  delay(5000);
}
