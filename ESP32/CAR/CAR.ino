#include <WiFi.h>
#include "ESP32_Servo.h"

//WIFI
const char* W_SSID = ":'v':";
const char* W_PASSWORD = "thedragonsarekawaii";

//pins
const byte BRAKE_LIGHT_PIN = 23;
const byte RIGHT_TURN_LIGHT_PIN = 22;
const byte LEFT_TURN_LIGHT_PIN = 21;
const byte MOTOR_ENABLE_PIN = 13;
const byte LEFT_MOTOR_INVERT_PIN = 12;
const byte RIGHT_MOTOR_INVERT_PIN = 14;
const byte SERVO_PIN = 4;

//blink routines
const byte BLINK_ROUTINE_STOPPED = 0;
const byte BLINK_ROUTINE_TURNING_LEFT = 1;
const byte BLINK_ROUTINE_TURNING_RIGHT = 2;
const byte BLINK_ROUTINE_REVERSE = 3;
const byte BLINK_ROUTINE_WIFI_DISCONNECTED = 4;
const byte BLINK_ROUTINE_NOTIFY = 5;

//Frontal
const bool FRONTAL_INVERT = LOW;

//DELAYS
const word BLINK_INTERVAL = 500;
const word WIFI_INTERVAL = 500;
const word SERVO_UPDATE_INTERVAL = 30;
const word MAX_SYNC_DELAY = 30000;

//STATUS
const byte DIRECTION_FORWARD = 0;
const byte DIRECTION_BACKWARDS = 1;
const byte DIRECTION_LEFT = 2;
const byte DIRECTION_RIGHT = 3;

//SERVO
Servo steering;

//SERVO STEPPTING
const word SERVO_STEP_VALUE = 5;

//Max steer speed factor
const float MAX_STEER_FACTOR = 2;
const word TIME_COUNT_FACTOR = 500;

//last dir recv only forward or backward
String last_recv = "";

//last data recv
String l_data = "";

//Status
byte actual_blink_routine;
int connAttempts = 0;

//lu stands for last update
unsigned long lu_brake_lights = 0, lu_right_turn_light = 0, lu_left_turn_light = 0, lu_wifi_connect_attempts, lu_servo = 0, lu_sync;

//st stands for state
boolean st_brake_lights = LOW, st_left_turn_light = LOW, st_right_turn_light = LOW, st_wifi_ok;

//st servo
word servo_angle = 90;
float steer_factor = 1;

//direction
byte dir = 0;

//steerng start time
unsigned long steer_start;

word PORT = 5500;
word CLIENT_PORT = PORT+5;
word SYNC_PORT = CLIENT_PORT+1;

//if wifi disconnected already sended stop
//boolean stop_sended;

WiFiServer server(PORT);

void setup()
{
  pinMode(BRAKE_LIGHT_PIN, OUTPUT);
  pinMode(LEFT_TURN_LIGHT_PIN, OUTPUT);
  pinMode(RIGHT_TURN_LIGHT_PIN, OUTPUT);
  pinMode(MOTOR_ENABLE_PIN, OUTPUT);
  pinMode(LEFT_MOTOR_INVERT_PIN, OUTPUT);
  pinMode(RIGHT_MOTOR_INVERT_PIN, OUTPUT);
  Serial.begin(115200);
  Esp_wifi_set_ps (WIFI_PS_NONE);
  WiFi.begin(W_SSID, W_PASSWORD);
  Serial.print("Connecting to wifi");
  actual_blink_routine = BLINK_ROUTINE_WIFI_DISCONNECTED;
  while(WiFi.status() != WL_CONNECTED && connAttempts < 50)
  {
    Serial.print(".");
    if(millis() - lu_wifi_connect_attempts > WIFI_INTERVAL)
    {
      connAttempts++;
      delay(500); 
    }
    DoBlinkRoutine();
  }
  Serial.println("");
  if(connAttempts < 50)
  {
    steering.attach(SERVO_PIN);
    steering.write(servo_angle);
    Serial.println("Connection successful!!");
    Serial.print("IP ADDRESS: ");
    Serial.println(WiFi.localIP());
    WiFi.setHostname("CEREBRO_DE_MUGRE");
    st_wifi_ok = HIGH;
    actual_blink_routine = BLINK_ROUTINE_NOTIFY;
    DoBlinkRoutine();
    actual_blink_routine = BLINK_ROUTINE_STOPPED;
    ResetLights();
    UpdateLights();
  }
  else
  {
    Serial.println("COULDN'T CONNECT TO WIFI!!");
    st_wifi_ok = LOW;
    SendData("stop");
  }
  server.begin();
}



WiFiClient client;
WiFiClient c2;



void loop()
{
  if(!st_wifi_ok)
  {
    DoBlinkRoutine();
  }
  else
  {
    client = server.available();
    String d = "";
    IPAddress client_ip;
    if(client)
    {
      Serial.println("Client connected");
      while(client.available())
      {
        char c = client.read();
        Serial.print("Received: ");
        Serial.println(c);
        d += c;
      }
      client_ip = client.remoteIP();
      Serial.println("");
      Serial.print("HOST IP: ");
      Serial.println(client_ip);
      client.stop();
      if(c2.connect(client_ip, CLIENT_PORT))
      {
        c2.println(d);
        c2.stop();
      }

      if(d == "nock nock")
      {
        actual_blink_routine = BLINK_ROUTINE_NOTIFY;
        DoBlinkRoutine();
        actual_blink_routine = BLINK_ROUTINE_STOPPED;
        ResetLights();
        UpdateLights();
      }
      SendData(d);
      if(!(d == "what ya doin"))
      {
        l_data = d;
      }
      else
      {
        lu_sync = millis();
        if(c2.connect(client_ip, SYNC_PORT))
        {
          c2.println(l_data);
          c2.stop();
        }
      }
      d = "";
    }

    
    if(millis() - lu_sync > MAX_SYNC_DELAY)
    {
      actual_blink_routine = BLINK_ROUTINE_WIFI_DISCONNECTED;
      if(!(l_data == "stop"))
      {
        l_data = "stop";
        SendData(l_data);
      }
    }
    
    DoBlinkRoutine();
    UpdateSteer();
  }
}

void SendData(String r_data)
{
  //stopped
  if(r_data == "stop"){
    digitalWrite(MOTOR_ENABLE_PIN, LOW);
    digitalWrite(LEFT_MOTOR_INVERT_PIN, LOW);
    digitalWrite(RIGHT_MOTOR_INVERT_PIN, LOW);
    servo_angle = 0;
    steering.write(servo_angle);
    actual_blink_routine = BLINK_ROUTINE_STOPPED;
    ResetLights();
    dir = 100;
  }
  
  //going forward
  if(r_data == "w"){
    digitalWrite(MOTOR_ENABLE_PIN, HIGH);
    digitalWrite(LEFT_MOTOR_INVERT_PIN, FRONTAL_INVERT);
    digitalWrite(RIGHT_MOTOR_INVERT_PIN, FRONTAL_INVERT);
    actual_blink_routine = -1;
    ResetLights();
    dir = DIRECTION_FORWARD;
  }
  
  //turining to the left
  if(r_data == "wa"){
    TurnLeft();
    last_recv = "w";
  }

  if(r_data == "sa"){
    TurnLeft();
    last_recv = "s";
  }
  
  //turning to the right
  if(r_data == "wd"){
    TurnRight();
    last_recv = "w";
  }

  if(r_data == "sd"){
    TurnRight();
    last_recv = "s";
  }
  
  //going backwards
  if(r_data == "s"){
    digitalWrite(MOTOR_ENABLE_PIN, HIGH);
    digitalWrite(LEFT_MOTOR_INVERT_PIN, !FRONTAL_INVERT);
    digitalWrite(RIGHT_MOTOR_INVERT_PIN, !FRONTAL_INVERT);
    actual_blink_routine = BLINK_ROUTINE_REVERSE;
    ResetLights();
    dir = DIRECTION_BACKWARDS;
  }

  //if steering change to last direction
  if(r_data == "w" || r_data == "s")
  {
    last_recv = r_data;
  }
  if(r_data == "wa" || r_data == "wd" || r_data == "sa" || r_data == "sd")
  {
    if(last_recv == "s")
    {
      digitalWrite(LEFT_MOTOR_INVERT_PIN, !FRONTAL_INVERT);
      digitalWrite(RIGHT_MOTOR_INVERT_PIN, !FRONTAL_INVERT);
      st_brake_lights = HIGH;
    }
    else
    {
      digitalWrite(LEFT_MOTOR_INVERT_PIN, FRONTAL_INVERT);
      digitalWrite(RIGHT_MOTOR_INVERT_PIN, FRONTAL_INVERT);
      st_brake_lights = LOW;
    }
  }
}

void TurnLeft()
{
    digitalWrite(MOTOR_ENABLE_PIN, HIGH);
    actual_blink_routine = BLINK_ROUTINE_TURNING_LEFT;
    ResetLights();
    if(dir != DIRECTION_LEFT)
    {
      CenterSteer();
      steer_factor = 1;
      steer_start = millis();
    }
    dir = DIRECTION_LEFT;
}

void TurnRight()
{
  digitalWrite(MOTOR_ENABLE_PIN, HIGH);
    actual_blink_routine = BLINK_ROUTINE_TURNING_RIGHT;
    ResetLights();
    if(dir != DIRECTION_RIGHT)
    {
      CenterSteer();
      steer_factor = 1;
      steer_start = millis();
    }
    dir = DIRECTION_RIGHT;
}

void CenterSteer()
{
  servo_angle = 90;
}

void UpdateSteer()
{
  if(millis() - lu_servo > SERVO_UPDATE_INTERVAL)
  {
    switch(dir)
    {
      case DIRECTION_FORWARD:
      {
        servo_angle = 90;
        steer_factor = 1;
        break;
      }
      case DIRECTION_BACKWARDS:
      {
        servo_angle = 90;
        steer_factor = 1;
        break;
      }
      case DIRECTION_RIGHT:
      {
        if(servo_angle < 170)
        {
          servo_angle += SERVO_STEP_VALUE * steer_factor;
          if(servo_angle > 170)
            servo_angle = 170;
        }
        break;
      }
      case DIRECTION_LEFT:
      {
        if(servo_angle > 0)
        {
          servo_angle -= SERVO_STEP_VALUE * steer_factor;
          if(servo_angle < 0)
            servo_angle = 0;
        }
        break;
      }
      default:
      {
        servo_angle = 90;
        steer_factor = 1;
        break;
      }
    }
    lu_servo = millis();
  }
  steering.write(servo_angle);
}

void UpdateSteerFactor()
{
  unsigned long time_steering = millis() - steer_start;
  word time_ = time_steering / TIME_COUNT_FACTOR;
  steer_factor += (time_ / 100);
  if(steer_factor > MAX_STEER_FACTOR)
    steer_factor = MAX_STEER_FACTOR;
}

void DoBlinkRoutine()
{

}

void ResetLights()
{

}

void UpdateLights()
{

}
