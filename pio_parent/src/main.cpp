#include <Arduino.h>
#include <ArduinoJson.h>

#include "config_utils.h"
#include "mesh_app.h"

DynamicJsonDocument config(5*1024);//5 KB
DynamicJsonDocument secret(1*1024);//1 KB
MeshApp mesh;

uint32_t cycle_count = 0;

void setup() {
  
  Serial.begin(115200);
  timelog("Boot ready");

  load_config(config,true);
  load_json(secret,"/secret.json");
  timelog("config loaded");

  mesh.start(config,secret);

  timelog("setup() done");

}

void loop() {
  cycle_count++;
  timelog("loop start cycle "+String(cycle_count));
  delay(10000);
}
