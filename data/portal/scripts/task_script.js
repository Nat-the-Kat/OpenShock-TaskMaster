function clear_tables(){
  var table = document.getElementById("task_table");
  table.innerHTML = "";

}

function load_from_ram(){
  clear_tables();
  $.ajax({
    url:"/ram/tasks",
    type:"GET",
    headers:{"accept":"application/json"},
    success:update_tasks,
    error:on_error
  });
}

function load_from_flash(){
  clear_tables();
  $.ajax({
    url:"/flash/tasks",
    type:"GET",
    headers:{"accept":"application/json"},
    success:update_tasks,
    error:on_error
  });
}

function update_tasks(data){
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

  $("#task_table").append(task_table);
}

function on_error(error) {
  alert("error");
}