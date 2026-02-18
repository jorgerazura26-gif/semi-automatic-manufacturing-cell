#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define SS_PIN 5
#define RST_PIN 27
#define SENSOR_PIN 34

//const char* ssid = "Admon";
//const char* password = "TP-pat@86";
const char* ssid = "Tec-IoT";
const char* password = "spotless.magnetic.bridge";
//const char* mqtt_server = "192.168.1.70";
//const char* mqtt_server = "192.168.30.25";
const char* mqtt_server = "10.25.88.167";
const char* mqtt_topic = "corriente";

// Lista de tarjetas autorizadas
const byte tarjetas_autorizadas[][4] = {
    {0x5D, 0xD6, 0xFC, 0x03}, // UID de tarjeta 1
    {0xAB, 0xCD, 0xEF, 0x01}  // UID de tarjeta 2
};

WiFiClient espClient;
PubSubClient client(espClient);
MFRC522 rfid(SS_PIN, RST_PIN);

bool accesoPermitido = false;
float corrienteMedida = 0.0;
unsigned long tiempoAnterior = 0;  // Variable para controlar el tiempo

long lastMsg = 0;
char msg[50];
float value = 0;
float Dato_Enviar = 0;


void setup() {
    Serial.begin(9600);
    SPI.begin();
    rfid.PCD_Init();

    // Conectar a WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi conectado!");

    // Conectar a MQTT
    client.setServer(mqtt_server, 1883);
    while (!client.connected()) {
        if (client.connect("ESP32_RFID")) {
            Serial.println("Conectado a MQTT!");
        } else {
            Serial.print("Error MQTT. Código: ");
            Serial.println(client.state());
            delay(2000);
        }
    }

    Serial.println("Acerca una tarjeta al lector...");
    client.publish("rfid", "Acerca una tarjeta al lector...");
}

void sensor(){
   
}

void loop() {
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
        return;
    }

    String uid_str = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
        uid_str += String(rfid.uid.uidByte[i], HEX);
        if (i < rfid.uid.size - 1) uid_str += ":";
    }

    Serial.print("UID de la tarjeta: ");
    Serial.println(uid_str);

    accesoPermitido = verificarAcceso(rfid.uid.uidByte);
    if (accesoPermitido) {
        Serial.println("Acceso permitido. Bienvenido!");
        client.publish("rfid", "Acceso permitido");    
        //sensor();
        while (accesoPermitido){
          float Irms=get_corriente(); //Calls the function that obtains the efective current
          float P=Irms*220.0; // P=IV (Watts)
          char corriente2[16];
        dtostrf(Irms, 5, 3, corriente2);
        client.publish("corriente", corriente2);
          delay(1000);
            loop();
        }
    } else {
        Serial.println("Acceso denegado");
        client.publish("rfid", "Acceso denegado");
        return;
    }
    rfid.PICC_HaltA();
}

bool verificarAcceso(byte *uid) {
    for (int i = 0; i < sizeof(tarjetas_autorizadas) / sizeof(tarjetas_autorizadas[0]); i++) {
        bool coincide = true;
        for (byte j = 0; j < 4; j++) {
            if (tarjetas_autorizadas[i][j] != uid[j]) {
                coincide = false;
                break;
            }
        }
        if (coincide) return true;
    }
    return false;
}
float get_corriente()
{
  float voltajeSensor;
  float corriente = 0;
  float sumatoria = 0;
  long tiempo = millis();
  int N = 0;
  
  float offset = 0;
  int totalMuestras = 0;
  float sumaOffset = 0;

  // Calculate the average of the offset 
  while (millis() - tiempo < 500) {
    float lectura = analogRead(34);
    sumaOffset += lectura;
    totalMuestras++;
  }
  offset = sumaOffset / totalMuestras;

  // Converts the offset into voltaje
  offset = offset * (3.3 / 4095.0);

  tiempo = millis();
  N = 0;
  sumatoria = 0;

  while (millis() - tiempo < 500) {
		// Converts the value obtained from 0 to 4095 into real voltage    
    voltajeSensor = analogRead(34) * (3.3 / 4095.0);
    voltajeSensor -= offset;  // Apply the average offset 
		// Adjust the conversion of the sensor, this was calculated in a practical way
    if (0.10<voltajeSensor<0.16){
        corriente = (voltajeSensor*2)*10;
    }
    else if (0.18<voltajeSensor<0.20){
        corriente = 71.73*voltajeSensor-9.34+4+0.3;
    }
		//Calculate the effective current (RMS) with the formula (square root(1/N*sum(N,n=0)i^2))
    sumatoria += sq(corriente); 
    N++;
  }

  return sqrt(sumatoria/N);  // Corriente RMS
}