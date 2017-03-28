$('.vueltas').click(function(event){
  value = $(event.target).closest('tr').find('[name=optionsRadios]');

  code = '<div class="flot-chart"> \
  <div class="flot-chart-content" id="vueltas"></div> \
  </div> \
  </div> \
  <div class="modal-footer"> \
  <button type="button" class="btn btn-danger delete_v buttonleft" data-dismiss="modal">Borrar</button> \
  <button type="button" class="btn btn-primary save disabled">Guardar</button> \
  </div>';
  $("#modalcontent").empty();
  $("#modalcontent").append(code);

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
      $('#myModal1').modal('hide');
	console.log("huehuehue");
	console.log(value.val());
	$.get("cortar_vueltas", {lat: coord[0], lon: coord[1], id: value.val()}, function(data, status, xhr){
        // alert("Data: " + data + "\nStatus: " + status);
        console.log(xhr.status);
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

