#include <M5Core2.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
//screen 320x240
char *timer_type;
char *timer_status="START";

//タイマー
unsigned long timer_value=0;
unsigned long timer_viewer_origin=0;
unsigned int timer_viewer_s=0;
unsigned int timer_viewer_m=0;
unsigned int timer_viewer_h=0;
unsigned int display_refresh_old=0;
int i=0,n=0;
unsigned long start_time=0;

// グローバル変数
//無線LANのSSID
extern char *ssid ;
//無線LANのパスワード
extern char *password ;

//スプレッドシートのURL
extern const char* published_url ;
//タイマー

void timer(int number){
  
}

void setup_serial(){
  Serial.begin(115200);
  while (!Serial) continue;
}

void setup_wifi(){
  
  Serial.println("Connecting to ");
  Serial.print(ssid);

  // WiFi接続性改善のため、いったん切断
  WiFi.disconnect();
  delay(500);

  // WiFi開始
  WiFi.begin(ssid, password);
 
  // Wi-Fi接続待ち
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    M5.Lcd.print(".");
  }

  // WiFi接続成功メッセージの表示
  Serial.println("\nWiFi Connected.");
  M5.Lcd.setCursor(10, 40);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(2);
  M5.Lcd.println("WiFi Connected.");

  // M5StackのIPアドレスを表示
  M5.Lcd.print("IP address: ");
  M5.Lcd.println(WiFi.localIP());
  


}

unsigned long cal_arduino_timer(unsigned long time){
  unsigned long t;
  unsigned long changed_time;
  t=millis();//今の経過時間ms
  changed_time=t-time;//最初の経過時間
  return changed_time;
}

void setup(){

  // M5Stack objectの初期化
    M5.begin(true, true, true, true);
// M5.Power.begin();//電池残量出すときはこれ
  setup_serial();

  // 画面初期設定

  M5.Lcd.fillScreen(TFT_NAVY);
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(3);
  M5.Lcd.printf("START");
  M5.Lcd.setBrightness(1);

  // Wi-Fi処理の開始
  setup_wifi();
  M5.Lcd.fillScreen(TFT_BLACK);
  timer_type ="kara";
  timer_status="START";
    Serial.print("準備完了\n");
}

void loop(){


  TouchPoint_t pos = M5.Touch.getPressPoint();
  if(pos.y > 240&&pos.x >160 ){
    //左ボタンでスリープ
    M5.Lcd.sleep(); 
    M5.Lcd.setBrightness(0);  
  }else if(pos.y > 240&&pos.x <160){
    //中央ボタンでスリープ復帰
    M5.Lcd.wakeup(); 
    M5.Lcd.setBrightness(1);
  }else{
    // M5.Lcd.wakeup();

     
  if(timer_type=="kara"){
      /*
      ジャンル選択画面
      */
      M5.Lcd.drawString("StudyEnglish", 20, 0, 2);//x,y,size
      M5.Lcd.drawString("U-labProject", 20, 50, 2);//x,y,size
      M5.Lcd.drawString("Programming", 20, 100, 2);//x,y,size
      M5.Lcd.drawString("Exercise", 20, 150, 2);//x,y,size
      M5.Lcd.drawString("ContentConsumption", 20, 200, 2);//x,y,size

      M5.Lcd.drawLine(0, 50, 2000,50,BLUE);  
      M5.Lcd.drawLine(0, 100, 2000,100,BLUE);  
      M5.Lcd.drawLine(0, 150, 2000,150,BLUE);  
      M5.Lcd.drawLine(0, 200, 2000,200,BLUE);  

      // M5.Lcd.sleep(); 
      if(pos.y <= 50&&pos.y >= 1){
        timer_type="StudyEnglish";
        M5.Lcd.fillScreen(TFT_BLACK);
        timer_status="STOP";
        delay(100);
        start_time=millis();
        
      }else if(pos.y > 50&&pos.y <= 100){
        timer_type="U-labProject";
        M5.Lcd.fillScreen(TFT_BLACK);
        timer_status="STOP";
        delay(100);
        start_time=millis();
        
      }else if(pos.y > 100&&pos.y <= 150){
        timer_type="Programming";
        M5.Lcd.fillScreen(TFT_BLACK);
        timer_status="STOP";
        delay(100);
        start_time=millis();
        
      }else if(pos.y > 150&&pos.y <= 200){
        timer_type="Exercise";
        M5.Lcd.fillScreen(TFT_BLACK);
        timer_status="STOP";
        delay(100);
        start_time=millis();
        
      }else if(pos.y > 200&&pos.y <= 240){
        timer_type="ContentConsumption";
        M5.Lcd.fillScreen(TFT_BLACK);
        timer_status="STOP";
        delay(100);
        start_time=millis();
        
      }
      delay(100);
    }else{
      if(timer_status!="END"){
        /*
        ストップウォッチ画面
        */



        //ジャンル表示
        M5.Lcd.setTextSize(2);
        M5.Lcd.drawString(timer_type, 40, 200, 2);
        //スタート・ストップ
        // Serial.print(timer_viewer_origin);
        // Serial.print("\n");
        if(timer_status=="START"){
          //タイマー停止中の処理
          timer_viewer_origin=round((timer_value)/1000);  
          M5.Lcd.fillTriangle(70,135,70,105,95,120, YELLOW);
        }else if(timer_status=="STOP"){
          //タイマー作動中
          timer_viewer_origin=round((millis()-start_time+timer_value)/1000);
         
          M5.Lcd.fillRect(70,105, 10, 30, BLUE); 
          M5.Lcd.fillRect(90,105, 10, 30, BLUE); 
        }
          M5.Lcd.fillRect(210, 105, 30, 30, BLUE); 
          M5.Lcd.drawCircle(80, 120, 50, RED);
          M5.Lcd.drawCircle(220, 120, 50, RED);


        //時間表示
        //秒を時間分秒に変換
        timer_viewer_h= floor(timer_viewer_origin/3600);
        timer_viewer_origin%=3600;

        timer_viewer_m= floor(timer_viewer_origin/60);
        timer_viewer_origin%=60;
      
        timer_viewer_s= timer_viewer_origin;

        //画面更新は秒数が変化した時のみする←ちらつき防止
      if(timer_viewer_s-display_refresh_old>=1){
        M5.Lcd.fillRect(40, 20, 300, 40, TFT_BLACK); 
        display_refresh_old=timer_viewer_s;
      }
        M5.Lcd.setCursor(40,20);
        M5.Lcd.setTextSize(4);
        M5.Lcd.printf("%3d:%2d:%2d",timer_viewer_h,timer_viewer_m,timer_viewer_s);//x,y,size



        //タッチ判定
        if(pos.x <= 160&&pos.y <= 230&&pos.x >= 0&&pos.y >= 0){
          Serial.print("startpushbutton:pushed\n");
          if(timer_status=="START"){
          timer_status="STOP";
          start_time=millis();
          Serial.print("view:STOP\n");
          delay(100);
          }else if(timer_status=="STOP"){
          timer_value+=cal_arduino_timer(start_time);
          timer_status="START";
          Serial.print("view:START\n");
          delay(100);
          }
          M5.Lcd.fillCircle(80, 120, 48, TFT_BLACK);
        }else if(pos.x > 160&&pos.y <= 230&&pos.y >= 0){
          timer_status="END";
          Serial.print("view:END");

        }
      
      }else{
        /*
        スプレッドシートへ送信処理
        */
        //タイマー加算  
        timer_value+=cal_arduino_timer(start_time);

        //ここからスプレッドシート送信準備
        StaticJsonDocument<500> doc;
        char pubMessage[256];
        M5.Lcd.setCursor(0, 0, 1);
        M5.Lcd.fillScreen(BLACK);
        M5.Lcd.setTextColor(WHITE);
        M5.Lcd.println("Sending...");
        
        // JSONメッセージの作成
        JsonArray idValues = doc.createNestedArray("ID");
        idValues.add("12345");

        JsonArray dataValues1 = doc.createNestedArray("time");
        Serial.print(timer_viewer_origin);
        Serial.print("\n");
        dataValues1.add(round(timer_value/(1000*60)));//ミリ秒→分にして送信する
        JsonArray dataValues2 = doc.createNestedArray("type");
        dataValues2.add(timer_type);

        serializeJson(doc, pubMessage);

        // HTTP通信開始
        HTTPClient http;

        Serial.print(" 通信開始　\n");
        http.begin(published_url);
      
        Serial.print(" POSTメソッド送る　\n");
        int httpCode = http.POST(pubMessage);
      
        if(httpCode > 0){
          M5.Lcd.setTextSize(4);
          M5.Lcd.printf("Response:%d\n", httpCode);
      
          if(httpCode == HTTP_CODE_OK){
            //GAS側で事故っても200帰ってくるので注意。serialの中身にエラー載ってる。
            M5.Lcd.println(" OK!");
            String payload = http.getString();
            Serial.println(payload);
          }
        }else{
          M5.Lcd.fillScreen(TFT_NAVY);
          M5.Lcd.println(" FAILED");
          Serial.printf("　HTTP　failed,error: %s\n", http.errorToString(httpCode).c_str());
        }
      
        http.end();
        delay(1000);

        timer_type ="kara";
        timer_status="START";
        timer_value=0;
        timer_viewer_origin=0;
        M5.Lcd.setTextSize(2);
        Serial.print("リセット\n");
        M5.Lcd.fillScreen(TFT_BLACK);
      }
    }
  }
}