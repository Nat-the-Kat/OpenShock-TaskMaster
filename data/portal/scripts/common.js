function on_error(error) {
  alert("error");
}

function clear(dest){
  dest.innerHTML = "";
}


function delete_selected(list, tag, to_object, update){
  for(var i = list.rows.length - 1; i >= 0; i--){
    var selected = document.getElementById(tag+"_"+i).checked;
    if(selected){
      console.log(i);
      list.deleteRow(i);
    }
  }
  //pass the current table to the update function to reset row ids
  //this could be better...
  var o = to_object();
  clear(list);
  update(o);
}

function load_from(link, update){
  $.ajax({
    url:link,
    type:"GET",
    headers:{"accept":"application/json"},
    success:update,
    error:on_error
  });
}

function text_to_time(text){
  var out = JSON.parse("["+ text + "]");
  return out;
}

function check_form(form){
  for(var i = 0; i < form.elements.length; i++){
    console.log(i);
    if(form.elements[i].value && form.elements[i].value == "null"){
      alert("null values aren't allowed!");
      return false;
    }
  }

  return true;
}

function send_data(url, data){
  xmlHttp = new XMLHttpRequest();
  var formData = new FormData();
  formData.append("data", new Blob([data], { type: "application/json" }));
  xmlHttp.open("POST", url);
  xmlHttp.send(formData);
}