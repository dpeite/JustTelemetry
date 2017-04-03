$('.editar').click(function(event){
  value = $(event.target).closest('tr').find('[name=optionsRadios]');

  code = '<div class="form"> \
  </div> \
  <h4 id="myModalLabel">Modifique los datos de la sesión.</h4>\
  <form id="form">\
  Nombre<input type="text" name="nombre"><br>\
  Descripción<input type="text" name="descripción" value=""><br>\
  </form>\
  </div>\
  <div class="modal-footer"> \
  <button type="button" id="reset" class="btn btn-danger buttonleft">Reset</button> \
  <button type="button" class="btn btn-primary save">Guardar</button> \
  </div>';
  $("#modalcontent").empty();
  $("#modalcontent").append(code);

  var coord = null;

$.getJSON( "static/data/sesiones/"+value.val()+"/info.json", ({ name: "John", time: "2pm" }) )
  .done(function() {
    console.log( "JSON Data: " + json.nombre[ 3 ].name );
  })
  .fail(function( jqxhr, textStatus, error ) {
    console.log( "JSON Data: " + json.nombre[ 3 ].name );
    var err = textStatus + ", " + error;
    console.log( "Request Failed: " + err );
});
    $('#myModal1').modal('show');


  // Guardar cambios
  $('.save').click(function(){
    if (!$('.save').hasClass('disabled')){
      $('#myModal2').modal('hide');
      console.log("huehuehue");
      console.log(value.val());
      $.get("cortar_vueltas", {lat: coord[0], lon: coord[1], id: value.val()}, function(data, status, xhr){
        // alert("Data: " + data + "\nStatus: " + status);
        console.log(xhr.status);
        $(".editar-correcto").fadeTo(2000, 500).slideUp(500, function(){
          $(".editar-correcto").slideUp(500);
        });


      })
      .fail(function(response) {
        $(".editar-incorrecto").fadeTo(2000, 500).slideUp(500, function(){
          $(".editar-incorrecto").slideUp(500);
        });
      });
    }

  }); // Cierre guardar

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
