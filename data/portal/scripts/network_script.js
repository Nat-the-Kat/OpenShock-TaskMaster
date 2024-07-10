let table_body = document.getElementById("network_table").getElementsByTagName("tbody").item(0);
let rows = table_body.rows;


function load_from_ram(){
  load_from("/ram/networks", update_networks);
}

/*function load_from_flash(){
  load_from("/flash/networks", update_networks);
}

function load_from_const(){
  const text = "{\"networks\":[{\"ssid\":\"1ssid\",\"password\":\"1password\"},{\"ssid\":\"ssid1\",\"password\":\"password1\"},{\"ssid\":\"ssid2\",\"password\":\"password2\"}]}";
  var obj = JSON.parse(text);
  update_networks(obj);
}*/

function update_networks(data){
  clear(table_body);
  var networks = data.networks;

  for (var i = 0; i < networks.length; i++) {
    insert_row(networks[i].ssid,networks[i].password);
  }
}

function insert_row(ssid, password){
  if(ssid==""){
    alert("ssid field is empty!");
    return;
  }
  var pos = rows.length;
  var row = table_body.insertRow(-1);
  var network_select = row.insertCell(0);
  var s = row.insertCell(1);
  var p = row.insertCell(2);
  network_select.innerHTML = "<input type=\"checkbox\" id=\"row_"+pos+"\">";
  s.innerHTML=ssid;
  p.innerHTML=password; 
}

function add_network(){
  var add_ssid = document.getElementById("add_ssid").value;
  var add_password = document.getElementById("add_password").value;
  insert_row(add_ssid,add_password);
}

function write_to_flash(){
  var j = JSON.stringify(to_object());
  send_data("/flash/networks", j);
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
