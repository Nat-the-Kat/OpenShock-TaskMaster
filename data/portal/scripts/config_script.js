function clear_tables(){
  var table = document.getElementById("config_table");
  table.innerHTML = "<thead></thead><tbody></tbody>";
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
  var values = data.config;
  update_os_config(data.config.os_config);
  var config_table_head = '<tr><td>ntp server</td><td>can override</td>';
  if(data.config.can_override){
    config_table_head += '<td>override pin</td><td>number of overrides</td><td>reset day</td>';
  }
  config_table_head += '<td>message time</td><td>reset time</td><td>timezone</td></tr>';
  $('#config_table thead').append(config_table_head);

  var config_table_body = '<tr>';
  config_table_body += '<tr><td>' + values.ntp_server + '</td><td>' + values.can_override + '</td>';
  if(data.config.can_override){
    config_table_body += '<td>' + values.override_pin + '</td><td>' + values.num_overrides + '</td><td>' + values.reset_day + '</td>';
  }
  config_table_body += '<td>' + values.message_time + '</td><td>' + values.reset_time[0] + ',' + values.reset_time[1] + ',' + values.reset_time[2] + '</td><td>' + values.timezone[0] + ',' + values.timezone[1] + ',' + values.timezone[2] + '</td></tr>';
  $('#config_table tbody').append(config_table_body);
}

function update_os_config(os_config){
  var os_config_table_head = '<tr><td>OpenShock server</td><td>api key</td><td>shocker</td></tr>';
  $('#os_config_table thead').append(os_config_table_head);
  var os_config_table_body = '<tr><td>' + os_config.server + '</td><td>' + os_config.api_key + '</td><td>' + os_config.api_key + '</td></tr>';
  $('#os_config_table tbody').append(os_config_table_body);
}

function on_error(error) {
  alert('error');
}