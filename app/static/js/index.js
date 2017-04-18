function carga() {

  $.get("info_sesiones", null, function(data, status, xhr){
  		var plot = $.plot("#morris-bar-chart", [data]);
  })
  .fail(function(response) {

  });
} // Cierre carga()

function obtener_datos() {

	$.getJSON("static/data/dash.json", function(d2){
		document.getElementById("n_sesiones").innerHTML = d2["numero"];
		document.getElementById("distancia").innerHTML = d2["distancia"];
		document.getElementById("tiempo").innerHTML = d2["tiempo"];
		document.getElementById("n_errores").innerHTML = d2["errores"];
    });
}