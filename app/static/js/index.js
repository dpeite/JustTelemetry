function carga() {

  $.get("info_sesiones", null, function(data, status, xhr){
  		var plot = $.plot("#morris-bar-chart", [data]);
  })
  .fail(function(response) {

  });
} // Cierre carga()