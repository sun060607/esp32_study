#include <WiFi.h>

//인터넷 공유기 아이디 비밀번호이다!
const char* ssid = "bssm_free";
const char* password = "bssm_free";

const char* host = "10.150.150.185";
const int Port = 80;

WiFiClient client;

void setup() {
  //보드내부의 결과를 PC로 전송해서 확인하겠다!
  Serial.begin(115200);
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
  
}

void loop() {
  // put your main code here, to run repeatedly:
   /////인터넷 공유기 연결 완료/////
  //1.서버와 TCp연결을 한다
  if (!client.connect(host, Port)) {
    Serial.println("connection failed");
    return;
  }
  //2.서버에 request를 전송한다
  int num = 100;
  String url = "/bssm_2_4/upload.php?did=device1&temp=12.34&humi=50.11" + String(num);
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
  Serial.println("연결이 해제되었습니다.");
  delay(5000);
}
