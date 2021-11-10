// Modulo lectura Sensores
// Código combinado sensores DHT22, YL 69, DSB 1820 y PH Sen 0161.

// Definiciones DHT
#include "DHT.h"

#define DHTPIN 32     

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

// Definiciones YL-69
#ifdef __cplusplus
extern "C" {
#endif
uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();

// use G23 of ESP32 to read data
const int YL69Pin = 14;

// Definiciones DSB 18B20
#include <OneWire.h>
#include <DallasTemperature.h>

// GPIO where the DS18B20 is connected to
const int oneWireBus = 33;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

// Definiciones SEN 0161
const byte pHpin = 0;// Connect the sensor's Po output to analogue pin 0.
float Po;

// Variables acumuladoras y promedios
float temperatureC; 
float temperatureF;
float h;
float t;
float f;
float hic;
float hif;
int convertedPercentage;

float acum_temp_amb_celcius = 0;
float acum_hum_Ambiente_Array = 0;
float acum_temp_suelo_celcius_Array = 0;
float acum_temp_suelo_celcius_Farenheit = 0;
float acum_temp_amb_farenheit = 0;
float acum_sensa_termi_celcius_Array = 0;
float acum_termi_faren_Array= 0;
float acum_humedad_suelo = 0;
float acum_ph_liqui = 0;

float acum_temp_amb_celcius_lte = 0;
float acum_hum_Ambiente_Array_lte = 0;
float acum_temp_suelo_celcius_Array_lte = 0;
float acum_temp_suelo_celcius_Farenheit_lte = 0;
float acum_temp_amb_farenheit_lte = 0;
float acum_sensa_termi_celcius_Array_lte = 0;
float acum_termi_faren_Array_lte = 0;
float acum_humedad_suelo_lte = 0;
float acum_ph_liqui_lte = 0;

float temp_amb_celcius_prom = 0;
float hum_Ambiente_Array_prom  = 0;
float temp_suelo_celcius_Array_prom = 0;
float temp_suelo_celcius_Farenheit_prom = 0;
float temp_amb_farenheit_prom = 0;
float sensa_termi_celcius_Array_prom = 0; 
float sensa_termi_faren_Array_prom  = 0;
float humedad_suelo_prom  = 0;
float ph_liqui_prom  = 0;

float temp_amb_celcius_prom_lte = 0;
float hum_Ambiente_Array_prom_lte  = 0;
float temp_suelo_celcius_Array_prom_lte = 0;
float temp_suelo_celcius_Farenheit_prom_lte = 0;
float temp_amb_farenheit_prom_lte = 0;
float sensa_termi_celcius_Array_prom_lte = 0; 
float sensa_termi_faren_Array_prom_lte  = 0;
float humedad_suelo_prom_lte  = 0;
float ph_liqui_prom_lte  = 0;

// Variables modo sleep 
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */

//Configuración de parámetro de tiempo de dormida del procesador 60= 1 segundo

#define TIME_TO_SLEEP  180       /* Time ESP32 will go to sleep (in seconds) */

RTC_DATA_ATTR int bootCount = 0;
int bootCount1 = 0;
int bootCount2 = 0;

//int bootCount1 = 0;

//Conexión Wiffi
#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif

//Credenciales de red wifi a la cuál se conectará el módulo
#include <Wire.h>
// Replace with your network credentials (Credenciales SSID y Password)
const char* ssid     = " ";// Nombre de red o ssid
const char* password = "} ";// Contraseña o credencial de acceso a la red

// REPLACE with your Domain name and URL path or IP address with path
// const char* serverName = "http://testiot.atwebpages.com/post-data.php";
const char* serverName = "http://pruebas.testiot.co/post-data.php";

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-data.php also needs to have the same key 
String apiKeyValue = "tPmAT5Ab3j7F9";

// Configure TinyGSM library
// SIM card PIN (leave empty, if not defined)
const char simPIN[]   = "";

// Your phone number to send SMS: + (plus sign) and country code, for Portugal +351, followed by phone number
// SMS_TARGET Example for Portugal +351XXXXXXXXX
#define SMS_TARGET  " " // Información de la SIM del usuario al cuál se el enviarán el sms con los resultados


// Your GPRS credentials (leave empty, if not needed)
const char apn[]      = "web.colombiamovil.com.co"; // APN (example: internet.vodafone.pt) use https://wiki.apnchanger.org
const char gprsUser[] = ""; // GPRS User
const char gprsPass[] = ""; // GPRS Password
 

// Servidor de almacenamiento de datos
// The server variable can be just a domain name or it can have a subdomain. It depends on the service you are using
const char server[] = "pruebas.testiot.co"; // domain name: example.com, maker.ifttt.com, etc
const char resource[] = "/post-data.php";         // resource path, for example: /post-data.php
const int  port = 80; // server port number

// Librerias y definiciones GSM- Comandos AT-4GLTE
// Configure TinyGSM library
#define TINY_GSM_MODEM_SIM7600      // Modem is SIM800
#define TINY_GSM_RX_BUFFER   1024  // Set RX buffer to 1Kb

#include <Wire.h>
#include <TinyGsmClient.h>

// TTGO T-Call pins... pines de la sp32 usados por módulo sim 7600 SA
#define MODEM_RST            5
#define MODEM_PWKEY          4
#define MODEM_POWER_ON       25
#define MODEM_TX             27
#define MODEM_RX             26
#define I2C_SDA              21
#define I2C_SCL              22

// Set serial for debug console (to Serial Monitor, default speed 115200)
#define SerialMon Serial
// Set serial for AT commands (to SIM800 module)
#define SerialAT Serial1

// Configure TinyGSM library
#define TINY_GSM_MODEM_SIM7600      // Modem is SIM800
#define TINY_GSM_RX_BUFFER   1024  // Set RX buffer to 1Kb

// Define the serial console for debug prints, if needed
//#define DUMP_AT_COMMANDS

#include <Wire.h>
#include <TinyGsmClient.h>

#ifdef DUMP_AT_COMMANDS
  #include <StreamDebugger.h>
  StreamDebugger debugger(SerialAT, SerialMon);
  TinyGsm modem(debugger);
#else
  TinyGsm modem(SerialAT);
#endif

// I2C for SIM800 (to keep it running when powered from battery)
TwoWire I2CPower = TwoWire(0);

// TinyGSM Client for Internet connection
TinyGsmClient client(modem);


#define TIME_TO_SLEEP  3600        /* Time ESP32 will go to sleep (in seconds) 3600 seconds = 1 hour */

#define IP5306_ADDR          0x75
#define IP5306_REG_SYS_CTL0  0x00

bool setPowerBoostKeepOn(int en){
  I2CPower.beginTransmission(IP5306_ADDR);
  I2CPower.write(IP5306_REG_SYS_CTL0);
  if (en) {
    I2CPower.write(0x37); // Set bit1: 1 enable 0 disable boost keep on
  } else {
    I2CPower.write(0x35); // 0x37 is default reg value
  }
  return I2CPower.endTransmission() == 0;
}

// cadenas de texto de entrega de resultados vía SMS
String test = "Wiffi: temperatura ambiente en farenheit ";
String test1 = "humedad ambiente  ";
String test2 = "temperatura suelo celcius  ";
String test3 = "temperatura suelo   farenheit";
String test4 = "temperatura ambiente celcius  ";
String test5 = "sensacion termica celcius   ";
String test6 = "sensacion termica farenheit  ";
String test7 = "humedad suelo  ";
String test8 = "PH  ";
String test9 = "4G LTE: temperatura ambiente en farenheit  ";
String test10 = "4G LTE:humedad ambiente  ";
String test11 = "4G LTE:temperatura suelo celcuis  ";
String test12 = "4G LTE:temperatura suelo farenheit  ";
String test13 = "4G LTE:temperatura ambiente farenheit  ";
String test14 = "4G LTE:sensacion termica celcius   ";
String test15 = "4G LTE:sensacion termica farenheit  ";
String test16 = "4G LTE:humedad suelo  ";
String test17 = "4G LTE:PH  ";


void setup() {
  Serial.begin(115200);
  // Inicialización librerías
  sensors.begin();
  dht.begin();
    
  }

  

void loop(){
  if(bootCount == 0) //Conteo de inicio del módulo
  {
      
      bootCount = bootCount+1;
  }else
  {
        
        // código wifi
        Serial.println("Código wifi");

        // Set modem reset, enable, power pins
        pinMode(MODEM_PWKEY, OUTPUT);
        pinMode(MODEM_RST, OUTPUT);
        pinMode(MODEM_POWER_ON, OUTPUT);
        digitalWrite(MODEM_PWKEY, LOW);
        digitalWrite(MODEM_RST, LOW);
        digitalWrite(MODEM_POWER_ON, LOW);

        // Lectura sensor DHT
      float h = dht.readHumidity();
      // Read temperature as Celsius (the default)
      float t = dht.readTemperature();
      // Read temperature as Fahrenheit (isFahrenheit = true)
      float f = dht.readTemperature(true);

      // Check if any reads failed and exit early (to try again).
      if (isnan(h) || isnan(t) || isnan(f)) {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
      }

      // Compute heat index in Fahrenheit (the default)
      float hif = dht.computeHeatIndex(f, h);
      // Compute heat index in Celsius (isFahreheit = false)
      float hic = dht.computeHeatIndex(t, h, false);

  
      // Lectura YL-69
      int const readYL69value = analogRead(YL69Pin);
      // map inversely to 0..10%
      int const convertedPercentage = map(readYL69value, 4095, 1200, 0, 100);
  

      // Lectura DSB 18B20
      sensors.requestTemperatures(); 
      float temperatureC = sensors.getTempCByIndex(0);
      float temperatureF = sensors.getTempFByIndex(0);
      
      // ciclo repetitivo primeras 10 muestras con comunicación wifi

      while(bootCount1 <= 9)
    {
      if (bootCount1 == 0){

        // Conexión red wifi
        WiFi.begin(ssid, password);
        Serial.println("Connecting");
        
  
        while(WiFi.status() != WL_CONNECTED) { 
              delay(500);
              Serial.print(".");
            }
          Serial.println("");
          Serial.print("Connected to WiFi network with IP Address: ");
          Serial.println(WiFi.localIP());
        }
          
          if(WiFi.status()== WL_CONNECTED){
            // Conexión al servidor web diseñado
            HTTPClient http;
    
            // Your Domain name with URL path or IP address with path
            http.begin(serverName);
    
            // Specify content-type header
            http.addHeader("Content-Type", "application/x-www-form-urlencoded");
            
            // acumuladores de las muestras tomadas para promedio

            Po = (1023 - analogRead(pHpin)) / 73.07; // Read and reverse the analogue input value from the pH sensor then scale 0-14.

            acum_humedad_suelo = acum_humedad_suelo + convertedPercentage;
            acum_temp_suelo_celcius_Array = acum_temp_suelo_celcius_Array + temperatureC;
            acum_temp_suelo_celcius_Farenheit = acum_temp_suelo_celcius_Farenheit + temperatureF;
            acum_hum_Ambiente_Array = acum_hum_Ambiente_Array + h;
            acum_temp_amb_celcius = acum_temp_amb_celcius + f;
            acum_temp_amb_farenheit = acum_temp_amb_farenheit + t;
            acum_sensa_termi_celcius_Array = acum_sensa_termi_celcius_Array + hic; 
            acum_termi_faren_Array = acum_termi_faren_Array + hif;
            acum_ph_liqui = acum_ph_liqui + Po;

            // Muestra de las lecturas en el shell 

            Serial.println("PH Sen 0161 Lectura");
            Serial.println(Po, 2);// Print the result in the serial monitor.

            Serial.print(F("DSB 18B20 lectura: "));
            Serial.print(temperatureC);
            Serial.println("ºC");
            Serial.print(temperatureF);
            Serial.println("ºF");

            Serial.print("Lectura (YL-69): ");
            Serial.print(convertedPercentage);
            Serial.print("%\n");

            Serial.print(F("DHT lectura: "));
            Serial.print(F("Humedad: "));
            Serial.print(h);
            Serial.print(F("%  Temperatura: "));
            Serial.print(t);
            Serial.print(F("°C "));
            Serial.print(f);
            Serial.print(F("°F  sensación térmica: "));
            Serial.print(hic);
            Serial.print(F("°C "));
            Serial.print(hif);
            Serial.println(F("°F"));
            
            // Enpaquetamiento de datos a enviar
            // Prepare your HTTP POST request data
            String httpRequestData = "api_key=" + apiKeyValue + "&value1=" + String(temperatureC) + "&value2=" + String(temperatureF) + "&value3=" + String(h) + "&value4=" + String(t) + "&value5=" + String(f) + "&value6=" + String(hic) + "&value7=" + String(hif) + "&value8=" + String(convertedPercentage) + "&value9=" + String(Po);
            Serial.print("httpRequestData: ");
            Serial.println(httpRequestData);

              //    Cálculos de los promedios
              temp_amb_celcius_prom = acum_temp_amb_celcius/10;
              hum_Ambiente_Array_prom  = acum_hum_Ambiente_Array/10;
              temp_suelo_celcius_Array_prom = acum_temp_suelo_celcius_Array/10;
              temp_suelo_celcius_Farenheit_prom = acum_temp_suelo_celcius_Farenheit/10;
              temp_amb_farenheit_prom = acum_temp_amb_farenheit/10;
              sensa_termi_celcius_Array_prom = acum_sensa_termi_celcius_Array/10; 
              sensa_termi_faren_Array_prom  = acum_termi_faren_Array/10;
              humedad_suelo_prom  = acum_humedad_suelo/10;
              ph_liqui_prom  = acum_ph_liqui/10;
        
            
            // Envío de datos al servidor
            // Send HTTP POST request
            int httpResponseCode = http.POST(httpRequestData);

            if (httpResponseCode>0) {
              Serial.print("HTTP Response code: ");
              Serial.println(httpResponseCode);
            }
            else {
              Serial.print("Error code: ");
              Serial.println(httpResponseCode);
            }

            if(bootCount1 == 9){

              // Muestra de los promedios en el shell
              Serial.println("Promedios wiffi");
              Serial.print(F("Humedad suelo: "));
              Serial.print(humedad_suelo_prom);
              Serial.print(F("%  Temperatura: "));
              Serial.print(temp_suelo_celcius_Array_prom);
              Serial.print(F("°C "));
              Serial.print(temp_suelo_celcius_Farenheit_prom);
              Serial.print(F("°F  Sensación térmica: "));
              Serial.print(sensa_termi_celcius_Array_prom);
              Serial.print(F("°C "));
              Serial.print(sensa_termi_faren_Array_prom);
              Serial.println(F("°F"));
              Serial.println("PH promedio");
              Serial.println(ph_liqui_prom);
              http.end();
              Serial.println("WiFi Disconnected");

              }

      }// se cierra while de conexión

        bootCount1++;
        bootCount2++;
    }// se cierra el while  


        
        //bootCount1++;
        //bootCount2++;

    }

    
  
    // Ciclo repetitivo para envío de 10 muestras con tecnología 4G LTE
    while(bootCount1 >= 10 && bootCount1 < 20)
    {
        // código 4GLTE
        Serial.println("Conexión 4GLTE");

      pinMode(MODEM_PWKEY, OUTPUT);
      pinMode(MODEM_RST, OUTPUT);
      pinMode(MODEM_POWER_ON, OUTPUT);
      digitalWrite(MODEM_PWKEY, LOW);
      digitalWrite(MODEM_RST, LOW);
      digitalWrite(MODEM_POWER_ON, LOW);

       // Lectura sensor DHT
      float h = dht.readHumidity();
      // Read temperature as Celsius (the default)
      float t = dht.readTemperature();
      // Read temperature as Fahrenheit (isFahrenheit = true)
      float f = dht.readTemperature(true);

      // Check if any reads failed and exit early (to try again).
      if (isnan(h) || isnan(t) || isnan(f)) {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
      }

      // Compute heat index in Fahrenheit (the default)
      float hif = dht.computeHeatIndex(f, h);
      // Compute heat index in Celsius (isFahreheit = false)
      float hic = dht.computeHeatIndex(t, h, false);


      // Lectura YL-69
      int const readYL69value = analogRead(YL69Pin);
      // map inversely to 0..10%
      int const convertedPercentage = map(readYL69value, 4095, 1200, 0, 100);
   
      
      // DSB
      sensors.requestTemperatures(); 
      float temperatureC = sensors.getTempCByIndex(0);
      float temperatureF = sensors.getTempFByIndex(0);

      
      Po = (1023 - analogRead(pHpin)) / 73.07; // Read and reverse the analogue input value from the pH sensor then scale 0-14.

      // Pines del modo reset, enable y pin de potencia para la transmisión
        // Set modem reset, enable, power pins
      pinMode(MODEM_PWKEY, OUTPUT);
      pinMode(MODEM_RST, OUTPUT);
      pinMode(MODEM_POWER_ON, OUTPUT);
      digitalWrite(MODEM_PWKEY, LOW);
      digitalWrite(MODEM_RST, HIGH);
      digitalWrite(MODEM_POWER_ON, HIGH);

        // Set GSM module baud rate and UART pins
      SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
      delay(3000);

      //modem.restart();
      // use modem.init() if you don't need the complete restart

      // Unlock your SIM card with a PIN if needed
      if (strlen(simPIN) && modem.getSimStatus() != 3 ) {
        modem.simUnlock(simPIN);
      }

      String result;

    do {
            result = modem.setNetworkMode(2);
            delay(500);
        } while (result != "OK");

    // ciclo repetitivo para empaquetamiento y envío de datos
    while(bootCount1 >= 10 && bootCount1 < 20)
    {
      SerialMon.print("Connecting to APN: ");
      SerialMon.print(apn);
      if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
        SerialMon.println(" fail");
      }
      else {
        SerialMon.println(" OK");
        SerialMon.print("Connecting to ");
        SerialMon.print(server);
        if (!client.connect(server, port)) {
          SerialMon.println(" fail");
        }
        else {
          SerialMon.println(" OK");
          SerialAT.println("AT+CPSI?");     //Get connection type and band
      delay(500);
      if (SerialAT.available()) {
        String r = SerialAT.readString();
        Serial.println(r);
      }
      //
            // Variables acumuladoras valores de las muestras
            acum_humedad_suelo_lte = acum_humedad_suelo_lte + convertedPercentage;
            acum_temp_suelo_celcius_Array_lte = acum_temp_suelo_celcius_Array_lte + temperatureC;
            acum_temp_suelo_celcius_Farenheit_lte = acum_temp_suelo_celcius_Farenheit_lte + temperatureF;
            acum_hum_Ambiente_Array_lte = acum_hum_Ambiente_Array_lte + h;
            acum_temp_amb_celcius_lte = acum_temp_amb_celcius_lte + f;
            acum_temp_amb_farenheit_lte = acum_temp_amb_farenheit_lte + t;
            acum_sensa_termi_celcius_Array_lte = acum_sensa_termi_celcius_Array_lte + hic; 
            acum_termi_faren_Array_lte = acum_termi_faren_Array_lte + hif;
            acum_ph_liqui_lte = acum_ph_liqui_lte + Po;

      // Muesta de resultados en el shell
      Serial.println("PH Sen 0161 Lectura");
            Serial.println(Po, 2);// Print the result in the serial monitor.

            Serial.print(F("DSB 18B20 lectura: "));
            Serial.print(temperatureC);
            Serial.println("ºC");
            Serial.print(temperatureF);
            Serial.println("ºF");

            Serial.print("Lectura (YL-69): ");
            Serial.print(convertedPercentage);
            Serial.print("%\n");

            Serial.print(F("DHT lectura: "));
            Serial.print(F("Humedad: "));
            Serial.print(h);
            Serial.print(F("%  Temperatura: "));
            Serial.print(t);
            Serial.print(F("°C "));
            Serial.print(f);
            Serial.print(F("°F  sensación térmica: "));
            Serial.print(hic);
            Serial.print(F("°C "));
            Serial.print(hif);
            Serial.println(F("°F"));

            // Empaquetamiento de datos para la transmisión

      // Making an HTTP POST request
      SerialMon.println("Performing HTTP POST request...");
      // Prepare your HTTP POST request data (Temperature in Celsius degrees)
      String httpRequestData = "api_key=" + apiKeyValue + "&value1=" + String(temperatureC) + "&value2=" + String(temperatureF) + "&value3=" + String(h) + "&value4=" + String(t) + "&value5=" + String(f) + "&value6=" + String(hic) + "&value7=" + String(hif) + "&value8=" + String(convertedPercentage) + "&value9=" + String(Po) ;
      
      client.print(String("POST ") + resource + " HTTP/1.1\r\n");
      client.print(String("Host: ") + server + "\r\n");
      client.println("Connection: close");
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.print("Content-Length: ");
      client.println(httpRequestData.length());
      client.println();
      client.println(httpRequestData);

      unsigned long timeout = millis();
      while (client.connected() && millis() - timeout < 10000L) {
        // Print available data (HTTP response from server)
        while (client.available()) {
          char c = client.read();
          SerialMon.print(c);
          timeout = millis();
        }
      }
      SerialMon.println();
      
      // Cálculos de los promedios
      temp_amb_celcius_prom_lte = acum_temp_amb_celcius_lte/10;
      hum_Ambiente_Array_prom_lte  = acum_hum_Ambiente_Array_lte/10;
      temp_suelo_celcius_Array_prom_lte = acum_temp_suelo_celcius_Array_lte/10;
      temp_suelo_celcius_Farenheit_prom_lte = acum_temp_suelo_celcius_Farenheit_lte/10;
      temp_amb_farenheit_prom_lte = acum_temp_amb_farenheit_lte/10;
      sensa_termi_celcius_Array_prom_lte = acum_sensa_termi_celcius_Array_lte/10; 
      sensa_termi_faren_Array_prom_lte  = acum_termi_faren_Array_lte/10;
      humedad_suelo_prom_lte  = acum_humedad_suelo_lte/10;
      ph_liqui_prom_lte  = acum_ph_liqui_lte/10;

      
      
    //delay(250);
    while (bootCount1 == 19) {
      // Close client and disconnect
      client.stop();
      SerialMon.println(F("Server disconnected"));
      modem.gprsDisconnect();
      SerialMon.println(F("GPRS disconnected"));
      bootCount1++;
      
    }
 
      
      }
      }
        
        bootCount1++;
        bootCount2++;

    }

      // Muestra de los promedios en la shell
      Serial.println("Promedios 4GLTE");
      Serial.print(F("Humedad: "));
      Serial.print(humedad_suelo_prom_lte);
      Serial.print(F("%  Temperatura: "));
      Serial.print(temp_suelo_celcius_Array_prom_lte);
      Serial.print(F("°C "));
      Serial.print(temp_suelo_celcius_Farenheit_prom_lte);
      Serial.print(F("°F  Sensación térmica: "));
      Serial.print(sensa_termi_celcius_Array_prom_lte);
      Serial.print(F("°C "));
      Serial.print(sensa_termi_faren_Array_prom_lte);
      Serial.println(F("°F"));
  
       // Envío de datos promedios al usuario mediante mensaje de texto SMS
       
       if (strlen(simPIN) && modem.getSimStatus() != 3 ) {
          modem.simUnlock(simPIN);
        }
      //Organización datos sms
        String smsMessage = test + temp_amb_celcius_prom + "\n" + test1 + hum_Ambiente_Array_prom + "\n" + test2 + temp_suelo_celcius_Array_prom + "\n" + test3 + temp_suelo_celcius_Farenheit_prom;
        if(modem.sendSMS(SMS_TARGET, smsMessage)){
        SerialMon.println(smsMessage);
        }
        else{
          SerialMon.println("SMS failed to send");
        }
        String smsMessage1 = test4 + temp_amb_farenheit_prom + "\n" + test5 + sensa_termi_celcius_Array_prom  + "\n" + test6 + sensa_termi_faren_Array_prom + "\n" + test7+ humedad_suelo_prom;
        if(modem.sendSMS(SMS_TARGET, smsMessage1)){
        SerialMon.println(smsMessage1);
        }
        else{
          SerialMon.println("SMS failed to send");
        }
        String smsMessage2 = test8 + ph_liqui_prom;
        if(modem.sendSMS(SMS_TARGET, smsMessage2)){
        SerialMon.println(smsMessage2);
        }
        else{
          SerialMon.println("SMS failed to send");
        }

        String smsMessage3 = test9 + temp_amb_celcius_prom_lte + "\n" + test10 + hum_Ambiente_Array_prom_lte + "\n" + test11 + temp_suelo_celcius_Array_prom_lte + "\n" + test12 + temp_suelo_celcius_Farenheit_prom_lte;
        if(modem.sendSMS(SMS_TARGET, smsMessage3)){
        SerialMon.println(smsMessage3);
        }
        else{
          SerialMon.println("SMS failed to send");
        }
        String smsMessage4 = test13 + temp_amb_farenheit_prom_lte + "\n" + test14 + sensa_termi_celcius_Array_prom_lte  + "\n" + test15 + sensa_termi_faren_Array_prom_lte + "\n" + test16+ humedad_suelo_prom_lte;
        if(modem.sendSMS(SMS_TARGET, smsMessage4)){
        SerialMon.println(smsMessage4);
        }
        else{
          SerialMon.println("SMS failed to send");
        }
        String smsMessage5 = test17 + ph_liqui_prom_lte;
        if(modem.sendSMS(SMS_TARGET, smsMessage5)){
        SerialMon.println(smsMessage5);
        }
        else{
          SerialMon.println("SMS failed to send");
        }
        

      delay(120000); //acá se modifica el tiempo de periodo despierto  del procesador 300000 = 5 min
      
  }
  
  delay(3000);


  // Funciones para dormir y despertar el procesador
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}

  
   

      


  
  

  
