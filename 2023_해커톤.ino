#include <WiFi.h>
#include <ArduinoJson.h>
const char* ssid = "bssm_free";
const char* password = "bssm_free";
const char* host = "10.150.149.66";
const int Port = 8000;
WiFiClient client;
int sound_analog1 = 2;
int sound_analog2 = 4;

void setup() {
  Serial.begin(115200);
  Serial.println(F("DHTxx test!"));
  pinMode(16,OUTPUT);
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
}

void loop() {
  if (!client.connect(host, Port)) {
    Serial.println("connection failed");
    return;
  }
  int val_analog1 = analogRead(sound_analog1);
  int val_analog2 = analogRead(sound_analog2);
  int num = (val_analog1 + val_analog2)/2;
  
  String url = "https://10.150.149.66:8000/api/sound?sound="+String(num);
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long t = millis();
  while(1){
    if(client.available()) break;
    if(millis() - t > 10000) break;
  }
  while(client.available()){
    String line = client.readStringUntil('\n');
    if(line.indexOf("{\"pin\"") != -1){
      Serial.println(line);
      StaticJsonDocument<48> doc;
      DeserializationError error = deserializeJson(doc,line);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }
      int pin = doc["pin"]; // 16
      int cmd = doc["cmd"]; // 0
      if(pin==-1){
        Serial.println("아무것도 제어할 필요가 없음");
      }else{
        Serial.println(pin);
        Serial.println(cmd);
        digitalWrite(pin,cmd);
      }
    }
  }
  //4.연결을 해제한다
  Serial.println("연결이 해제되었습니다.");
  delay(5000);
}
