let table_body = document.getElementById("network_table").getElementsByTagName("tbody").item(0);
let rows = table_body.rows;


function load_from_ram(){
  $.ajax({
    url:"/ram/networks",
    type:"GET",
    headers:{"accept":"application/json"},
    success:update_networks,
    error:on_error
  });
}

function load_from_flash(){
  $.ajax({
    url:"/flash/networks",
    type:"GET",
    headers:{"accept":"application/json"},
    success:update_networks,
    error:on_error
  });
}

function load_from_const(){
  const text = "{\"networks\":[{\"ssid\":\"1ssid\",\"password\":\"1password\"},{\"ssid\":\"ssid1\",\"password\":\"password1\"},{\"ssid\":\"ssid2\",\"password\":\"password2\"}]}";
  var obj = JSON.parse(text);
  update_networks(obj);
}

function update_networks(data){
  clear(table_body);
  var networks = data.networks;

  for (var i = 0; i < networks.length; i++) {
    insert_row(networks[i].ssid,networks[i].password);
  }
}

function insert_row(ssid, password){
  var pos = rows.length;
  var row = table_body.insertRow(-1);
  var network_select = row.insertCell(0);
  var s = row.insertCell(1);
  var p = row.insertCell(2);
  network_select.innerHTML = "<input type=\"checkbox\" id=\"row_"+pos+"\">";
  s.innerHTML=ssid;
  p.innerHTML=password; 
  var row_select = document.getElementById("row_"+ pos);
  row_select.addEventListener("click",on_test,false);
}

function add_network(){
  var add_ssid = document.getElementById("add_ssid").value;
  var add_password = document.getElementById("add_password").value;
  insert_row(add_ssid,add_password);
}

function on_test(){
  console.log(this);
}

function write_to_ram(){
  var j = JSON.stringify(to_object());
  console.log(j);
}

function to_object(){
  var out = {networks:[]};
  for(var i=0;i<rows.length;i++){
    var temp = {ssid:"",password:""};
    temp.ssid = rows[i].cells[1].innerHTML;
    temp.password = rows[i].cells[2].innerHTML;
    out.networks.push(temp);
  }
  console.log(out);
  return out;
}
function delete_objects(){
  delete_selected(table_body, "row", to_object, update_networks);
}
