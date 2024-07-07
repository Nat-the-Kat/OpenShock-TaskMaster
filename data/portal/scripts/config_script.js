let enabled = false;

function load_from_ram(){
  $.ajax({
    url:"/ram/config",
    type:"GET",
    headers:{"accept":"application/json"},
    success:update_config,
    error:on_error
  });
}

function load_from_flash(){
  $.ajax({
    url:"/flash/config",
    type:"GET",
    headers:{"accept":"application/json"},
    success:update_config,
    error:on_error
  });
}

function load_from_const(){
  const text = "{\"config\":{\"ntp_server\":\"pool.ntp.org\",\"os_config\":{\"server\":\"null\",\"api_key\":\"null\",\"shocker\":\"null\"},\"can_override\":true,\"override_pin\":1,\"num_overrides\":1,\"reset_day\":0,\"message_time\":3,\"reset_time\":[0,0,0],\"timezone\":[-7,0,0]}}";
  var obj = JSON.parse(text);
  update_config(obj);
}

function update_config(data){
  if(!enabled){
    enable_all();
  }
  var config = data.config;
  update_os_config(data.config.os_config);
  document.getElementById("ntp_server").value = config.ntp_server;
  document.getElementById("can_override").value = config.can_override;
  if(data.config.can_override){
    document.getElementById("override_pin").value = config.override_pin;
    document.getElementById("num_overrides").value = config.num_overrides;
    document.getElementById("reset_day").value = config.reset_day;
  }else{
    document.getElementById("override_pin").disabled = true;
    document.getElementById("num_overrides").disabled = true;
    document.getElementById("reset_day").disabled = true;
  }
  document.getElementById("message_time").value = config.message_time;
  var temp = "";
  temp += config.reset_time[0] + "," + config.reset_time[1] + "," + config.reset_time[2];
  document.getElementById("reset_time").value = temp;
  temp = "";
  temp += config.timezone[0] + "," + config.timezone[1] + "," + config.timezone[2];
  document.getElementById("timezone").value = temp;
}

function update_os_config(os_config){
  document.getElementById("os_server").value = os_config.server;
  document.getElementById("api_key").value = os_config.api_key;
  document.getElementById("shocker_id").value = os_config.shocker;
}

function write_to_ram(){
  var out = {config:{}};
  //this is kinda gross, but it works. i am so in unknown territory at this point...
  if(document.getElementById("can_override").value == "true"){
    out.config = {ntp_server, os_config:{server:"",api_key:"",shocker:""}, can_override, override_pin, num_overrides, reset_day, message_time, reset_time:[], timezone:[]};
    out.config.ntp_server = document.getElementById("ntp_server").value;
    out.config.os_config.server = document.getElementById("os_server").value;
    out.config.os_config.api_key = document.getElementById("api_key").value;
    out.config.os_config.shocker = document.getElementById("shocker_id").value;
    out.config.can_override = document.getElementById("can_override").value;
    out.config.override_pin = document.getElementById("override_pin").value;
    out.config.num_overrides = document.getElementById("num_overrides").value;
    out.config.reset_day = document.getElementById("reset_day").value;
    out.config.message_time = document.getElementById("message_time").value;
    out.config.reset_time = JSON.parse("[" + document.getElementById("reset_time").value + "]");
    out.config.timezone = JSON.parse("[" + document.getElementById("reset_time").value + "]");
  }else{
    out.config = {ntp_server, os_config:{server:"",api_key:"",shocker:""}, can_override, message_time, reset_time:[], timezone:[]};
    out.config.ntp_server = document.getElementById("ntp_server").value;
    out.config.os_config.server = document.getElementById("os_server").value;
    out.config.os_config.api_key = document.getElementById("api_key").value;
    out.config.os_config.shocker = document.getElementById("shocker_id").value;
    out.config.can_override = document.getElementById("can_override").value;
    out.config.message_time = document.getElementById("message_time").value;
    out.config.reset_time = JSON.parse("["+document.getElementById("reset_time").value + "]");
    out.config.timezone = JSON.parse("["+document.getElementById("reset_time").value + "]");
  }
  var j = JSON.stringify(out);
  console.log(j);
} 

function update_override(){
  var temp = JSON.parse(document.getElementById("can_override").value);
  document.getElementById("override_pin").disabled = !temp;
  document.getElementById("num_overrides").disabled = !temp;
  document.getElementById("reset_day").disabled = !temp;
}

function enable_all(){
  var inputs = document.getElementById("config_body").getElementsByTagName("input");
  document.getElementById("can_override").disabled = false;
  for(var i = 0; i<inputs.length;i++){
    inputs[i].disabled = false;
  }
  document.getElementById("os_server").disabled = false;
  document.getElementById("api_key").disabled = false;
  document.getElementById("shocker_id").disabled = false;
  enabled = true;
}