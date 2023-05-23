#include <WiFi.h>
#include <ArduinoJson.h>
#include <MQUnifiedsensor.h>
#define placa "Arduino UNO"
#define Voltage_Resolution 3.3
#define pin A0
#define type "MQ-6" //MQ6
#define ADC_Bit_Resolution 12
#define RatioMQ6CleanAir 10
//인터넷 공유기 아이디 비밀번호이다!
const char* ssid = "bssm_free";
const char* password = "bssm_free";
const char* host = "10.150.150.185";
const int Port = 80;
MQUnifiedsensor MQ6(placa, Voltage_Resolution, ADC_Bit_Resolution, pin, type);
/*
 * 
*/
WiFiClient client;

void setup() {
  //보드내부의 결과를 PC로 전송해서 확인하겠다!
  Serial.begin(115200);
  Serial.println(F("DHTxx test!"));
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
  /////인터넷 공유기 연결 완료/////
  //1.서버와 TCp연결을 한다
  MQ6.setRegressionMethod(1);
  MQ6.setA(2127.2); MQ6.setB(-2.526);
  MQ6.init();   
  Serial.print("Calibrating please wait.");
  float calcR0 = 0;
  for(int i = 1; i<=10; i ++)
  {
    MQ6.update();
    calcR0 += MQ6.calibrate(RatioMQ6CleanAir);
    Serial.print(".");
  }
  MQ6.setR0(calcR0/10);
  Serial.println("  done!.");
  
  if(isinf(calcR0)) {Serial.println("Warning: Conection issue, R0 is infinite (Open circuit detected) please check your wiring and supply"); while(1);}
  if(calcR0 == 0){Serial.println("Warning: Conection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply"); while(1);}
}

void loop() {
  MQ6.update();
  float data = MQ6.readSensor();
  int data2 = analogRead(39);
  // put your main code here, to run repeatedly:
   /////인터넷 공유기 연결 완료/////
  //1.서버와 TCp연결을 한다
  if (!client.connect(host, Port)) {
    Serial.println("connection failed");
    return;
  }
  //2.서버에 request를 전송한다
  int num = 100;
  String url = "/bssm_2_4/gas.php?gas="+String(data)+"&cds="+String(data2);
  //String url = "/test?id=6&data="+String(temp);
  //String url = "/bssm_2_4/control.php?did=device1";
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  //3.서버가 보낸 response를 수신한다
  unsigned long t = millis(); //생존시간
  while(1){
    if(client.available()) break;
    if(millis() - t > 10000) break;
  }
  //서버가 보낸 데이터가 버퍼에서 없어질때까지~
   while(client.available()){
    Serial.write(client.read());
  }
  //4.연결을 해제한다
  Serial.println("          ⠉⠲⣤⡀");
  Serial.println("⠀⠀⠀⠀⠀⠀⢀⣤⣀⣀⣀⠀⠻⣷⣄");
  Serial.println("⠀⠀⠀⠀⢀⣴⣿⣿⣿⡿⠋⠀⠀⠀⠹⣿⣦⡀");
  Serial.println("⠀⠀⢀⣴⣿⣿⣿⣿⣏⠀⠀⠀⠀⠀⠀⢹⣿⣧");
  Serial.println("⠀⠀⠙⢿⣿⡿⠋⠻⣿⣿⣦⡀⠀⠀⠀⢸⣿⣿⡆");
  Serial.println("⠀⠀⠀⠀⠉⠀⠀⠀⠈⠻⣿⣿⣦⡀⠀⢸⣿⣿⡇");
  Serial.println("⠀⠀⠀⠀⢀⣀⣄⡀⠀⠀⠈⠻⣿⣿⣶⣿⣿⣿⠁");
  Serial.println("⠀⠀⠀⣠⣿⣿⢿⣿⣶⣶⣶⣶⣾⣿⣿⣿⣿⡁");
  Serial.println("⢠⣶⣿⣿⠋⠀⠀⠛⠿⠿⠿⠿⠿⠛⠻⣿⣿⣦⡀");
  Serial.println("⣿⣿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⠀⠈⠻⣿⡿");
  delay(5000);
}
