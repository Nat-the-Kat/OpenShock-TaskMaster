let table_body = document.getElementById("task_table_body");


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

  var task_table = "";

  for (var i = 0; i < tasks.length; i++) {
    task_table += "<tr><td>" + tasks[i].name + "</td><td>" + tasks[i].type + "</td><td>" + tasks[i].can_punish + "</td><td>" + tasks[i].can_warn + "</td><td>" + tasks[i].can_reward + "</td>";
    if(tasks[i].type == 1){
      task_table += "<td>n/a</td><td>n/a</td><td>n/a</td><td>n/a</td>";
    }else if(tasks[i].type == 2){
      task_table += "<td>n/a</td><td>n/a</td><td>n/a</td><td>" + tasks[i].window[0] + ":" + tasks[i].window[1] + ":" + tasks[i].window[2] + "</td>";
    }else if(tasks[i].type == 3){
      task_table += "<td>" + tasks[i].start[0] + ":" + tasks[i].start[1] + ":" + tasks[i].start[2] + "</td><td>" + tasks[i].end[0] + ":" + tasks[i].end[1] + ":" + tasks[i].end[2] + "</td><td>" + tasks[i].interval[0] + ":" + tasks[i].interval[1] + ":" + tasks[i].interval[2] + "</td><td>n/a</td>";
    }

    if(tasks[i].can_punish){
      task_table += "<td>" + tasks[i].punish_time[0] + ":" + tasks[i].punish_time[1] + ":" + tasks[i].punish_time[2] + "</td><td>" + tasks[i].punishment.intensity + "</td><td>" + tasks[i].punishment.type + "</td><td>" + tasks[i].punishment.duration + "</td><td>" + tasks[i].punishment.message + "</td>";
    }else{
      task_table += "<td>n/a</td><td>n/a</td><td>n/a</td><td>n/a</td><td>n/a</td>";
    }
    if(tasks[i].can_warn){
      task_table += "<td>" + tasks[i].warn_time[0] + ":" + tasks[i].warn_time[1] + ":" + tasks[i].warn_time[2] + "</td><td>" + tasks[i].warning.intensity + "</td><td>" + tasks[i].warning.type + "</td><td>" + tasks[i].warning.duration + "</td><td>" + tasks[i].warning.message + "</td>";
    }else{
      task_table += "<td>n/a</td><td>n/a</td><td>n/a</td><td>n/a</td><td>n/a</td>";
    }
    if(tasks[i].can_reward){
      task_table += "<td>" + tasks[i].reward_message + "</td>";
    }else{
      task_table += "<td>n/a</td>";
    }
    task_table += "<td>" + tasks[i].gpio + "</td></tr>";
  }

  $("#task_table_body").append(task_table);
}
