unsigned long t1 = 0;//led를 제어하는데 필요한 1초라는 타이밍
unsigned long t2 = 0;//millis 반환하는 값이기 때문
unsigned long t3 = 0;
unsigned long t4 = 0;
int btn_cnt = 0;
int motion_cnt = 0;//
bool old_btn = HIGH;
bool old_motion = LOW;
void setup() {
  Serial.begin(115200);
  pinMode(16,OUTPUT);
  pinMode(17,INPUT);
  pinMode(5,INPUT);
}

void loop() {
  //모션센서의 상승엣지 섬출
  bool now_motion = digitalRead(5);
  if(old_motion == LOW && now_motion == HIGH){
    //하강엣지
    //btn_cnt++;
    motion_cnt++;
  }
  old_motion = now_motion;
  //led가 켜졌다면 3초뒤에 끄ㅔ겠다
  if(bool state = digitalRead(16) == HIGH){
    if(millis() - t4 > 3000){
    digitalWrite(16,LOW);
  }
  }
  //눌렀을때 low면 pullup
  bool now_btn = digitalRead(17);//n번째
  //now_btn = n번째 상태
  //old_btn = n-1번째
  //직전 상태가 high였는데 현재 상태가 low이다 == 하강엣지 == 버튼이 눌러짐
  if(old_btn == HIGH && now_btn == LOW){
    //하강엣지
    //btn_cnt++;
    t3 = millis();
  }else if(old_btn == LOW && now_btn == HIGH){
    //상승 엣지 누리고 있다가 누름을 해제하는 그시점
    Serial.println("버튼 누른 시간");
    Serial.println(millis() - t3);
    if(millis() - t3>50){
      //버튼 눌러진 것으로 인정하겠다
      digitalWrite(16,HIGH);
      t4 = millis();
      
      btn_cnt++;
    }
    //누름-안누름사이의 사간의 출력
  }
  old_btn = now_btn;
  /*
  if(now_btn==0){
    btn_cnt++;
  }*/
  //메인 루프 안에 2가지이상의 작업이 있을 경우 delay함수는 사용 불가능하다
  /*
  if(millis() - t1 >1000){
    t1 = millis();
    bool state = digitalRead(16);
    digitalWrite(16,!state);
  }*/
   if(millis() - t2 > 3000){
    t2 = millis();
    Serial.println("현재 상황");
    Serial.println(btn_cnt);
    Serial.println("현재 모션");
    Serial.println(motion_cnt);
  }
  /*
  digitalWrite(16,HIGH);
  delay(1000);
  digitalWrite(16,LOW);
  delay(1000);
  */
}
