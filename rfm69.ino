#include <SPI.h>
#include <RH_RF69.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>


RH_RF69 rf69(8, 7); // Adafruit Feather 32u4
Adafruit_MPU6050 mpu;
void setup() 
{
  Serial.begin(9600);
  while (!Serial) 
    ;
  if (!rf69.init())
    Serial.println("init failed");
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module)
  // No encryption
  if (!rf69.setFrequency(433.0))//통신 주파수 지정 433mh
    Serial.println("setFrequency failed");

  // If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
  // ishighpowermodule flag set like this:
  rf69.setTxPower(20, true); // 보낼 쌔기

  // The encryption key has to be the same as the one in the server
  uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};//데이터 키값
  rf69.setEncryptionKey(key);
  
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
      while (1) {
      delay(10);
     }
   }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}
uint8_t my_id = 6;
uint8_t myframe_counter = 0;
uint8_t frame_counter[16];
uint8_t repeat_counter[16];

uint8_t data[128] = {NULL, NULL, NULL, 'm','i','n',':',};
uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
uint8_t len = sizeof(buf);
union
{
  unsigned long l;
  byte c [4];
  float f;
} temperature;

void loop()
{
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  temperature.f = a.acceleration.x;
  //Serial.println(temperature.l,HEX);
  //Serial.println("");
  if(Serial.available())
  {  //시리얼통신켜기 1개이상일때
    int len_input = 0;
    data[0] = my_id;
    myframe_counter++;
    myframe_counter = myframe_counter % 128;
    data[1] = myframe_counter;
    data[2] = 0;
    while(Serial.available())
    {
        data[7 + len_input] = Serial.read();
        len_input++;
    }
    data[7+ len_input] = NULL;
    len_input++;
    rf69.send(data, len_input+7);
  }
  //Serial.println("Sending to rf69_server");
  // Send a message to rf69_server
  
  
  
  rf69.waitPacketSent();
  // Now wait for a reply


  if (rf69.waitAvailableTimeout(500))
  { 
    // Should be a reply message for us now   
    if (rf69.recv(buf, &len))
    {
      buf[len] = NULL;
      int tmp_id = buf[0] -1;
      Serial.print("got reply: ");
      Serial.print(tmp_id + 1);
      Serial.print(" ");
   
      if(frame_counter[tmp_id] != buf[1]){
        //처음들어왔다는 뜻
        Serial.println((char*)&buf[3]);
      }
      else
      {
        Serial.print(tmp_id +1);Serial.println("is already recv!");
      }
      uint8_t tmp_repeatcounter = buf[2];
      if(tmp_repeatcounter <10)
      {
        tmp_repeatcounter++;
        buf[2] = tmp_repeatcounter;
        rf69.send(buf, len + 1);
      }
      
    }
    else
    {
      Serial.println("recv failed");
    }
  }
  else
  {
    //Serial.println("No reply, is rf69_server running?");
  }
  delay(400);
}
