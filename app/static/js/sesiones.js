$('.vueltas').click(function(){
    value = $(event.target).closest('tr').find('[name=optionsRadios]');

    code = '<div class="flot-chart"> \
<div class="flot-chart-content" id="vueltas"></div> \
</div> \
</div> \
<div class="modal-footer"> \
<button type="button" class="btn btn-default" data-dismiss="modal">Cerrar</button> \
<button type="button" class="btn btn-primary">Guardar</button> \
</div>';
    $("#modalcontent").empty();
    $("#modalcontent").append(code);

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
	var coord = null;
	
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
	    
	    
	}
	console.log(coord);
    });
    });
    $('#myModal1').modal('show');


    
});


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
