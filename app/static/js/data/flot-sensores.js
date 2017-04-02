//Trazada
$(function() {
  var id = getCookie("id")

  var choiceContainer = $("#vueltas");
  plotAccordingToChoices();
  choiceContainer.find("input").change(plotAccordingToChoices);

  var plot
  var coord1 = null;
  var coord2 = null;

  function plotAccordingToChoices() {

    var key
    choiceContainer.find("input:checked").each(function () {
      key = $(this).attr("value");
    });

    console.log(key);
    var path = null;
    if (key == "trazada"){
      path = "static/data/sesiones/"+id+"/trazada.json";
    }
    else{
      path = "static/data/sesiones/"+id+"/vueltas/vuelta_"+key+".json";
    }

    $.getJSON(path, function(d2){
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
      // vel_ruedas(d2[0],d2[d2.length-1],"ruedas");
      // vel_ruedas(d2[0],d2[d2.length-1],"amortiguadores");
      // vel_ruedas(d2[0],d2[d2.length-1],"acelerador");
      // vel_ruedas(d2[0],d2[d2.length-1],"volante");
      plot = $.plot("#flot-line-chart", [d2], options);
    });
  }

  $("#flot-line-chart").bind("plotclick", function (event, pos, item) {
    if (item) {
      // $("#clickdata").text(" - click point " + item.datapoint);
      //plot.highlight(item.series, item.datapoint);
      console.log(item.series.data[item.dataIndex][2])
      if (coord1 == null){
        coord1 = item.datapoint;
        plot.highlight(item.series, item.datapoint);
      }
      else if (coord2 == null){
        coord2 = item.datapoint;
        plot.highlight(item.series, item.datapoint);
        // vel_ruedas(coord1, coord2, "ruedas")
        // vel_ruedas(coord1, coord2, "amortiguadores")
        // vel_ruedas(coord1, coord2, "acelerador")
        // vel_ruedas(coord1, coord2, "volante")
      }
      else {
        plot.unhighlight(item.series, coord1);
        plot.unhighlight(item.series, coord2);
        coord1 = item.datapoint;
        plot.highlight(item.series, coord1);
        coord2 = null;
      }
    }
    $("#clickdata").text("Inicio: " + coord1 + " Fin: " + coord2);
    console.log(coord1+" "+coord2);
  });

    $("#flot-line-chart").bind("plothover",function (event, pos, item) {
	if (item) {
	    $("#inst_data").show()
	    var x = item.datapoint[0].toFixed(2),
		y = item.datapoint[1].toFixed(2);
	    datos = item.series.data[item.dataIndex]
	    for (i=3; i < datos.length; i++) {
		var html = '';
		    console.log(datos[i]["data"])
		    $.each(datos[i]["data"], function(key, value){
			html += '<tr>';
			html += '<td>'+key+'</td>' ;
			html += '<td>'+value+'</td>' ;
			html += '</tr>';
		    });
		    console.log(datos[i]["label"])
		    switch (datos[i]["label"]){
		    case "ruedas":
			table_id = "VR-TR"
			break
		    case "amortiguadores":
			table_id = "CA-TR"
			break
		    case "acelerador":
			table_id = "AC-TR"
			break
		    case "volante":
			table_id = "GD-TR"
			break
		    }
		    $("#"+table_id).html(html)
		}
	    }
	else {
	    console.log("Ocultamos")
	    $("#inst_data").hide()
	}
    });
});
