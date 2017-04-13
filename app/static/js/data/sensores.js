$(function() {
    var sensores = true
    trazada(sensores)
});

var graficas = []

$("#add_graph").click(function() {
    var sensor1 = $('#sensor1').find(":selected").text();
    var sensor2 = $('#sensor2').find(":selected").text();

    var html = '<div id="panel-'+(graficas.length+1)+'" class="col-lg-6"> \
                       <div class="panel panel-default"> \
                        <div class="panel-heading"> \
                        Grafica '+(graficas.length+1)+' \
                        <button type="button" class="close">×</button> \
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
    console.log(graficas)
    vel_ruedas(coord1, coord2, "sensores", datos)
    RefreshSomeEventListener();
});

function RefreshSomeEventListener() {
    $(".close").off();
    
    $(".close").on("click", function() {
	console.log("cierro")
	$(this).parent().parent().parent().remove()
	console.log(graficas.length)
	console.log($(this).parent().parent().parent().attr("id").split("-")[1]-1)
	// graficas.splice($(this).parent().parent().parent().attr("id").split("-")[1]-1, 1)
	delete graficas[$(this).parent().parent().parent().attr("id").split("-")[1]-1]
	console.log(graficas.length)
	console.log($(this).parent().parent().parent())
});
}

function interpolate(y, p1, p2, ids, pos, series){
    if (p1 == null) {
	console.log(p1)
	console.log(p2)
        y = p2[1];
    } else if (p2 == null) {
        y = p1[1];
    } else {
            y = p1[1] + (p2[1] - p1[1]) * (pos.x - p1[0]) / (p2[0] - p1[0]);
           }
    // $("#"+ids+"-lat").text("Lat: "+ (p1[3] + (p2[3] - p1[3]) * (pos.x - p1[0]) / (p2[0] - p1[0])).toFixed(2))
    // $("#"+ids+"-lat").parent().css('visibility', 'visible');
    // $("#"+ids+"-lon").text("Lon: "+(p1[4] + (p2[4] - p1[4]) * (pos.x - p1[0]) / (p2[0] - p1[0])).toFixed(2))
    // $("#"+ids+"-lon").parent().css('visibility', 'visible');
    $("#"+ids+"-"+series.label).text(series.label+" = "+y.toFixed(2))
    
}
