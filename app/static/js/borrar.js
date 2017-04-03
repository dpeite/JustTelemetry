$('.borrar').click(function(event){
  value = $(event.target).closest('tr').find('[name=optionsRadios]');

  code = '<div class="form"> \
  </div> \
  <h5 id="myModalLabel">¿Quieres realmente borrar todos los datos de esta sesión?</h5>\
  </div>\
  <div class="modal-footer"> \
  <button type="button" id="reset" class="btn btn-danger buttonleft borrar" >Borrar</button> \
  <button type="button" class="btn btn-primary" data-dismiss="modal">Cancelar</button> \
  </div>';
  $("#modalcontent").empty();
  $("#modalcontent").append(code);
  $('#myModal1').modal('show');


  // Borrar datos
  $('.borrar').click(function(){
   $.get("borrar_vuelta", {id: value.val()}, function(data, status, xhr){
        // alert("Data: " + data + "\nStatus: " + status);
        console.log(xhr.status);
        $(".borrar-vueltas-correcto").fadeTo(2000, 500).slideUp(500, function(){
          $(".borrar-vueltas-correcto").slideUp(500);
        });


      })
   .fail(function(response) {
    $(".editar-incorrecto").fadeTo(2000, 500).slideUp(500, function(){
      $(".editar-incorrecto").slideUp(500);
    });
  });
  }); // Cierre borrar

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
