function carga() {

  $.get("info_sesiones", null, function(data, status, xhr){
  		console.log("Esto es un mundo")

  		console.log(data)

  		var plot = $.plot("#morris-bar-chart", [data]);
  })
  .fail(function(response) {

  });
} // Cierre carga()