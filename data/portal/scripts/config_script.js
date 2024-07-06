function clear_tables(){
  var table_body = document.getElementById("config_table");
  table_body.innerHTML = "";
  var table1 = document.getElementById("os_config_table");
  table1.innerHTML = "<thead></thead><tbody></tbody>";
}

function load_from_ram(){
  clear_tables();
  $.ajax({
    url:"/ram/config",
    type:"GET",
    headers:{"accept":"application/json"},
    success:update_config,
    error:on_error
  });
}

function load_from_flash(){
  clear_tables();
  $.ajax({
    url:"/flash/config",
    type:"GET",
    headers:{"accept":"application/json"},
    success:update_config,
    error:on_error
  });
}

function update_config(data){
  var config = data.config;
  update_os_config(data.config.os_config);
  var config_table_body = "<tr><td>" + config.ntp_server + "</td><td>" + config.can_override + "</td>";
  if(data.config.can_override){
    config_table_body += "<td>" + config.override_pin + "</td><td>" + config.num_overrides + "</td><td>" + config.reset_day + "</td>";
  }else{
    config_table_body += "<td>n/a</td><td>n/a</td><td>n/a</td>";
  }
  config_table_body += "<td>" + config.message_time + "</td><td>" + config.reset_time[0] + "," + config.reset_time[1] + "," + config.reset_time[2] + "</td><td>" + config.timezone[0] + "," + config.timezone[1] + "," + config.timezone[2] + "</td></tr>";
  $("#config_table").append(config_table_body);
}

function update_os_config(os_config){
  var os_config_table_body = "<tr><td>" + os_config.server + "</td><td>" + os_config.api_key + "</td><td>" + os_config.api_key + "</td></tr>";
  $("#os_config_table").append(os_config_table_body);
}

function on_error(error) {
  alert("error");
}