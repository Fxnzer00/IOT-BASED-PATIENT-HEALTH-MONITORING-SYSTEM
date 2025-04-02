#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MLX90614.h>  
#include <MAX30100_PulseOximeter.h>
#include <Adafruit_I2CDevice.h>
#define REPORTING_PERIOD_MS 1000
#include "MAX30100.h" //library sensor
#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels


LiquidCrystal_I2C lcd(0x27,16,2); //library lcd
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

MAX30100* pulseOxymeter;   //library

const char* ssid = "ipan";
const char* password = "hahahaha";
//const char* server = "10.12.149.209";

int ledpin =18;
int switchdum;

int spo;
float bpm;
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
WebServer server(80);

float TargetC;
int suhunya;

int bpmphp, oxiphp, oxi1, bpm1;
//char bpmlevel[5];
String bpmlevel, oxilevel, templevel, bodycond;



PulseOximeter pox;

uint32_t tsLastReport = 0;
String hrData = "";
void onBeatDetected()
{
//    Serial.println("Beat!");
}

void setup() {
  pinMode(ledpin, INPUT);
  Wire.begin();
  mlx.begin();
  Serial.begin(115200);
  WiFi.mode(WIFI_STA); //Optional
  WiFi.begin(ssid, password);

   while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
  
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");  
  
   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  lcd.init();
  lcd.backlight();
  lcd.clear();

   //Serial.print("Initializing pulse oximeter..");
 
    if (!pox.begin()) {
        //Serial.println("FAILED");
        for(;;);
    } else {
        //Serial.println("SUCCESS");
    }
 
   pox.setIRLedCurrent(MAX30100_LED_CURR_11MA);
   pox.setOnBeatDetectedCallback(onBeatDetected);
 
}
   
void loop(){

   server.handleClient();
   pox.update();
    
   if (millis() - tsLastReport > REPORTING_PERIOD_MS) {

       bpm = pox.getHeartRate();
       spo = pox.getSpO2();
       
       lcd.setCursor(0,1);
       lcd.print("BpmOxi:");
       lcd.print(pox.getHeartRate());
       lcd.print("/");
       lcd.print(pox.getSpO2());
       lcd.print("     ");

tsLastReport = millis();
    
}

mlx.begin();
TargetC = mlx.readObjectTempC();
//TargetC = mlx.readAmbientTempC();
suhunya = TargetC;

oxi1=pox.getSpO2();
oxiphp=oxi1;

bpm1 = pox.getHeartRate();
bpmphp=bpm1;

switchdum = digitalRead(ledpin);


lcd.setCursor(0,0);
lcd.print("Temp: ");
lcd.print(TargetC);
lcd.print(" C");
  

       Serial.println("BPM");
       Serial.println(pox.getHeartRate());
       Serial.println("OXI");
       Serial.println(pox.getSpO2());

  display.clearDisplay();
  display.setCursor(40, 0);
  display.print("BPM Level :");
  display.setCursor(40, 20);
  display.print("OXI Level :");
  display.setCursor(20, 40);
  display.print("Body Temp Level :");

if(bpm1 ==0){

  display.setCursor(20, 10);
  display.print("No finger Detect");
  //display.print(level);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  bpmlevel = "No Finger Detect";

}

if(bpm1 >=51 && bpm1 <=120){

  display.setCursor(50, 10);
  display.print("Normal");
  //display.print(level);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  //bpmlevel[0] = 'Normal';
  bpmlevel = "Normal";

}

if(bpm1 >=1 && bpm1 <=50){

  display.setCursor(50, 10);
  display.print("Bradycardia");
  //display.print(level);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  //bpmlevel[0] = 'Normal';
  bpmlevel = "Bradycardia";

}
if(bpm1 >=121 && bpm1 <=129){

  display.setCursor(50, 10);
  display.print("Elevated");
  //display.print(level);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  //bpmlevel[1] = 'Elevated';
  bpmlevel = "Elevated";

}

if(bpm1 >=130 && bpm1 <=139){

  display.setCursor(30, 10);
  display.print("Hypertension 1");
  //display.print(level);
  display.setTextSize(1);
  display.setTextColor(WHITE);
 // bpmlevel[2] = 'Hypertension1';
 bpmlevel = "Hypertension 1";

}

if(bpm1 >=140 && bpm1 <=179){

  display.setCursor(30, 10);
  display.print("Hypertension 2");
  //display.print(level);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  //bpmlevel[3] = 'Hypertension 2';
  bpmlevel = "Hypertensiion 2";

}

if(bpm1 >=180){

  display.setCursor(40, 10);
  display.print("Emergency");
  //display.print(level);
  display.setTextSize(1);
  display.setTextColor(WHITE);
 // bpmlevel[4] = 'Emergency';
 bpmlevel = "Emergency";

}


if(oxi1 ==0){

  display.setCursor(20, 30);
  display.print("No finger Detect");
  //display.print(level);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  oxilevel = "No finger Detect";

}

if(oxi1 >=96){

  display.setCursor(50, 30);
  display.print("Normal");
  //display.print(level);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  oxilevel = "Normal";

}

if(oxi1 ==95){

  display.setCursor(50, 30);
  display.print("Acceptable");
  //display.print(level);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  oxilevel = "Acceptable";

}

if(oxi1 >=93 && oxi1 <=94){

  display.setCursor(20, 30);
  display.print("Need Advice");
  //display.print(level);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  oxilevel = "Need Advice";

}

if(oxi1 >=1 && oxi1 <=92){

  display.setCursor(10, 30);
  display.print("Need urgent medical");
  //display.print(level);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  oxilevel = "Need Urgent Medical";

}

if(suhunya >=38 && suhunya <=40){

  display.setCursor(30, 50);
  display.print("Common Fever");
  //display.print(level);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  templevel = "Common Fever";

}

if(suhunya >=35 && suhunya <=37){

  display.setCursor(30, 50);
  display.print("Normal");
  //display.print(level);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  templevel = "Normal";

}

if(suhunya >=30 && suhunya <=34){

  display.setCursor(30, 50);
  display.print("Hypothermia");
  //display.print(level);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  templevel = "Hypothermia";

}

if(bpm1 >=40 && bpm1 <=100 && oxi1>=96 && TargetC >=35 && TargetC <=37.5){

  bodycond= "Normal";
}

if(bpm1 >=101 && bpm1 <=109 && oxi1==95 && TargetC == 38){

  bodycond= "Acceptable";
}

if(bpm1 >=101 && bpm1 <=109 && oxi1>=93 && oxi1<=94 && TargetC >=38.1 && TargetC<=39){

  bodycond= "Need Advice";
}

if(bpm1 >=101 && bpm1 <=109 && oxi1<=92 && TargetC >=38.1 && TargetC<=40){

  bodycond= "Urgent Medical";
}

if(bpm1 ==0 && oxi1 ==0){
  bodycond=" ";
}

  display.display(); 

delay(1);

}

void handle_OnConnect() {
  server.send(200, "text/html", SendHTML(bpmphp, oxiphp, TargetC, bpmlevel, oxilevel, templevel, bodycond));
}
void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML(int bpm, int oxiphp, float TargetC, String bpmlevel, String oxilevel, String templevel, String bodycond) {
  String ptr = "<!DOCTYPE html>";
  ptr += "<html>";
  ptr += "<head>";
  ptr += "<title>IOT Based Patient Health Monitoring System</title>";
  ptr += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  ptr += "<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.7.2/css/all.min.css'>";
  ptr += "<link rel='stylesheet' type='text/css' href='styles.css'>";
  ptr += "<style>";
  ptr += "body { background-color: #fff; font-family: sans-serif; color: #333333; font: 14px Helvetica, sans-serif box-sizing: border-box;}";
  ptr += "#page { margin: 20px; background-color: #fff;}";
  ptr += ".container { height: inherit; padding-bottom: 20px;}";
  ptr += ".header { padding: 20px;}";
  ptr += ".header h1 { padding-bottom: 0.3em; color: #008080; font-size: 45px; font-weight: bold; font-family: Garmond, 'sans-serif'; text-align: center;}";
  ptr += "h2 { padding-bottom: 0.2em; border-bottom: 1px solid #eee; margin: 2px; text-align: left;}";
  ptr += ".header h3 { font-weight: bold; font-family: Arial, 'sans-serif'; font-size: 17px; color: #b6b6b6; text-align: center;}";
  ptr += ".box-full { padding: 20px; border 1px solid #ddd; border-radius: 1em 1em 1em 1em; box-shadow: 1px 7px 7px 1px rgba(0,0,0,0.4); background: #fff; margin: 20px; width: 300px;}";
  ptr += "@media (max-width: 494px) { #page { width: inherit; margin: 5px auto; } #content { padding: 1px;} .box-full { margin: 8px 8px 12px 8px; padding: 10px; width: inherit;; float: none; } }";
  ptr += "@media (min-width: 494px) and (max-width: 980px) { #page { width: 465px; margin 0 auto; } .box-full { width: 380px; } }";
  ptr += "@media (min-width: 980px) { #page { width: 930px; margin: auto; } }";
  ptr += ".sensor { margin: 12px 0px; font-size: 2.5rem;}";
  ptr += ".sensor-labels { font-size: 1rem; vertical-align: middle; padding-bottom: 15px;}";
  ptr += ".units { font-size: 1.2rem;}";
  ptr += "hr { height: 1px; color: #eee; background-color: #eee; border: none;}";
  ptr += "</style>";
  //Ajax Code Start
  ptr += "<script>\n";
  ptr += "setInterval(loadDoc,1000);\n";
  ptr += "function loadDoc() {\n";
  ptr += "var xhttp = new XMLHttpRequest();\n";
  ptr += "xhttp.onreadystatechange = function() {\n";
  ptr += "if (this.readyState == 4 && this.status == 200) {\n";
  ptr += "document.body.innerHTML =this.responseText}\n";
  ptr += "};\n";
  ptr += "xhttp.open(\"GET\", \"/\", true);\n";
  ptr += "xhttp.send();\n";
  ptr += "}\n";
  ptr += "</script>\n";
  //Ajax Code END
  ptr += "</head>";
  ptr += "<body>";
  ptr += "<center><img alt='Unimap' src='https://ftken.unimap.edu.my/templates/header_menu12/images/logofktenandunimap.png' width='200' height='130'></center>";
  ptr += "<div id='page'>";
  ptr += "<div class='header'>";
  ptr += "<h1>IOT Based Patient Health Monitoring System</h1>";
  ptr += "</div>";
  ptr += "<div id='content' align='center'>";
  ptr += "<div class='box-full' align='left'>";
  ptr += "<h2>Sensor Readings</h2>";
  ptr += "<div class='sensors-container'>";
  //For Heart Rate
  ptr += "<p class='sensor'>";
  ptr += "<i class='fas fa-heartbeat' style='color:#cc3300'></i>";
  ptr += "<span class='sensor-labels'> Heart Rate </span>";
  ptr += (int)bpmphp;
  ptr += "<sup class='units'>BPM</sup>";
  ptr += "</p>";
  ptr += "<hr>";
  //For Sp02
  ptr += "<p class='sensor'>";
  ptr += "<i class='fas fa-burn' style='color:59DCF7'></i>";
  ptr += "<span class='sensor-labels'> Sp02 </span>";
  ptr += (int)oxiphp;
  ptr += "<sup class='units'>%</sup>";
  ptr += "</p>";
  ptr += "<hr>";

  ptr += "<p class='sensor'>";
  ptr += "<i class='fas fa-temperature-high' style='color:#f7347a'></i>";
  ptr += "<span class='sensor-labels'> Body Temperature </span>";
  ptr += (float)TargetC;
  ptr += "<sup class='units'>°C</sup>";
  ptr += "</p>";

  

  ptr += "</div>";
  ptr += "</div>";
  ptr += "</div>";
  ptr += "</div>";
  ptr += "</div>";

  ptr += "<div id='page'>";
  ptr += "<div id='content' align='center'>";
  ptr += "<div class='box-full' align='left'>";
  ptr += "<h2>Level Sensor</h2>";
  ptr += "<div class='sensors-container'>";
  //For Heart Rate
  ptr += "<p class='sensor'>";
  ptr += "<span class='sensor-labels'> BPM Level :  </span>";
  ptr += "<h2>";
  ptr +=  (String)bpmlevel;
  ptr += "</h2>";
  ptr += "</p>";
  ptr += "<hr>";
  //For Sp02
  ptr += "<p class='sensor'>";
  ptr += "<span class='sensor-labels'> Sp02 Level : </span>";
  ptr += "<h2>";
  ptr +=  (String)oxilevel;
  ptr += "</h2>";
  ptr += "</p>";
  ptr += "<hr>";
  //For Body Temperature
  ptr += "<p class='sensor'>";
  ptr += "<span class='sensor-labels'> Body Temperature Level : </span>";
  ptr += "<h2>";
  ptr +=  (String)templevel;
  ptr += "</h2>";
  ptr += "</p>";
  ptr += "<hr>";
  //For overall body condition  
  ptr += "<p class='sensor'>";
  ptr += "<span class='sensor-labels'> Overall Body Condition : </span>";
  ptr += "<h2>";
  ptr +=  (String)bodycond;
  ptr += "</h2>";
  ptr += "</p>";
  
  ptr += "</div>";
  ptr += "</div>";
  ptr += "</div>";
  ptr += "</div>";
  ptr += "</div>";
  ptr += "<style>";
  ptr += "footer { background-color: #B8C7CB; padding: 10px; text-align: center; color: white; }";
  ptr += "</style>";
  ptr += "<footer>";
  ptr += "<p>Embedded System Project IOT based Patient Health Monitoring System</p>";
  ptr += "</footer>";
  ptr += "</body>";
  ptr += "</html>";
  return ptr;
}
