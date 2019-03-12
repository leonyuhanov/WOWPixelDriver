#include <ESP8266WiFi.h>

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial.printf("\r\n\r\nThis Modules ESP Chip ID:\t");
  Serial.print(ESP.getChipId());
  Serial.printf("\r\n\tThis Modules WIFI_STA MAC ADdress:\t");
  Serial.println(WiFi.macAddress());
  pinMode(2, OUTPUT);
}

void loop()
{
  digitalWrite(2, HIGH);   
  delay(50);                       
  digitalWrite(2, LOW);    
  delay(20);                     
}