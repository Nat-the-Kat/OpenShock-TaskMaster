#include "oscontrol.h"

void task_master::control_request(task_master::config* conf, task_master::control c){
  HTTPClient http;

  http.setInsecure();
  http.begin("https://api.openshock.app/2/shockers/control");
  http.addHeader("Content-Type", "application/json");
  http.addHeader("accept", "application/json", false, false);
  http.addHeader("OpenShockToken", conf->api_key.c_str(), false, false);


  //behold, the extremely cursed way i created the body of the request before i got smart and used ArduinoJson
  /*
  //compose the http body from the function arguments
  String request;
  request += "{\"shocks\": [\n{\n\"id\": \"";
  request += SHOCKER;
  request += "\",\n\"type\": \"";
  request += control;
  request += "\",\n\"intensity\": ";
  request += String(intensity);
  request += ",\n\"duration\": ";
  request += String(duration);
  request += ",\n\"exclusive\": true\n}\n],\n\"customName\": \"";
  request += reason;
  request += "\"\n}";
  */
  JsonDocument doc;

  JsonArray shocks = doc["shocks"].to<JsonArray>();
  JsonObject shock = shocks.add<JsonObject>();

  shock["id"] = conf->shocker.c_str();
  shock["type"] = c.type.c_str();
  shock["intensity"] = c.strength;
  shock["duration"] = c.dur;
  shock["exclusive"] = true;

  doc["customName"] = c.message.c_str();

  String request;

  doc.shrinkToFit();  // optional

  serializeJson(doc, request);
  int httpCode = http.POST(request);
  if(httpCode > 0){
    Serial.printf("[HTTP] POST... code: %d\n", httpCode);

    const String& payload = http.getString();
    Serial.println("received payload:\n<<");
    Serial.println(payload);
    Serial.println(">>");
  }else{
    Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
}
