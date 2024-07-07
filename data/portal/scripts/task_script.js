let table_body = document.getElementById("task_table").tBodies[0];
let punish_body = document.getElementById("punish_table").tBodies[0];
let warning_body = document.getElementById("warning_table").tBodies[0];
let rows = table_body.rows;
let punishments = [];
let warnings = [];
let control_template = {time:[],intensity:0,type:"",duration:0,message:""};


function clear_rows(){
  table_body.innerHTML = "";
}

function load_from_ram(){
  $.ajax({
    url:"/ram/tasks",
    type:"GET",
    headers:{"accept":"application/json"},
    success:update_tasks,
    error:on_error
  });
}

function load_from_flash(){
  $.ajax({
    url:"/flash/tasks",
    type:"GET",
    headers:{"accept":"application/json"},
    success:update_tasks,
    error:on_error
  });
}

function load_from_const(){
  const text = "{\"tasks\":[{\"name\":\"hydrate\",\"type\":3,\"can_punish\":true,\"can_warn\":true,\"can_reward\":true,\"start\":[9,0,0],\"end\":[23,0,0],\"interval\":[1,0,0],\"punish_time\":[0,55,0],\"punishment\":{\"intensity\":40,\"type\":\"Shock\",\"duration\":1000,\"message\":\"Drink some water. Its good for you.\"},\"warn_time\":[0,45,0],\"warning\":{\"intensity\":50,\"type\":\"Vibrate\",\"duration\":1000,\"message\":\"Its time to drink some water.\"},\"reward_message\":\"Good job\",\"gpio\":5},{\"name\":\"eat food\",\"type\":2,\"can_punish\":true,\"can_warn\":true,\"can_reward\":true,\"window\":[0,30,0],\"punish_time\":[12,45,0],\"punishment\":{\"intensity\":40,\"type\":\"Shock\",\"duration\":1000,\"message\":\"Eat something. Its good for you.\"},\"warn_time\":[12,30,0],\"warning\":{\"intensity\":50,\"type\":\"Vibrate\",\"duration\":1000,\"message\":\"its lunch time, you should have something to eat!\"},\"reward_message\":\"Good job\",\"gpio\":4},{\"name\":\"wake up\",\"type\":1,\"can_punish\":true,\"can_warn\":true,\"can_reward\":true,\"punish_time\":[8,45,0],\"punishment\":{\"intensity\":100,\"type\":\"Shock\",\"duration\":1000,\"message\":\"You really should have gotten up by now...\"},\"warn_time\":[8,30,0],\"warning\":{\"intensity\":100,\"type\":\"Vibrate\",\"duration\":1000,\"message\":\"Its time to wake up\"},\"reward_message\":\"Good morning!\",\"gpio\":3}]}";
  var obj = JSON.parse(text);
  update_tasks(obj);
}

function update_tasks(data){
  clear_rows();
  var tasks = data.tasks;

  for (var i = 0; i < tasks.length; i++) {
    insert_task_row(tasks[i]);
  }
}

function insert_control_row(time, data, destination, id){
  var pos = destination.rows.length;
  var row = destination.insertRow(-1);
  var control_select = row.insertCell(0);
  var control_id = row.insertCell(1);
  var control_time = row.insertCell(2);
  var control_intensity = row.insertCell(3);
  var control_type = row.insertCell(4);
  var control_duration = row.insertCell(5);
  var control_message = row.insertCell(6);
  control_select.innerHTML = "<input type=\"checkbox\" id=\"" + id + "_" + pos + "\">";
  control_id.innerHTML = pos;
  control_time.innerHTML = time;
  control_intensity.innerHTML = data.intensity;
  control_type.innerHTML = data.type;
  control_duration.innerHTML = data.duration;
  control_message.innerHTML = data.message;
  var row_select = document.getElementById(id+"_" + pos);
  row_select.addEventListener("click", on_test, false);
  return pos;
}

function insert_task_row(data){
  var pos = table_body.rows.length;
  var row = table_body.insertRow(-1);
  var task_select = row.insertCell(0);
  var name = row.insertCell(1);
  var type = row.insertCell(2);
  var can_punish = row.insertCell(3);
  var can_warn = row.insertCell(4);
  var can_reward = row.insertCell(5);
  var start = row.insertCell(6);
  var end = row.insertCell(7);
  var interval = row.insertCell(8);
  var window = row.insertCell(9);
  var punish_id = row.insertCell(10);
  var warning_id = row.insertCell(11);
  var reward_message = row.insertCell(12);
  var gpio = row.insertCell(13);
  name.innerHTML=data.name;
  type.innerHTML=data.type;
  can_punish.innerHTML=data.can_punish;
  can_warn.innerHTML=data.can_warn;
  can_reward.innerHTML=data.can_reward;
  gpio.innerHTML=data.gpio;
  console.log(data.can_punish);
  switch(data.type){
    case 1:
      start.innerHTML = "n/a";
      end.innerHTML = "n/a";
      interval.innerHTML = "n/a";
      window.innerHTML = "n/a";
      break;
    case 2:
      start.innerHTML = "n/a";
      end.innerHTML = "n/a";
      interval.innerHTML = "n/a";
      window.innerHTML = data.window;
      break;
    case 3:
      start.innerHTML = data.start;
      end.innerHTML = data.end;
      interval.innerHTML = data.interval;
      window.innerHTML = "n/a";
      break;
  }
  if(data.can_punish){
    punish_id = insert_control_row(data.punish_time, data.punishment, punish_body, "punishment");
  }
  if(data.can_warn){
    warning_id = insert_control_row(data.warn_time, data.warning, warning_body, "warning");
  }
  if(data.can_reward){
    reward_message.innerHTML = data.reward_message;
  }

  task_select.innerHTML = "<input type=\"checkbox\" id=\"task_"+pos+"\">";

 // var row_select = document.getElementById("task_"+ pos);
  //row_select.addEventListener("click",on_test,false);
}

function on_test(){

}

function delete_selected(){
  for(var i = rows.length - 1; i >= 0; i--){
    var selected = document.getElementById("row_"+i).checked;
    if(selected){
      console.log(i);
      table_body.deleteRow(i);
    }
  }
  //pass the current table to update_networks to reset row ids
  //this could be better...
  var o = to_object();
  clear_rows();
  update_networks(o);
}

function add_punishment(){
  var id;


  return id;
}
function add_punishment(time,data){
  var temp = {time:[],intensity,type,duration,message};
  temp.time= time;
  temp.intensity = data.intensity;
  temp.type = data.type;
  temp.duration = data.duration;
  temp.message = data.message;
  return insert_control_row(temp, punish_body, "punishment");
}

function add_warning(){
  var id;


  return id;
}

function add_warning(time,data){
  var temp = {time:[],intensity,type,duration,message};
  temp.time= time;
  temp.intensity = data.intensity;
  temp.type = data.type;
  temp.duration = data.duration;
  temp.message = data.message;
  return insert_control_row(temp, warning_body, "warning");
}
