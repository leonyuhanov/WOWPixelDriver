#include <ESP8266WiFi.h>

void setup()
{
  Serial.begin(115200);
  Serial.printf("\r\n\r\nDevices WIFI_STA MAC Address is\t");
  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());
}

void loop()
{   
	delay(1000000);
}
