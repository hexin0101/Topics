//函式庫 你懂的
#include <Arduino.h>//載入函式庫 vscode用
#include <WiFi.h>//連接wifi
#include <HTTPClient.h>//line notify
#include <ESP32Servo.h>
#include <FastLED.h>//WS2812B使用
#include <HUSKYLENS.h>
#include <SoftwareSerial.h>
#define NUM_LEDS 8//宣告led光帶長度
#define leds_y 3//宣告有幾條
#define DATA_PIN1 5 //led腳位
#define DATA_PIN2 13//led腳位
#define DATA_PIN3 14//led腳位
HTTPClient http;
HUSKYLENS huskylens;
Servo myservo1;
Servo myservo2;
Servo myservo3;  
CRGB leds[leds_y][NUM_LEDS];//陣列 條 第幾顆
String Linetoken = "Bearer M1HViaxFufDRiDAdNNKDeB3UPqsJjfhzdP632D0QvX9";//tonken
//各種變數
const char* ssid = "Redmi125G";//wifi名稱
const char* password = "2147483647";//密碼
char trigPin_1=17; //發出聲波腳位(ESP32 GPIO17 19 22)
char echoPin_1=16; //接收聲波腳位(ESP32 GPIO16 18 21)
char trigPin_2=19; //發出聲波腳位(ESP32 GPIO17 19 22)
char echoPin_2=18; //接收聲波腳位(ESP32 GPIO16 18 21)
char trigPin_3=4; 
char echoPin_3=15;
char isfullornot;//判斷有沒有滿
char whichisfulll;//哪一個滿了
char PETnumber[10];
char retortnumber[10];
char cansnumber[10];
int lasttybe_temp;
int typecountla[]={0,0,0};
int onesec;
int onemin;
int aa;
int zuoyou[2];
int pos = 0;   
int whitchtybe=0;
int lasttybe=2;
int servoPin1 = 33;
int servoPin2 = 32;
int servoPin3 = 27;
unsigned long time_temp=0;
int moving;
void printResult(HUSKYLENSResult result);
unsigned long d;//選擇使用哪一個超音波
String wwwwwww="message=寶特瓶";
String typetwo="message=鋁箔包";
String typethree="message=鐵鋁罐";
String typefour="個";
//初始化哈士奇
void servoway();
void printResult(HUSKYLENSResult result){
    if (result.command == COMMAND_RETURN_BLOCK){
      Serial.println(String()+F(",ID=")+result.ID);
    }
    else if (result.command == COMMAND_RETURN_ARROW){
      Serial.println(String()+F(",ID=")+result.ID);
    }
    else{
      Serial.println("Object unknown!");
    }
}
//偵測物品
void zhencewupin(){
  if (!huskylens.request()) Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
    else if(!huskylens.isLearned()) Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
    else if(!huskylens.available()) Serial.println(F("No block or arrow appears on the screen!"));
    else{
      Serial.println(F("###########"));
      while (huskylens.available()){
        HUSKYLENSResult result = huskylens.read();
        printResult(result);
        
        
        whitchtybe=result.ID;

      }    
    }
}
//設定網路
void setup_wifi() {//設定wifi
  WiFi.begin(ssid, password);//開始 連接指定wifi並輸入密碼
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(1000);//等待一秒
    Serial.println("Connecting to WiFi..");//顯示連線中
  }
  Serial.println("WiFi connected");
}
//通知用
void tongzhishouji(int nazhong){
  nazhong=nazhong+1;
  switch (nazhong){
    case 1:http.POST("message=寶特瓶滿了");http.POST("message=寶特瓶滿了");break;
    case 2:http.POST("message=鋁箔包滿了");http.POST("message=鋁箔包滿了");break;
    case 3:http.POST("message=鐵鋁罐滿了");http.POST("message=鐵鋁罐滿了");break;
    default:delay(50);break;
  }
}
//超音波 用收到時間計算
unsigned long ping(int temp_1,int temp_2) { 
  digitalWrite(temp_1, HIGH);
  delayMicroseconds(10);
  digitalWrite(temp_1, LOW);
  return pulseIn(temp_2, HIGH);
}
//閃燈 包擴偵測容量
void aleddblank(int temp_type){
  for(char clearledlong=0;clearledlong<=7;clearledlong++){
    leds[temp_type][clearledlong] = CRGB::Black;
}
  d=ping(trigPin_1,echoPin_1)/58;
  int awa;
  switch (temp_type)
  {
  case 0:
    awa=22;
    break;
  case 1:
    awa=22;
    break;
  case 2:
    awa=22;
    break;
  default:
    break;
  }
  isfullornot=(d-awa)/4;
  
  int tempa=8-isfullornot;
  Serial.println("tempa");
  
  Serial.println(tempa);
  for(int ledlong=0;ledlong<=tempa;ledlong++){
    if(tempa<=3)leds[temp_type][ledlong] = CRGB::Red;
    if(tempa<=6 && tempa>3)leds[temp_type][ledlong] = CRGB::Blue;
    if(tempa>=7)leds[temp_type][ledlong] = CRGB::Green;
    
  }
  if(tempa>=8||tempa<=0){
      Serial.println("whitchtybe");Serial.println(whitchtybe);tongzhishouji(temp_type);whitchtybe=2;servoway();
      for(char clearledlong=0;clearledlong<=7;clearledlong++){
            leds[temp_type][7] = CRGB::Green;
            FastLED.show();
        }
      }
  Serial.print(d);
  Serial.println("cm");    
}
//伺服馬達移動
void moveservo(int temp_fangxiang,int temp_shang_zhong_xia,int timelong){//1開2關
switch(temp_fangxiang){ 
  case 1:
    switch (temp_shang_zhong_xia)
    {
    case 1:
      myservo1.write(180);delay(timelong);myservo1.write(90);
      break;
    case 2:
      myservo2.write(180);delay(timelong);myservo2.write(90);
      break;
    case 3:
      myservo3.write(180);delay(timelong);myservo3.write(90);
      break;
    default:
      break;
    }
  break;
  case 2:
  switch (temp_shang_zhong_xia)
    {
    case 1:
      myservo1.write(0);delay(timelong);myservo1.write(90);
      break;
    case 2:
      myservo2.write(0);delay(timelong);myservo2.write(90);
      break;
    case 3:
      myservo3.write(0);delay(timelong);myservo3.write(90);
      break;
    default:
      break;
    }
  break;
  default:myservo1.write(90);break;
}
}
//選擇左右
void servoway(){
  Serial.println(whitchtybe);
  Serial.println(lasttybe);
  if (lasttybe < whitchtybe)
  {
    for (int i=lasttybe; i < whitchtybe; i++)
    {
      moveservo(1,2,2000);
      Serial.println(i);
      Serial.println(lasttybe);
    }
    lasttybe=whitchtybe;
    Serial.println(whitchtybe);
      Serial.println(lasttybe);
  }else if (lasttybe > whitchtybe){
    for (int i=lasttybe; i > whitchtybe; i--)
    {
      moveservo(2,2,2300);
      Serial.println(i);
      Serial.println(lasttybe);
    }
    lasttybe=whitchtybe;
    Serial.println(whitchtybe);
    Serial.println(lasttybe);
  }else{
    myservo3.write(90);
    Serial.println(whitchtybe);
    Serial.println(lasttybe);
  } 
  
}
//偵測路人
void peoplemove(){
  do
  {
    
    d=ping(trigPin_3,echoPin_3)/58;
    if (d<50)
    {
      delay(2000);
      d=ping(trigPin_3,echoPin_3)/58;
      Serial.println("no");
    }
    
  } while (d>50);

  Serial.println("pass");
}
//是否投入

//初始化容量
void resetall(){
  whitchtybe=1;
  servoway();
  lasttybe_temp=lasttybe-1;
  aleddblank(lasttybe_temp);
  FastLED.show();
  whitchtybe=3;
  servoway();
  lasttybe_temp=lasttybe-1;
  aleddblank(lasttybe_temp);
  FastLED.show();
  whitchtybe=2;
  servoway();
  lasttybe_temp=lasttybe-1;
  aleddblank(lasttybe_temp);
  FastLED.show();
}
void typecount(int temp){
  switch (temp)
  {
  case 1:
    typecountla[0]+=1;
    break;
  case 2:
    typecountla[1]+=1;
    break;
  case 3:
    typecountla[2]+=1;
    break;
  default:
    break;
  }
}
void setup() {
  
  pinMode(trigPin_1, OUTPUT);
  pinMode(trigPin_2, OUTPUT);
  pinMode(trigPin_3, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	myservo1.setPeriodHertz(50);
  myservo2.setPeriodHertz(50);
  myservo3.setPeriodHertz(50);    
	myservo1.attach(servoPin1, 1000, 2000);
  myservo2.attach(servoPin2, 1000, 2000);
  myservo3.attach(servoPin3, 1000, 2000);
  FastLED.addLeds<WS2812B, DATA_PIN1, RGB>(leds[0], NUM_LEDS);
  FastLED.addLeds<WS2812B, DATA_PIN2, RGB>(leds[1], NUM_LEDS);
  FastLED.addLeds<WS2812B, DATA_PIN3, RGB>(leds[2], NUM_LEDS);
  Wire.begin();
  while (!huskylens.begin(Wire)){
    Serial.println(F("Begin failed!"));
    Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>I2C)"));
    Serial.println(F("2.Please recheck the connection."));
    delay(100);
  }
  //resetall();
}
void loop() {
  http.begin("https://notify-api.line.me/api/notify");
  http.addHeader("Content-Type","application/x-www-form-urlencoded");
  http.addHeader("Authorization",Linetoken);
  
  peoplemove();
  Serial.println("1st");
  moveservo(1,1,1530);
  do
  {
    zhencewupin();
  } while (whitchtybe==4);
  delay(5000);
  moveservo(2,1,1440);
  do
  {
    zhencewupin();
  } while (whitchtybe==4);
  Serial.println("whitchtybe");
  Serial.println(whitchtybe);
  typecount(whitchtybe);
  servoway();
  moveservo(1,3,2000);
  delay(1000);
  moveservo(2,3,2300);
  myservo3.write(50);
  delay(100);
  myservo3.write(90);
  lasttybe_temp=lasttybe-1;
  aleddblank(lasttybe_temp);
  FastLED.show();
  delay(500);//每次間格0.5秒
  time_temp=millis()-time_temp;
  onesec=time_temp/1000;
  itoa(typecountla[0],PETnumber,10);
  itoa(typecountla[1],retortnumber,10);
  itoa(typecountla[2],cansnumber,10);

  wwwwwww=wwwwwww+PETnumber+typefour;
  Serial.println(wwwwwww);
  typetwo=typetwo+retortnumber+typefour;
  typethree=typethree+cansnumber+typefour;
  delay(1000);
  http.POST("message=今日數量");
  http.POST(wwwwwww);
  http.POST(typetwo);
  http.POST(typethree);
  
  
  /*if (onesec>=3600){
    itoa(typecountla[0],PETnumber,10);
    itoa(typecountla[1],retortnumber,10);
    itoa(typecountla[2],cansnumber,10);
    http.POST(wwwwwww+PETnumber+typefour);
    http.POST(typetwo+retortnumber+typefour);
    http.POST(typethree+cansnumber+typefour);
    time_temp=millis();
  }*/
  wwwwwww="message=寶特瓶";
  typetwo="message=鋁箔包";
  typethree="message=鐵鋁罐";
  
  
}
