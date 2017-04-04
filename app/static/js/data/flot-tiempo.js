$(function() {
$( ".sortable" ).sortable({
    handle : '.panel-heading'
    // items       : 'div:not(.unsortable)',
    // placeholder : 'sortable-placeholder'
});
$( ".sortable" ).disableSelection();

});

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
      vel_ruedas(d2[0],d2[d2.length-1],"ruedas");
      vel_ruedas(d2[0],d2[d2.length-1],"amortiguadores");
      vel_ruedas(d2[0],d2[d2.length-1],"acelerador");
      vel_ruedas(d2[0],d2[d2.length-1],"volante");
      plot = $.plot("#flot-trazada", [d2], options);
    });
  }

  $("#flot-trazada").bind("plotclick", function (event, pos, item) {
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
        vel_ruedas(coord1, coord2, "ruedas")
        vel_ruedas(coord1, coord2, "amortiguadores")
        vel_ruedas(coord1, coord2, "acelerador")
        vel_ruedas(coord1, coord2, "volante")
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

    $("#flot-trazada").bind("plothover",function (event, pos, item) {
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

//Velocidad de las ruedas
function vel_ruedas(coord1, coord2, sensor){

  var id = getCookie("id")
  var plot = {};
  switch (sensor){
    case "ruedas":
    // var path = "static/data/sesiones/"+id+"/ruedas.json";
    var ylabel = "rpm"
    var choiceContainer = $("#choices");
    var plotContainer = "#flot-rpm"
    var ids = "VR"
    var colsize = "col-xs-3"
    break;
    case "amortiguadores":
    // var path = "static/data/sesiones/"+id+"/amortiguador.json";
    var ylabel = "mm"
    var choiceContainer = $("#choices2");
    var plotContainer = "#flot-amortiguadores"
    var ids = "CA"
    var colsize = "col-xs-3"
    break;
    case "acelerador":
    // var path = "static/data/sesiones/"+id+"/amortiguador.json";
    var ylabel = "Grados"
    var choiceContainer = $("#choices3");
    var plotContainer = "#flot-acelerador"
    var ids = "AT"
    var colsize = "col-xs-3"
    break;
    case "volante":
    // var path = "static/data/sesiones/"+id+"/amortiguador.json";
    var ylabel = "Porcentaje"
    var choiceContainer = $("#choices4");
    var plotContainer = "#flot-direccion"
    var ids = "GD"
    var colsize = "col-xs-12"
    break;


  }
  // console.log(path)
  $.get("cortar_json", {lat1: coord1[0], lon1: coord1[1],lat2: coord2[0], lon2: coord2[1], id: id, sensor: sensor}, function(data, status, xhr){
    var datasets = data

    var i = 0;
    $.each(datasets, function(key, val) {
      val.color = i;
      ++i;
    });

    // insert checkboxes
    if (!$.trim(choiceContainer.html())){
      $.each(datasets, function(key, val) {
        choiceContainer.append("<label class='btn btn-primary active "+colsize+"' for='"+ids+"2-" + val.label + "'>" +"<input type='checkbox' name='" + key + "' checked='checked' id='"+ids+"2-" + val.label + "'></input>"+"<div id='"+ids+"-"+val.label+"'> "+val.label+" </div>"  + "</label>");
      });

      choiceContainer.next().append("<label class='btn btn-primary col-xs-6' style='visibility: hidden;'>" + "<div id='"+ids+"-lat'>Lat</div>"  + "</label>");
      choiceContainer.next().append("<label class='btn btn-primary col-xs-6' style='visibility: hidden;'>" + "<div id='"+ids+"-lon'>Lon</div>"  + "</label>");

    }
    choiceContainer.find(":input").change(plotAccordingToChoices);

    function plotAccordingToChoices() {

      var data = [];
      choiceContainer.find("input:not(:checked)").each(function () {
        $(this).parent().css("background-color", "")
      });

      choiceContainer.find("input:checked").each(function () {
        var key = $(this).attr("name");
        if (key && datasets[key]) {
          data.push(datasets[key]);
        }
      });

      if (data.length > 0) {
        plot[ids] = $.plot(plotContainer, data, {
          xaxis: {
            tickDecimals: 0
          },
          crosshair: {
            mode: "x"
          },
          grid : {
            hoverable : true,
            autoHighlight: false
          },
          legend: {
            labelFormatter: function(label, series) {
              // series is the series object for the label
              return null
            }
          },
          axisLabels: {
            show: true
          },
          xaxes: [{
            axisLabel: 'ms',
          }],
          yaxes: [{
            axisLabel: ylabel,
          }]

        });
      }


      var series = plot[ids].getData();
      console.log(series.length)
      for (var i = 0; i < series.length; ++i){
        $("#"+ids+"-"+series[i].label).parent().css("background-color",series[i].color)
      }

      var updateLegendTimeout = null;
      var latestPosition = null;

      function updateLegend() {
        updateLegendTimeout = null;
        var pos = latestPosition;

        var axes = plot[ids].getAxes();
        if (pos.x < axes.xaxis.min || pos.x > axes.xaxis.max ||
          pos.y < axes.yaxis.min || pos.y > axes.yaxis.max) {
            return;
          }

          var i, j, dataset = plot[ids].getData();
          for (i = 0; i < dataset.length; ++i) {

            var series = dataset[i];

            // Find the nearest points, x-wise

            for (j = 0; j < series.data.length; ++j) {
              if (series.data[j][0] > pos.x) {
                break;
              }
            }

            // Now Interpolate
            var y,
            p1 = series.data[j - 1],
            p2 = series.data[j];

            if (p1 == null) {
              y = p2[1];
            } else if (p2 == null) {
              y = p1[1];
            } else {0
              y = p1[1] + (p2[1] - p1[1]) * (pos.x - p1[0]) / (p2[0] - p1[0]);
            }
            $("#"+ids+"-lat").text("Lat: "+ (p1[2] + (p2[2] - p1[2]) * (pos.x - p1[0]) / (p2[0] - p1[0])).toFixed(2))
            $("#"+ids+"-lat").parent().css('visibility', 'visible');
            $("#"+ids+"-lon").text("Lon: "+(p1[3] + (p2[3] - p1[3]) * (pos.x - p1[0]) / (p2[0] - p1[0])).toFixed(2))
            $("#"+ids+"-lon").parent().css('visibility', 'visible');
            $("#"+ids+"-"+series.label).text(series.label+" = "+y.toFixed(2))
          }
        }

        $(plotContainer).bind("plothover",  function (event, pos, item) {
          latestPosition = pos;
          choiceContainer.find("div").each(function() {
            console.log("Entroooo")
            var id = this.id;
            // console.log(id.split("-")[0]);
            ids = id.split("-")[0]
            // console.log(ids)
          });
          if (!updateLegendTimeout) {
            updateLegendTimeout = setTimeout(updateLegend, 50);
          }
        });
      }

      plotAccordingToChoices();

      $(".flot-chart-content-wide").mouseleave(function(){
        choiceContainer.find("div").each(function() {
          var id = this.id;
          $("#"+id).text(id.split("-")[1])
        });

        choiceContainer.next().find("div").each(function() {
          console.log("Saaaalgo")
          var id = this.id;
          if (id == ""+ids+"-lat" || id == ""+ids+"-lon"){
            $("#"+id).parent().css('visibility', 'hidden')
          }
        });
      });
      

    });
  }

  //Fuerzas G
  $(function() {
    var d2 = [[-2.5, -2.5], [2.5, 2.5], [-2.5, 0], [2.5, 0],[0, 2.5],[0, -2.5],[2.5, -2.5],[-2.5, 2.5]];
    var options = {
      lines : {show : false},
      points : {show : true},
      grid: { show:true,
        markings: [
          { xaxis: { from: 0, to: 0 }, color:"red" },
          { yaxis: { from: 0, to: 0 }, color:"red" }
        ]
      }

    }
    $.plot("#flot-G", [d2], options);
  });
