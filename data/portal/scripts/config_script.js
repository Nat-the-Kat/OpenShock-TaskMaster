let enabled = false;

let ntp_server = document.getElementById("ntp_server");
let can_override = document.getElementById("can_override");
let override_pin = document.getElementById("override_pin");
let num_overrides = document.getElementById("num_overrides");
let reset_day = document.getElementById("reset_day");
let message_time = document.getElementById("message_time");
let reset_time = document.getElementById("reset_time");
let timezone = document.getElementById("timezone");
let os_server = document.getElementById("os_server");
let api_key = document.getElementById("api_key");
let shocker_id = document.getElementById("shocker_id");

let config_form = document.getElementById("config_form");

let zones = document.getElementById("zones");

let zone_names = [];
let zone_rules = [];
let zone_id = 0;

function on_load(){
  load_zones();
  load_from_ram();
  
}

function load_from_ram(){
  load_from("/data/config", update_config);
}


function load_zones(){
  var tz_parsed = JSON.parse(tz_text);
  tz_parsed.tz_data.forEach(item =>{
    var option = document.createElement("option");
    option.value = item[0];
    zones.appendChild(option);
    zone_names.push(item[0]);
    zone_rules.push(item[1]);
  });
}

function zone_changed(){
  scan_zones(timezone.value);
}

function scan_zones(input){
  for(var i =0; i < zone_names.length;i++){
    if(input == zone_names[i]){
      console.log(zone_names[i]);
      zone_id = i;
      return;
    }
  }
  zone_id = 457; //if we reach this point, there wasn't a match, so default to utc
}

/*function load_from_const(){
  const text = "{\"config\":{\"ntp_server\":\"pool.ntp.org\",\"os_config\":{\"server\":\"null\",\"api_key\":\"null\",\"shocker\":\"null\"},\"can_override\":true,\"override_pin\":1,\"num_overrides\":1,\"reset_day\":0,\"message_time\":3,\"reset_time\":[0,0,0],\"timezone_name\":\"America/Los_Angeles\",\"timezone_rules\":\"PST8PDT,M3.2.0,M11.1.0\"}}";
  var obj = JSON.parse(text);
  update_config(obj);
}*/

function update_config(data){
  if(!enabled){
    enable_all();
  }
  var config = data.config;
  update_os_config(data.config.os_config);
  ntp_server.value = config.ntp_server;
  can_override.value = config.can_override;
  if(data.config.can_override){
    override_pin.value = config.override_pin;
    num_overrides.value = config.num_overrides;
    reset_day.value = config.reset_day;
  }else{
    override_pin.disabled = true;
    num_overrides.disabled = true;
    reset_day.disabled = true;
  }
  message_time.value = config.message_time;
  reset_time.value = config.reset_time;
  timezone.value = config.timezone_name;
  scan_zones(config.timezone_name);

}

function update_os_config(os_config){
  os_server.value = os_config.server;
  api_key.value = os_config.api_key;
  shocker_id.value = os_config.shocker;
}

function write_to_flash(){
  var out = {config:{}};
  out.config = {os_config:{server:"",api_key:"",shocker:""}};
  if(can_override.value == "true"){
    out.config.override_pin = override_pin.value;
    out.config.num_overrides = num_overrides.value;
    out.config.reset_day = reset_day.value;
  }
  out.config.ntp_server = ntp_server.value;
  out.config.os_config.server = os_server.value;
  out.config.os_config.api_key = api_key.value;
  out.config.os_config.shocker = shocker_id.value;
  out.config.can_override = can_override.value;
  out.config.message_time = message_time.value;
  out.config.reset_time = text_to_time(reset_time.value);
  out.config.timezone_name = zone_names[zone_id];
  out.config.timezone_rule = zone_rules[zone_id];
  var j = JSON.stringify(out);
  if(check_form(config_form)){
    send_data("/data/config", j);
  console.log(j);
  }
} 

function update_override(){
  var temp = JSON.parse(can_override.value);
  override_pin.disabled = !temp;
  num_overrides.disabled = !temp;
  reset_day.disabled = !temp;
}

function enable_all(){
  var inputs = config_form.elements;
  can_override.disabled = false;
  for(var i = 0; i<inputs.length;i++){
    inputs[i].disabled = false;
  }
  os_server.disabled = false;
  api_key.disabled = false;
  shocker_id.disabled = false;
  enabled = true;
}