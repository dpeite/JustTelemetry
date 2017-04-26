$('.vueltas').click(function(event){
  value = $(event.target).closest('tr').find('[name=optionsRadios]');

  code = '<div class="flot-chart"> \
  <div class="flot-chart-content-wide" id="vueltas"></div> \
  </div> \
  </div> \
  <div class="modal-footer"> \
  <button type="button" class="btn btn-danger delete_v buttonleft" data-dismiss="modal">Borrar</button> \
  <button type="button" class="btn btn-primary save disabled">Guardar</button> \
  </div>';
  $("#modalcontent").empty();
  $("#modalcontent").append(code);
  $(".modal-title").empty();
  $(".modal-title").append("Seleccionar punto inicio vuelta");
  
  var coord = null;

  $.getJSON("static/data/sesiones/"+value.val()+"/trazada.json", function(d2){
    var options = {
      points: { show: false, radius: 5, lineWidth: 4, fill: false },
      lines: { show: true },
      xaxis: { show: false },
      yaxis: { show: false },
      grid: { show:true,
        color: "transparent",
        hoverable: true,
        clickable: true
      }
    };

    var plot = $.plot("#vueltas", [d2], options);

    $("#vueltas").bind("plotclick", function (event, pos, item) {
      if (item) {
        if (coord == null){
          coord = item.datapoint;
          plot.highlight(item.series, item.datapoint);
        }
        else {
          plot.unhighlight(item.series, coord);
          coord = item.datapoint;
          plot.highlight(item.series, coord);
        }
        $('.save').removeClass('disabled');

      }
      console.log(coord);
    });
  });
  $('#myModal1').modal('show');

  $('.save').click(function(){
    if (!$('.save').hasClass('disabled')){

          code = '<div class="flot-chart"> \
                  <div id="loading"> \
                  <i class="fa fa-refresh fa-spin fa-5x fa-fw img-responsive"></i> \
                  <span class="sr-only">Loading...</span> \
                  </div> \
           </div>';
        $("#modalcontent").empty();
        $("#modalcontent").append(code);
        $(".modal-title").empty();
        //$(".modal-title").append("Seleccionar punto inicio vuelta");


      console.log("huehuehue");
      console.log(value.val());
      $.get("cortar_vueltas", {lat: coord[0], lon: coord[1], id: value.val()}, function(data, status, xhr){
        // alert("Data: " + data + "\nStatus: " + status);
        console.log(xhr.status);
       $('#myModal1').modal('hide');
        $(".cortar-vueltas-correcto").fadeTo(2000, 500).slideUp(500, function(){
          $(".cortar-vueltas-correcto").slideUp(500);
        });


      })
      .fail(function(response) {
        $(".cortar-vueltas-incorrecto").fadeTo(2000, 500).slideUp(500, function(){
          $(".cortar-vueltas-incorrecto").slideUp(500);
        });
      });
    }

  });

  // Borrar las vueltas
  $('.delete_v').click(function(){
    $.get("borrar_vuelta", {id: value.val()}, function(data, status, xhr){
      // alert("Data: " + data + "\nStatus: " + status);
      console.log(xhr.status);
      $(".borrar-vueltas-correcto").fadeTo(2000, 500).slideUp(500, function(){
        $(".borrar-vueltas-correcto").slideUp(500);
      });


    })
    .fail(function(response) {
      $(".borrar-vueltas-incorrecto").fadeTo(2000, 500).slideUp(500, function(){
        $(".borrar-vueltas-incorrecto").slideUp(500);
      });
    });
  }); // Cierre delete_v
}); // Cierre event


// Borrar la sesion
$('.delete_s').click(function(){
  console.log(this.id)
  $.get("borrar_sesion", {id: this.id.split("-")[1]}, function(data, status, xhr){
    // alert("Data: " + data + "\nStatus: " + status);
    console.log(xhr.status);
    $(".borrar-sesiones-correcto").fadeTo(2000, 500).slideUp(500, function(){
      $(".borrar-sesiones-correcto").slideUp(500);
    });

    // Actualizamos la info de la dashboard
      $.get("info_sesiones", null, function(data, status, xhr){});
      location.reload(true);
  })
  .fail(function(response) {
    $(".borrar-sesiones-incorrecto").fadeTo(2000, 500).slideUp(500, function(){
      $(".borrar-sesiones-incorrecto").slideUp(500);
    });
  });
}); // Cierre delete_v



$(function() {
  $("input[name=optionsRadios][value="+getCookie("id")+"]").prop("checked",true);
});


var radioButtonValue;
$('input[name="optionsRadios"]').on("click", function(e) {
  var value = $(this).val();
  console.log(value);
  document.cookie = "id="+value;
  console.log(document.cookie);
});

$('.tooltip-demo').tooltip({
  selector: "[data-toggle=tooltip]",
  container: "body"
});

$('.editar').click(function(event){
  var id  = this.id.split("-")[1]
  console.log($('#descripcion'+id).val())
  code = '<div class="modal-body"> \
  <div class="row"> \
  <div class="col-xs-12"> \
  <form id="editar"> \
  <div class="form-group"> \
  <label for="editar-nombre">Nombre</label> \
  <input type="text" name="nombre" class="form-control" id="editar-nombre" value="'+$('#nombre'+id).text()+'">\
  </div> \
  <div class="form-group"> \
  <label for="editar-descripcion">Descripción</label> \
  <input type="text" name="descripcion" class="form-control" id="editar-descripcion" value="'+$('#descripcion'+id).text().trim()+'"> \
  <label for="editar-descripcion">Fecha</label> \
  <input type="date" name="fecha" class="form-control" id="editar-fecha" value="'+$('#fecha'+id).text()+'"> \
  </div> \
  </form> \
  </div> \
  </div> \
  </div> \
  <div class="modal-footer"> \
  <button type="button" class="btn btn-danger buttonleft" data-dismiss="modal">Cancelar</button> \
  <button type="button" class="btn btn-primary save">Guardar</button> \
  </div>';

  $("#modalcontent").empty();
  $(".modal-title").empty();
  $("#modalcontent").append(code);
  $(".modal-title").append("Editar datos de la sesión");
  $('#myModal1').modal('show');

  $('.save').click(function(){
    console.log()
    var $inputs = $('#editar :input');
    var values = {};
    $inputs.each(function() {
      values[this.name] = $(this).val();
    });
    console.log(values)
    $('#myModal1').modal('hide');

    values["id"] = id
    console.log(values)
    $.post("editar_datos", {datos : values}, function(data, status, xhr){
      $(".cortar-vueltas-correcto").fadeTo(2000, 500).slideUp(500, function(){
        $(".cortar-vueltas-correcto").slideUp(500);
      });
      $('#nombre'+id).load(location.href +  ' #nombre'+id);
      $('#fecha'+id).load(location.href +  ' #fecha'+id);
      $('#tiempo'+id).load(location.href +  ' #tiempo'+id);
      $('#metros'+id).load(location.href +  ' #metros'+id);
      $('#descripcion'+id).load(location.href +  ' #descripcion'+id+" .panel-body");
    // Actualizamos la info de la dashboard
      $.get("info_sesiones", null, function(data, status, xhr){});

    })
    .fail(function(response) {
      $(".cortar-vueltas-incorrecto").fadeTo(2000, 500).slideUp(500, function(){
        $(".cortar-vueltas-incorrecto").slideUp(500);
      });
    });


  });
});

$('#subir_sesion').click(function(){
  console.log("siii")
  var data = new FormData();

  if ($('.nav-tabs .active').text() == "Desde fichero") {
      jQuery.each(jQuery('#file')[0].files, function(i, file) {
          data.append('file-'+i, file);
      });

      var other_data = $('#upload_sesion').serializeArray();
      $.each(other_data,function(key,input){
        console.log(input.name)
          if (input.value){
            data.append(input.name,input.value);
          }
    });
  } // Cierre IF Desde fichero
  else {
    var other_data = $('#upload_sesion_ip').serializeArray();
      $.each(other_data,function(key,input){
        console.log(input.name)
          if (input.value){
            console.log(input.value)
            data.append(input.name,input.value);
          }
    });
  }

  jQuery.ajax({
    url: 'upload_sesion',
    data: data,
    cache: false,
    contentType: false,
    processData: false,
    type: 'POST',
    success: function(data){
      $('#myModal').modal('hide');

      // Actualizamos la info de la dashboard
      $.get("info_sesiones", null, function(data, status, xhr){});
      location.reload(true);
    }
  });

});
