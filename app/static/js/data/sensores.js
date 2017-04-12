$(function() {
    var sensores = true
    trazada(sensores)
});

var graficas = []

$("#add_graph").click(function() {
    var sensor1 = $('#sensor1').find(":selected").text();
    var sensor2 = $('#sensor2').find(":selected").text();

var html = '<div class="col-lg-6"> \
                       <div class="panel panel-default"> \
                        <div class="panel-heading"> \
                        Grafica '+(graficas.length+1)+' \
                        </div> \
                        <!-- /.panel-heading --> \
                        <div class="panel-body"> \
			      <div class="flot-chart"> \
				  <div class="flot-chart-content-wide" id="grafica-'+(graficas.length+1)+'"></div> \
			      </div> \
			      <div id="choices-'+(graficas.length+1)+'" class="btn-group col-xs-12" data-toggle="buttons"></div> \
			      <div class="btn-group col-xs-12 latlon" data-toggle="buttons"></div> \
             </div> \
                        <!-- /.panel-body --> \
                      </div> \
                      <!-- /.panel --> \
                    </div> \
<!-- /.col-lg-6 -->';
    
    $("#panel_add_graph").before(html)
    datos = {ylabel:sensor2,xlabel:sensor1, choiceContainer : $("#choices-"+(graficas.length+1)), plotContainer : $("#grafica-"+(graficas.length+1)), ids : "sensores"+(graficas.length+1), colsize : "col-xs-3", sensores : sensor1+"-"+sensor2}
    graficas.push(datos)
    vel_ruedas(coord1, coord2, "sensores", datos)
});
