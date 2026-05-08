# Nodo IoT de Clima y Ubicación: ESP32 + LoRa + GPS + BME280 🛰️☁️

Este proyecto convierte un ESP32 en un nodo sensor remoto capaz de leer datos meteorológicos (temperatura, humedad, presión), obtener su ubicación satelital precisa y transmitir toda esta información a larga distancia utilizando tecnología LoRa.

## 🛠️ Hardware Utilizado

* **Microcontrolador:** ESP32-WROOM-32
* **Sensor de Clima:** BME280 (I2C)
* **Módulo GPS:** GY-GPSV3-NEO (NEO-6M/7M/8M) (UART)
* **Módulo LoRa:** REYAX RYLR498 (UART)

## 🔌 Esquema de Conexiones (Wiring)

| Módulo | Pin del Módulo | Pin del ESP32 | Protocolo |
| :--- | :--- | :--- | :--- |
| **BME280** | VCC / GND | 3.3V / GND | Alimentación |
| **BME280** | SDA | GPIO 21 | I2C |
| **BME280** | SCL | GPIO 22 | I2C |
| **GPS NEO** | VCC / GND | 3.3V / GND | Alimentación |
| **GPS NEO** | TX | GPIO 16 (RX2) | UART 2 |
| **GPS NEO** | RX | GPIO 17 (TX2) | UART 2 |
| **LoRa RYLR** | VDD / GND | 3.3V / GND | Alimentación |
| **LoRa RYLR** | TXD | GPIO 4 (RX1) | UART 1 |
| **LoRa RYLR** | RXD | GPIO 5 (TX1) | UART 1 |

> **⚠️ Advertencia:** Nunca enciendas el módulo LoRa sin su antena conectada para evitar daños en el hardware.

## 📚 Librerías Requeridas

Para compilar este código en el Arduino IDE, necesitas instalar las siguientes librerías desde el Gestor de Bibliotecas:
1.  `Adafruit BME280 Library` (y sus dependencias de Adafruit Unified Sensor).
2.  `TinyGPSPlus` (por Mikal Hart).

## 🚀 Cómo usarlo

1. Realiza las conexiones de hardware según la tabla superior.
2. Clona este repositorio y abre el archivo principal en Arduino IDE.
3. Asegúrate de tener seleccionada la placa **ESP32 Dev Module**.
4. Compila y sube el código.
5. Abre el Monitor Serie a **115200 baudios**.
6. *Nota:* El GPS puede tardar un par de minutos en obtener señal satelital la primera vez. El LED del módulo parpadeará cuando tenga un "Fix".

## 📦 Formato de Datos Enviados

El módulo LoRa transmite una cadena de texto cada 5 segundos con el siguiente formato:
`T:25.40,H:50.00,P:1012.50,Lat:19.4326,Lng:-99.1332`