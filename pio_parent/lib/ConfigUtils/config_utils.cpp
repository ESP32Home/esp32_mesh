#include "config_utils.h"
#include <FS.h>
#include <SPIFFS.h>
    
#define FORMAT_SPIFFS_IF_FAILED true
static bool ready = false;

bool spiffs_init(){
    if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
        Serial.println("SPIFFS Mount Failed");
        return false;
    }
    ready = true;
    return true;
}

bool load_json(DynamicJsonDocument &config,const char* FileName,bool verbose){
    if(!ready){
        if(!spiffs_init()){
            return false;
        }
    }
    File file = SPIFFS.open(FileName,"r");

    DeserializationError error = deserializeJson(config, file);
    if (error){
        Serial.println(F("Failed to read json file"));
        file.close();
        return false;
    }else{
        if(verbose){
            Serial.println();
            Serial.println("Loaded json :");
            serializeJsonPretty(config, Serial);
            Serial.println();
        }
        file.close();
        return true;
    }
}

bool save_json(DynamicJsonDocument &config,const char* FileName){
    if(!ready){
        if(!spiffs_init()){
            return false;
        }
    }
    File file = SPIFFS.open(FileName,"w");
    //TODO
    file.close();
    return true;
}

bool load_config(DynamicJsonDocument &config,bool verbose){
    bool result = load_json(config,"/config.json");
    if(verbose){
        if(result){
            Serial.println();
            Serial.println("Loaded configuration :");
            serializeJsonPretty(config, Serial);
            Serial.println();
        }else{
            Serial.println("Failed to load configuration");
        }
    }
    return result;
}

bool save_config(DynamicJsonDocument &config){
    return save_json(config,"/config.json");
}

void timelog(String Text){
  Serial.println(String(millis())+" : "+Text);//micros()
}

