#include <clocks.h>

#define MW         16
#define PIN_alarm_TONE         16
#define MH         16
#define BRIGHTNESS 20

Adafruit_NeoMatrix *matrix = new Adafruit_NeoMatrix(MW, MH, 12,
  NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG, NEO_GRB + NEO_KHZ800);

int hours = 12;
int minutes = 0;
int seconds = 0;
int last_button_min = 0;
int button_t_f = 0;
int button_flag_t_f = 0;
int alarm_minutes = -1;
int alarm_hours = -1;
int last_button_hours = 0;
int buttonMin = 0;
int buttonHour = 0;
bool Con = false;
uint32_t Time = 0;
int period = 1000;
const char *ssid     = "iPhone pukich";
const char *password = "88888888";

const long utcOffsetInSeconds = 10800;


// Определение NTP-клиента для получения времени
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

uint16_t RGB_bmp[][256] = {
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
};

  void fixdrawRGBBitmap(int16_t x, int16_t y, uint16_t* bitmap, int16_t w, int16_t h){
  uint16_t RGB_bmp_fixed[w * h];
  for (uint16_t pixel = 0; pixel < w * h; pixel++){
    uint8_t r, g, b;
    uint16_t color = pgm_read_word(bitmap + pixel);
    b = (color & 0xF00) >> 8;
    g = (color & 0x0F0) >> 4;
    r = color & 0x00F;
    b = map(b, 0, 15, 0, 31);
    g = map(g, 0, 15, 0, 63);
    r = map(r, 0, 15, 0, 31);
    RGB_bmp_fixed[pixel] = (r << 11) + (g << 5) + b;
  }
   matrix->drawRGBBitmap(x, y, RGB_bmp_fixed, w, h);
  }

void display_rgbBitmap(int hours, int minutes, int alarm, bool wifi) 
  {
    uint16_t bmx, bmy;
    connect(RGB_bmp[0], hours, minutes, alarm, wifi);
    fixdrawRGBBitmap(bmx, bmy, RGB_bmp[0], 16, 16);
    bmx += 16;
    if (bmx >= MW) { bmx = 0; }
    if (!bmx) { bmy += 16; }
    if (bmy >= MH) { bmy = 0; }
    matrix->show();
  }

  void getTime(){
   if (WiFi.status() == WL_CONNECTED){
    Con = true;
    timeClient.update();
    hours = timeClient.getHours();
    minutes = timeClient.getMinutes();
    seconds = timeClient.getSeconds();
    }
  }

  void wifi_connect(){
    WiFi.begin(ssid, password);
    timeClient.begin();
    Con = false;
  }

  boolean button_true_false(const int i){
    button_t_f = digitalRead(i);
  if (button_t_f != button_flag_t_f){
     if (button_t_f == HIGH){ 
    button_flag_t_f = button_t_f;
    return true;
    } 
  }
  button_flag_t_f = button_t_f;
    return false;
 
}

 void button_of_hours(int& a){
  buttonHour = digitalRead(5);
  if (buttonHour != last_button_hours){
     if (buttonHour == HIGH){
    a += 1;
    } 
  }
  last_button_hours = buttonHour;
}

 void button_of_min(int& a){
  buttonMin = digitalRead(2);
  if (buttonMin != last_button_min){
     if (buttonMin == HIGH){
    a += 1;
    } 
  }
  last_button_min = buttonMin;
}

 void alarm_on (){
  if (button_true_false(0)){
   alarm_minutes = minutes;
    alarm_hours = hours;
      while (!button_true_false(0)){
      button_of_min(alarm_minutes);
      button_of_hours(alarm_hours);
      display_rgbBitmap(alarm_hours, alarm_minutes, alarm_hours, Con);
    }
  }
}

 void alarm_voice(){     
  if (hours == alarm_hours && minutes == alarm_minutes && seconds == 0){
  tone(PIN_alarm_TONE, 2000); delay(1000); noTone(PIN_alarm_TONE); delay(1000);
  tone(PIN_alarm_TONE, 2000); delay(1000); noTone(PIN_alarm_TONE); delay(1000);
  tone(PIN_alarm_TONE, 2000); delay(1000); noTone(PIN_alarm_TONE);
    alarm_hours = -1;
    alarm_minutes = -1;
    }
}

 void wifi_button(){
if (button_true_false(4)){
    if (Con == true)
      getTime();
    else {
    wifi_connect(); 
    }
  }
}

 void button(){
  button_of_hours(hours);
  button_of_min(minutes); 
  wifi_button();
  alarm_on();
  }

void setup()
{
  pinMode(2, INPUT);
  pinMode(5, INPUT);
  pinMode(PIN_alarm_TONE, OUTPUT);
  pinMode(4, INPUT);
  pinMode(0, INPUT);
  Serial.begin(9600);
  matrix->begin();
  matrix->setBrightness(BRIGHTNESS);
  matrix->show();
  wifi_connect();
}

void chek (){
  if (hours >= 24)
        hours = 0;
  if (minutes >= 60)
        minutes = 0;
  if (seconds >= 60)
        seconds = 0;
  }

void loop() 
{  
  
  display_rgbBitmap(hours, minutes, alarm_hours, Con);
  if (Con == false)
  getTime();
  if (WiFi.status() != WL_CONNECTED)
  Con = false;
  alarm_voice();
  button();
  chek();
if (millis() - Time >= period){
    Time += period;
    seconds+= 1;
    if (seconds == 60) {
    minutes++;
    seconds = 0;
      if(minutes == 60){
      hours++;
      minutes = 0;
        if (hours == 24)
        hours = 0;  
            }
    }
  }
}
  
