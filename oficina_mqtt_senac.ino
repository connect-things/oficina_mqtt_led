#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid     = "iotcel";
const char* password = "12348765";

const char* host = "iot.franciscocalaca.com";
const int port = 1883;

WiFiClient client;
PubSubClient MQTT(client);

void setup() {
  pinMode(2, OUTPUT);
  

  Serial.begin(9600);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());  

  MQTT.setServer(host, port);
  MQTT.setCallback(callback); 
  
  delay(700);
}

void callback(char* topic, byte* payload, unsigned int length){
  DynamicJsonBuffer jsonBuffer;
  String json = (char*)payload;
  JsonObject& rootRead = jsonBuffer.parseObject(json);
  long s = rootRead["number"];
  Serial.println(json);
  if(s != 1){
    digitalWrite(2, HIGH);
    Serial.println("...ligar");
  }else{
    digitalWrite(2, LOW);
    Serial.println("...desligar");
  }
}

void reconnect() {
  while (!MQTT.connected()) {
    if (MQTT.connect("ESP8266-Francisco-Aula")) {
      MQTT.subscribe("hello/world");
    } else {
      Serial.print(".");
      delay(3000);
    }
  }
}

void loop() {
    reconnect();
    MQTT.loop();

}
