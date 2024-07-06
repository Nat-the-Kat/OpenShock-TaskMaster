function clear_tables(){
  var table = document.getElementById("network_table");
  table.innerHTML = "<!--network_script fills this in-->";

}

function load_from_ram(){
  clear_tables();
  $.ajax({
    url:"/ram/networks",
    type:"GET",
    headers:{"accept":"application/json"},
    success:update_networks,
    error:on_error
  });
}

function load_from_flash(){
  clear_tables();
  $.ajax({
    url:"/flash/networks",
    type:"GET",
    headers:{"accept":"application/json"},
    success:update_networks,
    error:on_error
  });
}

function update_networks(data){
  var networks = data.networks;

  var network_table_body = "";

  for (var i = 0; i < networks.length; i++) {
    network_table_body += "<tr><td>" + networks[i].ssid + "</td><td>" + networks[i].password + "</td></tr>";
  }

  $("#network_table").append(network_table_body);
}

function on_error(error) {
  alert("error");
}