#include "control_request.h"

void openshock::control_request(openshock::config conf, openshock::control c){
  HTTPClient http;

  http.setInsecure();
  http.begin("https://api.openshock.app/2/shockers/control");
  http.addHeader("Content-Type", "application/json");
  http.addHeader("accept", "application/json", false, false);
  http.addHeader("OpenShockToken", conf.api_key.c_str(), false, false);


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

  shock["id"] = conf.shocker.c_str();
  shock["type"] = c.type.c_str();
  shock["intensity"] = c.intensity;
  shock["duration"] = c.duration;
  shock["exclusive"] = true;

  doc["customName"] = c.message.c_str();

  String request;

  doc.shrinkToFit();  // optional

  serializeJson(doc, request);
  int http_code = http.POST(request);
  if(http_code > 0){
    if(http_code != HTTP_CODE_OK){
      Serial.printf("[HTTP] POST... code: %d\n", http_code);
      const String& payload = http.getString();
      Serial.println("received payload:\n<<");
      Serial.println(payload);
      Serial.println(">>");
    }

  }else{
    Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(http_code).c_str());
  }
  http.end();
}
