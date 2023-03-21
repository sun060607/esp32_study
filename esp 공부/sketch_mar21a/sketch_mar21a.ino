void setup() {
  // put your setup code here, to run once:
  int num1 = 10;
  int num2 = 20;
  Serial.begin(9600);
  //pc에서 아두이노로 전송
  Serial.print("값은 = ");//CR 개행없이 전송
  Serial.println(num1+num2);//CR+LF 개행 있게 전송
  //케리지 리턴(cr)
  //Serial.write("문자열3");
//----------------------------------------------
  //문자열은 char 배열이다
  char text1 = "나는 문자열1이다";//123
  char *text2 = "나는 문자열2이다";//123
  String text3 = "나는 문자열3이다";//123
  text3 = text3 + "123";
  Serial.println(text3);////CR+LF 개행 있게 전송
//-------------------------------------------------
  String serverUrl= "http://19.168.0.1";//123
  int num3 = 123;
  serverUrl = serverUrl + num;
  Serial.println(serverUrl);////CR+LF 개행 있게 전송
  //------------------------------------------------
  String serverUrl2= "<html><input type = text><input type = button></html>";//123
  int num4 = 123;
  int first_pos = severUrl2.indexOf("<input type = b");//시작위치
  int second_pos  = severUrl2.indexOf("</html>");//시작위치
  String output  = severUrl2.substring(first_pos,second_pos);
  Serial.println("첫번재위치=");////CR+LF 개행 있게 전송
  Serial.println(first_pos);
  erial.println("두번재위치=");////CR+LF 개행 있게 전송
  Serial.println(second_pos);
  erial.println("결과=");////CR+LF 개행 있게 전송
  Serial.println(output);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
