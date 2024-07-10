let table_body = document.getElementById("task_table").tBodies[0];
let editor_dialog = document.getElementById("editor");
let form = document.getElementById("editor_form");
let task_list = [];

let task_name = document.getElementById("task_name");
let task_type = document.getElementById("task_type");
let can_punish = document.getElementById("can_punish");
let can_warn = document.getElementById("can_warn");
let can_reward = document.getElementById("can_reward");
let start = document.getElementById("start_time");
let end = document.getElementById("end_time");
let interval = document.getElementById("interval_time");
let window_time = document.getElementById("window_time");
let reward_message = document.getElementById("reward_message");
let gpio = document.getElementById("gpio");

let punish_time = document.getElementById("punish_time");
let punish_intensity = document.getElementById("punish_intensity");
let punish_type = document.getElementById("punish_type");
let punish_duration = document.getElementById("punish_duration");
let punish_message = document.getElementById("punish_message");

let warning_time = document.getElementById("warning_time");
let warning_intensity = document.getElementById("warning_intensity");
let warning_type = document.getElementById("warning_type");
let warning_duration = document.getElementById("warning_duration");
let warning_message = document.getElementById("warning_message");

const formToObject = form => Object.fromEntries(new FormData(form));

function load_from_ram(){
  load_from("/ram/tasks", update_tasks);
}

function load_from_flash(){
  load_from("/flash/tasks", update_tasks);
}

function load_from_const(){
  const text = "{\"tasks\":[{\"name\":\"hydrate\",\"type\":3,\"can_punish\":true,\"can_warn\":true,\"can_reward\":true,\"start\":[9,0,0],\"end\":[23,0,0],\"interval\":[1,0,0],\"punish_time\":[0,55,0],\"punishment\":{\"intensity\":40,\"type\":\"Shock\",\"duration\":1000,\"message\":\"Drink some water. Its good for you.\"},\"warn_time\":[0,45,0],\"warning\":{\"intensity\":50,\"type\":\"Vibrate\",\"duration\":1000,\"message\":\"Its time to drink some water.\"},\"reward_message\":\"Good job\",\"gpio\":5},{\"name\":\"eat food\",\"type\":2,\"can_punish\":true,\"can_warn\":true,\"can_reward\":true,\"window\":[0,30,0],\"punish_time\":[12,45,0],\"punishment\":{\"intensity\":40,\"type\":\"Shock\",\"duration\":1000,\"message\":\"Eat something. Its good for you.\"},\"warn_time\":[12,30,0],\"warning\":{\"intensity\":50,\"type\":\"Vibrate\",\"duration\":1000,\"message\":\"its lunch time, you should have something to eat!\"},\"reward_message\":\"Good job\",\"gpio\":4},{\"name\":\"wake up\",\"type\":1,\"can_punish\":true,\"can_warn\":true,\"can_reward\":true,\"punish_time\":[8,45,0],\"punishment\":{\"intensity\":100,\"type\":\"Shock\",\"duration\":1000,\"message\":\"You really should have gotten up by now...\"},\"warn_time\":[8,30,0],\"warning\":{\"intensity\":100,\"type\":\"Vibrate\",\"duration\":1000,\"message\":\"Its time to wake up\"},\"reward_message\":\"Good morning!\",\"gpio\":3}]}";
  var obj = JSON.parse(text);
  update_tasks(obj);
}

function update_tasks(data){
  task_list.splice(0,table_body.rows.length);
  clear(table_body);
  var tasks = data.tasks;

  for (var i = 0; i < tasks.length; i++) {
    insert_task_row(tasks[i]);
  }
}

function insert_task_row(data){

  var pos = table_body.rows.length;
  var row = table_body.insertRow(pos);
  var task_select = row.insertCell(0);
  var name = row.insertCell(1);
  var type = row.insertCell(2);
  var punish = row.insertCell(3);
  var warn = row.insertCell(4);
  var reward = row.insertCell(5);
  var pin = row.insertCell(6);
  name.innerHTML=data.name;
  type.innerHTML=data.type;
  punish.innerHTML=data.can_punish;
  warn.innerHTML=data.can_warn;
  reward.innerHTML=data.can_reward;
  pin.innerHTML=data.gpio;
  task_select.innerHTML = "<input type=\"checkbox\" id=\"task_"+pos+"\">";
  task_list.push(data);
}

function delete_task(){
  for(var i = table_body.rows.length - 1; i >= 0; i--){
    var selected = document.getElementById("task_"+i).checked;
    if(selected){
      task_list.splice(i,1);
      table_body.deleteRow(i);
    }
  }
  update_tasks(to_object());
}

function load_selected(){
  //search for the row we are editing
  var temp = false;
  var pos;
  for (var i = 0; i < table_body.rows.length; i++) {
    if(document.getElementById("task_"+ i).checked){
      if(temp){
        //error out if multiple tasks are selected
        alert("error! multiple tasks selected!");
        return;
      }else{
        temp=true;
        pos = i;
      }
    }
  }
  if(temp){
    edit_task(task_list[pos]);
  }else{
    alert("no task selected!");
  }
}

function show_editor(){
  document.getElementById("overlay").style.display = "block";
  editor_dialog.open = true;
}

function edit_task(data){
    form.reset();
    task_name.value = data.name;
    task_type.value = data.type;
    can_punish.value = data.can_punish;
    can_warn.value = data.can_warn;
    can_reward.value = data.can_reward;
    gpio.value = data.gpio;
    for(var i = 0;i<form.elements.length;i++){
      form.elements[i].disabled = false;
    }
    switch(data.type){
      case 1:
        start.disabled = true;
        end.disabled = true;
        interval.disabled = true;
        window_time.disabled = true;
        break;
      case 2:
        start.disabled = true;
        end.disabled = true;
        interval.disabled = true;
        window_time.value = data.window;
        break;
      case 3:
        start.value = data.start;
        end.value = data.end;
        interval.value = data.interval;
        window_time.disabled = true;
        break;
    }
    if(data.can_punish){
      punish_time.value = data.punish_time;
      punish_intensity.value = data.punishment.intensity;
      punish_type.value = data.punishment.type;
      punish_duration.value = data.punishment.duration;
      punish_message.value = data.punishment.message;
    }else{
      punish_time.disabled = true;
      punish_intensity.disabled = true;
      punish_type.disabled = true;
      punish_duration.disabled = true;
      punish_message.disabled = true;
    }
    if(data.can_warn){
      warning_time.value = data.warn_time;
      warning_intensity.value = data.warning.intensity;
      warning_type.value = data.warning.type;
      warning_duration.value = data.warning.duration;
      warning_message.value = data.warning.message;
    }else{
      warning_time.disabled = true;
      warning_intensity.disabled = true;
      warning_type.disabled = true;
      warning_duration.disabled = true;
      warning_message.disabled = true;
    }
    if(data.can_reward){
      reward_message.value = data.reward_message;
    }else{
      reward_message.disabled = true;
    }
    show_editor();
}

function hide_editor(){
  document.getElementById("overlay").style.display = "none";
  editor_dialog.open = false;
}


function to_object(){
  var data;
  data = {tasks:[]};
  task_list.forEach(element => {
    data.tasks.push(element);
  });
  return data;
}

function on_accept(){
  delete_task();
  insert_task_row(editor_to_task());
  console.log(JSON.stringify(editor_to_task()));
  hide_editor();
}

function editor_to_task(){
  var data = {};
  data.name = task_name.value;
  data.type = JSON.parse(task_type.value);
  data.can_punish = JSON.parse(can_punish.value);
  data.can_warn = JSON.parse(can_warn.value);
  data.can_reward = JSON.parse(can_reward.value);
  switch(data.type){
    case 1:
      break;
    case 2:

    data.window = text_to_time(window_time.value);
      break;
    case 3:
      data.start = text_to_time(start.value);
      data.end = text_to_time(end.value);
      data.interval = text_to_time(interval.value);
      break;
  }
  if(data.can_punish){
    data.punish_time = text_to_time(punish_time.value);
    data.punishment = {};
    data.punishment.intensity = JSON.parse(punish_intensity.value);
    data.punishment.type = punish_type.value;
    data.punishment.duration = JSON.parse(punish_duration.value);
    data.punishment.message = punish_message.value;
  }
  if(data.can_warn){
    data.warn_time = text_to_time(warning_time.value);
    data.warning = {};
    data.warning.intensity = JSON.parse(warning_intensity.value);
    data.warning.type = warning_type.value;
    data.warning.duration = JSON.parse(warning_duration.value);
    data.warning.message = warning_message.value;
  }
  if(data.can_reward){
    data.reward_message = reward_message.value;
  }
  data.gpio = JSON.parse(gpio.value);
  return data;
}


function update_editor_reward(){
  var r = JSON.parse(can_reward.value);
  reward_message.disabled = !r;
}

function update_editor_punish(){
  var p = JSON.parse(can_punish.value);
  punish_time.disabled = !p;
  punish_intensity.disabled = !p;
  punish_type.disabled = !p;
  punish_duration.disabled = !p;
  punish_message.disabled = !p;
}

function update_editor_warn(){
  var w = JSON.parse(can_warn.value);
  warning_time.disabled = !w;
  warning_intensity.disabled = !w;
  warning_type.disabled = !w;
  warning_duration.disabled = !w;
  warning_message.disabled = !w;
}

function change_tasks_type(){
  start.disabled = false;
  end.disabled = false;
  interval.disabled = false;
  window_time.disabled = false;
  switch(JSON.parse(task_type.value)){
    case 1:
      start.disabled = true;
      end.disabled = true;
      interval.disabled = true;
      window_time.disabled = true;
      break;
    case 2:
      start.disabled = true;
      end.disabled = true;
      interval.disabled = true;
      break;
    case 3:
      window_time.disabled = true;
      break;
  }
}

function new_task(){
  form.reset();
  change_tasks_type();
  update_editor_warn();
  update_editor_reward();
  update_editor_punish();
  show_editor();
}

function write_to_flash(){
  console.log(JSON.stringify(to_object()));
}