#include <Arduino.h>
#include "PCF8574.h"
#include "HCSR04.h"
#include "ShiftRegister74HC595.h"
#include "Adafruit_NeoPixel.h"

#include "WebServer.h"
#include "ArduinoJson.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define sensorPositive 39
#define cupSensor 36
#define enablePumps 23
#define hoseLight 17
#define auxPin2 16
#define ambientalLight 26
#define indicationLight 25

#define shieldLed 2

//handles para usar los dos procesadores
TaskHandle_t secondCore;
TaskHandle_t firstCore;

//Credenciales wifi
const char *SSID = "Fiush machine";
const char *PWD = "Fiush123456";
// Web server running on port 80
WebServer server(80);
// JSON buffer de datos
StaticJsonDocument<1000> jsonDocument;
char buffer[1000];

HCSR04 ultrasonicSensor(32, 33);                    // trigg, echo

ShiftRegister74HC595<4> auxiliarOutputs(19, 5, 18); // Data, SHCP, STCP

Adafruit_NeoPixel ambientStrip = Adafruit_NeoPixel(88, ambientalLight, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel indicationStrip = Adafruit_NeoPixel(80, indicationLight, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel hoseStrip = Adafruit_NeoPixel(16, hoseLight, NEO_GRB + NEO_KHZ800);

Adafruit_SSD1306 display(128, 32, &Wire, -1);
int  x, minX;
int scroll_speed1 = 2;
int scroll_speed2 = 3;

PCF8574 PCF1_input(0x20);  // read encoder 0-7
PCF8574 PCF1_output(0x24); // set pump direction 0-7
PCF8574 PCF2_input(0x26);  // read encoder 8-15
PCF8574 PCF2_output(0x27); // set pump direction 8-15
PCF8574 PCF3_input(0x23);  // read encoder 16-23
PCF8574 PCF3_output(0x21); // set pump direction 16-23

bool last_read_encoder[24] = {true};
bool now_read_encoder[24] = {true};
int encoder_counter[24] = {0};

String coctel_name = "FIUSH 1.3";
int pump1 = 99;
float ml1 = 99;
int priority1 = 0;
bool rotation1 = 0;


int pump2 = 99;
float ml2 = 99;
int priority2 = 0;
bool rotation2 = 0;


int pump3 = 99;
float ml3 = 99;
int priority3 = 0;
bool rotation3 = 0;

bool cherry = 0;
bool lemon = 0;
bool orange = 0;
bool herb = 0;

//bandera de estado de la máquina 1=Maquina ocupada 0=Máquina disponible
volatile bool state_machine = 0;

void FirstCore(void *pvParameters);
void SecondCore(void *pvParameters);

void setupRouting();  //rutas de comunicación del JSON
void handlePost();    //recepción de recetas desde la app
void connectToWiFi();
void getState();      //Responde via JSON a la app disponibilidad de la máquina

uint32_t WheelAmbiental(byte WheelPos);
void rainbowAmbiental(uint8_t wait);
uint32_t WheelHose(byte WheelPos);
void rainbowHose(uint8_t wait);

void TestShiftOutput();
void TestDistanceSensors();
void TestUltrasonicSensor();
void TestIndicationStrip();
void TestAmbientStrip();

bool cupSensorOffToOn();
bool cupSensorOnToOff();
void iconsLight();
void StartCoctel();
void UpdateEncoders();
void InitialEncodersRead();
void AllPCFLow();
void AllPCFHigh();
void TurnOffOnePCFOutput(int PCF_output);
void ResetEncoders();
void ScrollText(int speed);

void PriorityOrder( int pump1, int ml1, int priority1, bool rotation1, int pump2, int ml2, int priority2,bool rotation2,
                    int pump3, int ml3, int priority3, bool rotation3);



void setup()
{
  Serial.begin(115200);
  Serial.println("hoi");
  pinMode(sensorPositive, INPUT);
  pinMode(cupSensor, INPUT);
  pinMode(enablePumps, OUTPUT);
  pinMode(hoseLight, OUTPUT);
  pinMode(auxPin2, OUTPUT);
  pinMode(shieldLed, OUTPUT);
  pinMode(ambientalLight, OUTPUT);
  pinMode(indicationLight, OUTPUT);

  WiFi.softAP(SSID, PWD);
  IPAddress IP = WiFi.softAPIP(); //Creación de red local wifi
  Serial.print("AP IP address: ");
  Serial.println(IP);
  server.begin(); //inicialización del servidor
  setupRouting(); //inicialización de las rutas JSON

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  delay(100);
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setTextWrap(false);
  x    = display.width();

  indicationStrip.setBrightness(30);
  ambientStrip.setBrightness(30);
  hoseStrip.setBrightness(30);

  PCF1_input.begin();
  PCF1_output.begin();
  PCF2_input.begin();
  PCF2_output.begin();
  PCF3_input.begin();
  PCF3_output.begin();

  AllPCFHigh();
  AllPCFLow();

  xTaskCreatePinnedToCore(SecondCore, "secondCore", 10000, NULL, 1, &secondCore, 0); //inicialización procesador 2
  delay(500);
  xTaskCreatePinnedToCore(FirstCore, "firstCore", 10000, NULL, 1, &firstCore, 1); //inicialización procesador 1

  digitalWrite(shieldLed, HIGH);
  delay(1000);
  digitalWrite(shieldLed, LOW);
}

void FirstCore(void *pvParameters)
{
  //Si llega una receta nueva comienza la preparación
  for (;;)
  {
    ScrollText(scroll_speed1);
    if (state_machine == 1)
    {
      StartCoctel();
      state_machine = 0; //deja la maquina disponible para otra receta
    }

  }
  vTaskDelay(10);
}

void SecondCore(void *pvParameters)
{
  for (;;)
  {
    server.handleClient(); //Recepcion de información desde la APP
    rainbowAmbiental(250);
    rainbowHose(50);

  }
  vTaskDelay(10);
}

void loop()
{
  delay(10);
}

void TestShiftOutput()
{
  digitalWrite(enablePumps, HIGH);
  // delay(1000);

  for (int i = 0; i < 24; i++)
  {
    auxiliarOutputs.set(i, HIGH);
    delay(50);
  }

  delay(1000);

  for (int i = 0; i < 24; i++)
  {
    auxiliarOutputs.set(i, LOW);
    // delay(100);
  }
  delay(1000);
}

void TestDistanceSensors()
{
  if (digitalRead(sensorPositive) == true || digitalRead(cupSensor) == true)
  {
    digitalWrite(shieldLed, HIGH);
    delay(500);
    digitalWrite(shieldLed, LOW);
    delay(500);
  }
}

void TestUltrasonicSensor()
{
  while (ultrasonicSensor.dist() >= 5 && ultrasonicSensor.dist() < 50)
  {
    digitalWrite(shieldLed, HIGH);
  }
  digitalWrite(shieldLed, LOW);
}

void TestIndicationStrip()
{

  for (int i = 0; i < indicationStrip.numPixels(); i++)
  {
    indicationStrip.setPixelColor(i, 255, 255, 255);
  }
  indicationStrip.show();

  delay(500);

  for (int i = 0; i < indicationStrip.numPixels(); i++)
  {
    indicationStrip.setPixelColor(i, 0, 0, 0);
  }
  indicationStrip.show();

  delay(500);
}

void TestAmbientStrip()
{

  for (int i = 0; i < ambientStrip.numPixels(); i++)
  {
    ambientStrip.setPixelColor(i, 255, 255, 255);
  }
  ambientStrip.show();

  delay(500);

  for (int i = 0; i < ambientStrip.numPixels(); i++)
  {
    ambientStrip.setPixelColor(i, 0, 0, 0);
  }
  ambientStrip.show();

  delay(500);
}

void TestEncodersAndPumps()
{
  delay(100);
}

void AllPCFLow()
{
  for (int i = 0; i < 8; i++)
  {
    PCF1_output.write(i, LOW);
  }
  for (int i = 0; i < 8; i++)
  {
    PCF2_output.write(i, LOW);
  }
  for (int i = 0; i < 8; i++)
  {
    PCF3_output.write(i, LOW);
  }    
}

void AllPCFHigh()
{
  for (int i = 0; i < 8; i++)
  {
    PCF1_output.write(i, HIGH);
  }
    for (int i = 0; i < 8; i++)
  {
    PCF2_output.write(i, HIGH);
  }
    for (int i = 0; i < 8; i++)
  {
    PCF3_output.write(i, HIGH);
  }
}

void UpdateEncoders()
{

  for (int i = 0; i < 24; i++)
  {
    // auxiliarOutputs.set(23, HIGH);
    // digitalWrite(shieldLed, HIGH);
    // delay(10);
    // auxiliarOutputs.set(23, LOW);
    // digitalWrite(shieldLed, LOW);
    // delay(10);


    if (i < 8)
    {
      // auxiliarOutputs.set(22, HIGH);
      // delay(10);
      // auxiliarOutputs.set(22, LOW);
      // delay(10);

      now_read_encoder[i] = PCF1_input.read(i);
      if (now_read_encoder[i] != last_read_encoder[i])
      {
        encoder_counter[i]++;
        last_read_encoder[i] = now_read_encoder[i];
      }
    }

    if (i >= 8 && i < 16)
    {
      // auxiliarOutputs.set(21, HIGH);
      // delay(10);
      // auxiliarOutputs.set(21, LOW);
      // delay(10);

      now_read_encoder[i] = PCF2_input.read(i-8);
      if (now_read_encoder[i] != last_read_encoder[i])
      {
        // auxiliarOutputs.set(15, HIGH);
        // delay(100);
        // auxiliarOutputs.set(15, LOW);
        // delay(100);
        encoder_counter[i]++;
        last_read_encoder[i] = now_read_encoder[i];
      }
    }

    if (i >= 16 && i < 24)
    {
      // auxiliarOutputs.set(20, HIGH);
      // delay(10);
      // auxiliarOutputs.set(20, LOW);
      // delay(10);  

      now_read_encoder[i] = PCF3_input.read(i-16);
      if (now_read_encoder[i] != last_read_encoder[i])
      {
        encoder_counter[i]++;
        last_read_encoder[i] = now_read_encoder[i];
      }
    }    
  } 
}

void InitialEncodersRead()
{
  for (int i = 0; i < sizeof(now_read_encoder); i++)
  {
    if (i < 8)
    {
      now_read_encoder[i] = PCF1_input.read(i);
      last_read_encoder[i] = now_read_encoder[i];
    }
    else if (i >= 8 && i < 16)
    {
      now_read_encoder[i] = PCF2_input.read(i-8);
      last_read_encoder[i] = now_read_encoder[i];
    }
    else if (i >= 16 && i < 24)
    {
      now_read_encoder[i] = PCF3_input.read(i-16);
      last_read_encoder[i] = now_read_encoder[i];
    }
  }
}

void TurnOffOnePCFOutput(int PCF_output){
  // for (int i = 0; i < sizeof(now_read_encoder); i++)
  // {
    if (PCF_output < 8)
    {
      PCF1_output.write(PCF_output, LOW);
      return;
    }
    else if (PCF_output >= 8 && PCF_output < 16)
    {
      PCF2_output.write(PCF_output-8, LOW);
      return;
    }
    else if (PCF_output >= 16 && PCF_output < 24)
    {
      PCF1_output.write(PCF_output-16, LOW);
      return;
    }
  // }
}

void ResetEncoders(){
  for(int i=0 ; i<24 ; i++){
    encoder_counter[i] = 0;
  }
}

void PriorityOrder( int pump1, int ml1, int priority1, bool rotation1, int pump2, int ml2, int priority2,bool rotation2,
                    int pump3, int ml3, int priority3, bool rotation3)
{
  static int pump_number = 3;

  const int real_pumps[pump_number] = {pump1, pump2, pump3};

  const int pumps[pump_number] = {pump1, pump2, pump3};

  bool pump_direction[pump_number] = {rotation1, rotation2, rotation3};

  int minilliters[pump_number] = {ml1, ml2, ml3};

  bool process_finished = false;

  bool pump_finished[pump_number] = {false, false, false};

  int actual_priority = 0;

  int pumps_priority[pump_number] = {priority1, priority2, priority3};

  int priority_counter[pump_number] = {0, 0, 0};

  int pump_finished_counter = 0;

  int same_priority[pump_number] = {0, 0, 0};

  for(int i = 0; i < pump_number; i++) {

  }

  digitalWrite(enablePumps, HIGH);
  AllPCFLow();  
  ResetEncoders();
  InitialEncodersRead();

  for(int i = 0; i < pump_number; i++){
    if(real_pumps[i] == 98){
      pump_finished[i] = true;
    }
  }

  for (int i = 0; i < pump_number; i++)
  {
    for (int j = 0; j < pump_number; j++)
    {
      if (pumps_priority[j] == i)
      {
        same_priority[i]++;
      }
    }
  }
  for (int i = 0; i < pump_number; i++)
  {
    if (same_priority[i] == 0)
    {
      same_priority[i] = 99;
    }
    Serial.println(same_priority[i]);
  }


  for(int i = 0 ; i < pump_number; i++){
    if(pump_direction[i] == 1){
      if(real_pumps[i] < 8){
        PCF1_output.write(i , HIGH);
      }
      if(real_pumps[i] >= 8 && real_pumps[i] < 16){
        PCF2_output.write(real_pumps[i]-8, HIGH);
      }
      if(real_pumps[i] >=16 && real_pumps[i] < 24){
        PCF3_output.write(real_pumps[i]-16, HIGH);
      }
    }
  }

  Serial.println("bombas");
  for (int i = 0; i < pump_number; i++)
  {
    Serial.println(pumps[i]);
  }
  Serial.println("ml");
  for (int i = 0; i < pump_number; i++)
  {
    Serial.println(minilliters[i]);
  }
  Serial.println("prioridad");
  for (int i = 0; i < pump_number; i++)
  {
    Serial.println(pumps_priority[i]);
  }
  Serial.println("/////////////");
  // InitialStates();

  InitialEncodersRead();

  while (process_finished == false)
  {

    for (int i = 0; i < pump_number; i++)
    {
      if (pump_finished[i] == true)
      {
        process_finished = true;
      }
      else
      {
        process_finished = false;
        i = pump_number;
      }
    }

    for (int i = actual_priority; i < pump_number; i++)
    {
      if (same_priority[i] == priority_counter[i])
      {
        Serial.println("iguales");
        actual_priority++;
      }
    }

    for (int i = 0; i < pump_number; i++)
    {
      if (actual_priority == pumps_priority[i])
      {

        if (pump_finished[i] == false)
        {
          // digitalWrite(real_pumps[i], LOW);
          // digitalWrite(real_pumps[pumps[i]], LOW);//mosfet
          // digitalWrite(real_pumps[pumps[i]], HIGH);//RELAY
          // digitalWrite(pumps[i], HIGH);
          auxiliarOutputs.set(real_pumps[i], HIGH);
          Serial.println("bomba encendida " + String(real_pumps[i]) + " i = " + String(i));
        }
      }
    }

    UpdateEncoders();
    ScrollText(scroll_speed2);
    

    for (int i = 0; i < 8; i++)
    {
      Serial.println(encoder_counter[i]);
    }
    // ingrediente 1
    if (encoder_counter[pumps[0]] >= minilliters[0])
    {
      if (pump_finished[0] == false)
      {
        // digitalWrite(real_pumps[pumps[0]], HIGH);//mosfet
        // digitalWrite(real_pumps[pumps[0]], LOW);//RELAY
        auxiliarOutputs.set(real_pumps[0], LOW);
        TurnOffOnePCFOutput(pumps[0]);
        pump_finished_counter++;
        for (int i = 0; i < pump_number; i++)
        {
          if (pumps_priority[0] == i)
          {
            priority_counter[i]++;
          }
        }
        pump_finished[0] = true;
      }
    }
    // ingrediente 2
    if (encoder_counter[pumps[1]] >= minilliters[1])
    {
      if (pump_finished[1] == false)
      {
        // digitalWrite(real_pumps[pumps[1]], HIGH);
        // digitalWrite(real_pumps[pumps[1]], LOW);
        auxiliarOutputs.set(real_pumps[1], LOW);
        TurnOffOnePCFOutput(pumps[1]);
        pump_finished_counter++;
        for (int i = 0; i < pump_number; i++)
        {
          if (pumps_priority[1] == i)
          {
            priority_counter[i]++;
          }
        }
        pump_finished[1] = true;
      }
    }
    // ingrediente 3
    if (encoder_counter[pumps[2]] >= minilliters[2])
    {
      if (pump_finished[2] == false)
      {
        // digitalWrite(real_pumps[pumps[2]], HIGH);
        // digitalWrite(real_pumps[pumps[2]], LOW);
        auxiliarOutputs.set(real_pumps[2], LOW);
        TurnOffOnePCFOutput(pumps[2]);
        pump_finished_counter++;
        for (int i = 0; i < pump_number; i++)
        {
          if (pumps_priority[2] == i)
          {
            priority_counter[i]++;
          }
        }
        pump_finished[2] = true;
      }
    }
  }
  digitalWrite(enablePumps, LOW);
  AllPCFLow();
}

void StartCoctel(){
  cupSensorOffToOn(); //espera a detectar el vaso

  PriorityOrder(pump1, ml1, priority1, rotation1,
                pump2, ml2, priority2, rotation2,
                pump3, ml3, priority3, rotation3);

  cupSensorOnToOff(); //Espera a que se retire el vaso mientras parpadea 
  coctel_name = "FIUSH 1.3";
}

void setupRouting()
{
  server.on("/state", HTTP_POST, getState); //dirección para conocer si la máquina está preparando un coctel o está disponible
  server.on("/preparation", HTTP_POST, handlePost); //dirección de rcepción de recetas
  // start server
  server.begin();
}

void createJson(char *tag, float value) //crea el JSON para responder el estado de la máquina ( disponible/no disponible)
{
  jsonDocument.clear();
  jsonDocument["state"] = value;
  serializeJson(jsonDocument, buffer); //Serializa los datos
}

void addJsonObject(char *tag, float value)  //agrega el objeto al JSON
{
  JsonObject obj = jsonDocument.createNestedObject();
  obj["state"] = value;
}

void getState() //Responde el estado d ela máquina
{
  jsonDocument.clear();
  createJson("state", state_machine);
  server.send(200, "application/json", buffer);
}

void handlePost() //Recive la receta
{
  if (server.hasArg("plain") == false)
  {
    //handle error here
  }
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);
  JsonObject obj = jsonDocument.as<JsonObject>();
  server.send(200, "application/json", "{Receta recivida}");//coctel listo  //Responde al recibir la receta

//Actualización de variables
  String name_input = obj["name"];
  coctel_name = name_input;

  pump1 = jsonDocument["pump1"];
  pump1 = pump1 - 1;  //Evita tener la bomba 0
  ml1 = jsonDocument["ml1"];
  priority1 = jsonDocument["priority1"];
  priority1= priority1 - 1; 
  rotation1 = jsonDocument["rotation1"];

  pump2 = jsonDocument["pump2"];
  pump2 = pump2 - 1;
  ml2 = jsonDocument["ml2"];
  priority2 = jsonDocument["priority2"];
  priority2= priority2 - 1;
  rotation2 = jsonDocument["rotation2"];

  pump3 = jsonDocument["pump3"];
  pump3 = pump3 - 1;
  ml3 = jsonDocument["ml3"];
  priority3 = jsonDocument["priority3"];
  priority3= priority3 - 1;
  rotation3 = jsonDocument["rotation3"];
  
  Serial.println("name1: " + coctel_name);
  Serial.println("pump1: " + String(pump1));
  Serial.println("ml1: " + String(ml1));
  Serial.println("priority1: " + String(priority1));
  Serial.println("rotation1: " + String(rotation1));
  // pump4 = jsonDocument["pump4"];
  // pump4 = pump4 - 1;
  // ml4 = jsonDocument["ml4"];
  // k4 = jsonDocument["k4"];

  // pump5 = jsonDocument["pump5"];
  // pump5 = pump5 - 1;
  // ml5 = jsonDocument["ml5"];
  // k5 = jsonDocument["k5"];

  // pump6 = jsonDocument["pump6"];
  // pump6 = pump6 - 1;
  // ml6 = jsonDocument["ml6"];
  // k6 = jsonDocument["k6"];

  // pump7 = jsonDocument["pump7"];
  // pump7 = pump7 - 1;
  // ml7 = jsonDocument["ml7"];
  // k7 = jsonDocument["k7"];

  cherry = jsonDocument["GC"];
  lemon = jsonDocument["GL"];
  orange = jsonDocument["GO"];
  herb = jsonDocument["GH"];

  state_machine = 1; //Avisa al otro procesador que puede iniciar la preparación
  delay(100);
  Serial.println("state en 1");

  //Serial.println(String(pump1) + "," + String(ml1) + "," + String(k1) + String(pump2) + "," + String(ml2) + "," + String(k2) + String(pump3) + "," + String(ml3) + "," + String(k3) + String(pump4) + "," + String(ml4) + "," + String(k4) + String(pump5) + "," + String(ml5) + "," + String(k5) + String(pump6) + "," + String(ml6) + "," + String(k6) + String(pump7) + "," + String(ml7) + "," + String(k7));
}

void ScrollText(int speed){
  minX = -20 * coctel_name.length();
  display.clearDisplay();
  display.setCursor(x, 11);
  display.print(coctel_name);
  display.display();
  x=x-speed;

  if(x < minX) x = display.width();
}


uint32_t WheelAmbiental(byte WheelPos)
{
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85)
  {
    return ambientStrip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170)
  {
    WheelPos -= 85;
    return ambientStrip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return ambientStrip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void rainbowAmbiental(uint8_t wait)
{
  static unsigned long previousTimeToChange = 0;
  static int i, j = 0;
  // ambientStrip.setBrightness(100);

  if ((millis() - previousTimeToChange) >= wait)
  {
    j++;
    for (i = 0; i < ambientStrip.numPixels(); i++)
    {
      ambientStrip.setPixelColor(i, WheelAmbiental((i + j) & 255));
    }

    ambientStrip.show();
    if (j >= 255)
    {
      j = 0;
    }
    previousTimeToChange = millis();
  }
}

uint32_t WheelHose(byte WheelPos)
{
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85)
  {
    return hoseStrip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170)
  {
    WheelPos -= 85;
    return hoseStrip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return hoseStrip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void rainbowHose(uint8_t wait)
{
  static unsigned long previousTimeToChange = 0;
  static int i, j = 0;
  // ambientStrip.setBrightness(100);

  if ((millis() - previousTimeToChange) >= wait)
  {
    j++;
    for (i = 0; i < hoseStrip.numPixels(); i++)
    {
      hoseStrip.setPixelColor(i, WheelHose((i + j) & 255));
    }

    hoseStrip.show();
    if (j >= 255)
    {
      j = 0;
    }
    previousTimeToChange = millis();
  }
}





bool cupSensorOffToOn()
{
  bool cupDetection = true;
  cupDetection = digitalRead(cupSensor);

  while (cupDetection == false)
  {
    ScrollText(scroll_speed1);
    // alertCupLight(0.8, 'G');  //Parpadeo en azul del cabezal hasta detectar el vaso
    cupDetection = digitalRead(cupSensor);
    if (cupDetection == true)
    {
      delay(100);
      cupDetection = digitalRead(cupSensor);
      if (cupDetection == true)   //Confirmación de vaso
      {
        Serial.println("Si hay vaso");
        return cupDetection;
      }
    }
    // delay(10);
  }
  // iconsLight(); //Deja de parpadear
  return cupDetection;
}

bool cupSensorOnToOff()
{
  bool cupDetection = false;
  cupDetection = digitalRead(cupSensor);

  while (cupDetection == true)
  {
    ScrollText(scroll_speed1);
    // alertCupLight(2.5, 'R'); //parpadeo lento indicando que termino la receta pero no han retirado el vaso
    cupDetection = digitalRead(cupSensor);
    if (cupDetection == false)
    {
      delay(100);
      cupDetection = digitalRead(cupSensor);
      if (cupDetection == false)  //confirmacion de vaso retirado
      {
        Serial.println("No hay vaso");
        return cupDetection;
      }
    }
    // delay(10);
  }
  // iconsLight();
  return cupDetection;
}

void turnOnDropsLight()
{
  for (int i = 96; i < 104; i++)
  {
    indicationStrip.setPixelColor(i, 255, 255, 255);
  }
  iconsLight();
}

void iconsLight()
{
  for (int i = 33; i < 96; i++)
  {
    indicationStrip.setPixelColor(i, 255, 255, 255);
  }

  indicationStrip.show();
}

void alertCupLight(float pulseTime, char color)
{
  //Hace parpadear la luz del cabezal en azul(G) o rojo(R) a una frecuencia determinada 
  int delayTime = (pulseTime * 1000) / 10;
  static unsigned long previousTimeToChange = millis();
  static bool changeFlag = 0;

  if ((millis() - previousTimeToChange) >= delayTime)
  {
    if (changeFlag == false)
    {
      for (int i = 96; i < 104; i++)
      {
        indicationStrip.setPixelColor(i, 0, 0, 0);
      }
      iconsLight();
      changeFlag = true;
    }

    else if (changeFlag == true)
    {
      for (int i = 96; i < 104; i++)
      {
        switch (color){
          case 'G':
            indicationStrip.setPixelColor(i, 0, 0, 255);
          break;
          case 'R':
            indicationStrip.setPixelColor(i, 255, 0, 0);
          break;
          default:
            indicationStrip.setPixelColor(i, 255, 255, 255);
          break;
        }

      }
      iconsLight();
      changeFlag = false;
    }
    previousTimeToChange = millis();
  }
}