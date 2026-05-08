#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

// --- Configuración BME280 (I2C) ---
Adafruit_BME280 bme; 

// --- Configuración GPS (UART2) ---
TinyGPSPlus gps;
HardwareSerial GPS_Serial(2); 
const int GPS_RX_Pin = 16;
const int GPS_TX_Pin = 17;
const uint32_t GPS_Baud = 9600;

// --- Configuración LoRa RYLR498 (UART1) ---
// Usaremos los pines 4 y 5 para dejar libres otros pines importantes
HardwareSerial LoRa_Serial(1);
const int LORA_RX_Pin = 4;
const int LORA_TX_Pin = 5;
// La mayoría de los módulos REYAX vienen a 115200 baudios por defecto
const uint32_t LORA_Baud = 115200; 

// --- Variables para el Temporizador ---
unsigned long tiempoAnterior = 0;
const long intervalo = 5000; // Enviar datos cada 5 segundos (5000 ms)

void setup() {
  Serial.begin(115200);
  while(!Serial);
  
  Serial.println(F("Iniciando Nodo LoRa + GPS + Clima..."));

  // 1. Iniciar BME280
  if (!bme.begin(0x76)) {
    Serial.println(F("Error: BME280 no encontrado."));
    while (1) delay(10);
  }

  // 2. Iniciar GPS
  GPS_Serial.begin(GPS_Baud, SERIAL_8N1, GPS_RX_Pin, GPS_TX_Pin);
  
  // 3. Iniciar LoRa
  LoRa_Serial.begin(LORA_Baud, SERIAL_8N1, LORA_RX_Pin, LORA_TX_Pin);
  
  Serial.println(F("Todos los módulos iniciados."));
  Serial.println(F("==================================="));
}

void loop() {
  // 1. Mantener actualizado el GPS (NO BLOQUEANTE)
  while (GPS_Serial.available() > 0) {
    gps.encode(GPS_Serial.read());
  }

  // 2. Revisar si el módulo LoRa nos responde algo (para depuración)
  while (LoRa_Serial.available() > 0) {
    Serial.write(LoRa_Serial.read());
  }

  // 3. Temporizador para leer y enviar datos
  unsigned long tiempoActual = millis();
  if (tiempoActual - tiempoAnterior >= intervalo) {
    tiempoAnterior = tiempoActual; 

    // --- Recopilar datos ---
    float temperatura = bme.readTemperature();
    float humedad = bme.readHumidity();
    float presion = bme.readPressure() / 100.0F;
    
    float latitud = 0.0;
    float longitud = 0.0;
    
    if (gps.location.isValid()) {
      latitud = gps.location.lat();
      longitud = gps.location.lng();
    }

    // --- Empaquetar datos en un String ---
    // Formato: T:25.40,H:50.00,P:1012.50,Lat:19.4326,Lng:-99.1332
    String datos = "T:" + String(temperatura, 2) + 
                   ",H:" + String(humedad, 2) + 
                   ",P:" + String(presion, 2) + 
                   ",Lat:" + String(latitud, 6) + 
                   ",Lng:" + String(longitud, 6);

    Serial.println("Preparando envío LoRa: " + datos);

    // --- Enviar por LoRa (Comando AT de REYAX) ---
    // El formato es: AT+SEND=<Dirección>,<Tamaño_Datos>,<Datos>
    // Dirección 0 significa "Broadcast" (Enviar a todos los que escuchen)
    
    LoRa_Serial.print("AT+SEND=0,");
    LoRa_Serial.print(datos.length());
    LoRa_Serial.print(",");
    LoRa_Serial.println(datos); // println añade el \r\n que el módulo necesita para ejecutar el comando

    Serial.println(F("==================================="));
  }
}