$(function() {
    var sensores = true
    trazada(sensores)
});

var graficas = []

$("#add_graph").click(function() {
var html = '<div class="col-lg-6"> \
                       <div class="panel panel-default"> \
                        <div class="panel-heading"> \
Grafica 1 \
                        </div> \
                        <!-- /.panel-heading --> \
                        <div class="panel-body"> \
			      <div class="flot-chart"> \
				  <div class="flot-chart-content-wide" id="grafica-1"></div> \
			      </div> \
			      <div id="choices" class="btn-group col-xs-12" data-toggle="buttons"></div> \
			      <div class="btn-group col-xs-12 latlon" data-toggle="buttons"></div> \
             </div> \
                        <!-- /.panel-body --> \
                      </div> \
                      <!-- /.panel --> \
                    </div> \
<!-- /.col-lg-6 -->';
    
    $("#panel_add_graph").before(html)
    datos = {ylabel:"aaaa",xlabel:"bbbb", choiceContainer : $("#choices"), plotContainer : $("#grafica-1"), ids : "AB", colsize : "col-xs-3", sensores : "ruedas-amortiguadores"}
    console.log(coord1)
    graficas.push(datos)
    vel_ruedas(coord1, coord2, "huehuehue", datos)
});
