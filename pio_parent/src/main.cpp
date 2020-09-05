#include <Arduino.h>
#include <ArduinoJson.h>

#include "config_utils.h"

DynamicJsonDocument config(5*1024);//5 KB

uint32_t cycle_count = 0;

void setup() {
  
  Serial.begin(115200);
  timelog("Boot ready");

  load_config(config);
  timelog("config loaded");

  timelog("wifi setup");

  timelog("setup() done");

}

void loop() {
  cycle_count++;
  timelog("loop start cycle "+String(cycle_count));
  delay(10000);
}
