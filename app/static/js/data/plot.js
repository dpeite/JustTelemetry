//Trazada
var coord1 = null;
var coord2 = null;
var id = getCookie("id")

function trazada(sensores = false, dist = false) {

  var choiceContainer = $("#vueltas");
  plotAccordingToChoices();
  choiceContainer.find("input").change(plotAccordingToChoices);

  var plot

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
      coord1 = d2[0]
      coord2 = d2[d2.length-1]
      if (!sensores){
        vel_ruedas(d2[0],d2[d2.length-1],"ruedas");
        vel_ruedas(d2[0],d2[d2.length-1],"amortiguadores");
        vel_ruedas(d2[0],d2[d2.length-1],"acelerador");
        vel_ruedas(d2[0],d2[d2.length-1],"volante");
        vel_ruedas(d2[0],d2[d2.length-1],"fuerzas");
      }
      else {
        if (graficas.length != 0){
          for (i = 0; i < graficas.length; i++){
            if (graficas[i] != null){
              vel_ruedas(coord1, coord2, "aaa", graficas[i])
            }
          }
        }
      }
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
        if (!sensores){
          vel_ruedas(coord1, coord2, "ruedas")
          vel_ruedas(coord1, coord2, "amortiguadores")
          vel_ruedas(coord1, coord2, "acelerador")
          vel_ruedas(coord1, coord2, "volante")
          vel_ruedas(coord1, coord2, "fuerzas")
        }
        else {
          if (graficas.length != 0){
            for (i = 0; i < graficas.length; i++){
              if (graficas[i] != null){
                vel_ruedas(coord1, coord2, "aaa", graficas[i])
              }
            }
          }
        }
      }
      else {
        plot.unhighlight(item.series, coord1);
        plot.unhighlight(item.series, coord2);
        coord1 = item.datapoint;
        plot.highlight(item.series, coord1);
        coord2 = null;
      }
    }
    // $("#clickdata").text("Inicio: " + coord1 + " Fin: " + coord2);
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
}

//Velocidad de las ruedas
function vel_ruedas(coord1, coord2, sensor, sensores){

  var id = getCookie("id")
  var plot = {};
  switch (sensor){
    case "ruedas":
    var ylabel = "rpm"
    var choiceContainer = $("#choices");
    var plotContainer = "#flot-rpm"
    var ids = "VR"
    var colsize = "col-xs-3"
    break;
    case "amortiguadores":
    var ylabel = "mm"
    var choiceContainer = $("#choices2");
    var plotContainer = "#flot-amortiguadores"
    var ids = "CA"
    var colsize = "col-xs-3"
    break;
    case "acelerador":
    var ylabel = "Porcentaje"
    var choiceContainer = $("#choices3");
    var plotContainer = "#flot-acelerador"
    var ids = "AT"
    var colsize = "col-xs-3"
    break;
    case "volante":
    var ylabel = "Grados"
    var choiceContainer = $("#choices4");
    var plotContainer = "#flot-direccion"
    var ids = "GD"
    var colsize = "col-xs-12"
    break;
    case "fuerzas":
    var ylabel = "Porcentaje"
    var choiceContainer = $("#choices5")
    var plotContainer = "#flot-g"
    var ids = "FG"
    var colsize = "col-xs-12"
    break;

    default:
    var xlabel = sensores.xlabel
    var ylabel = sensores.ylabel
    var choiceContainer = sensores.choiceContainer
    var plotContainer = sensores.plotContainer
    var ids = sensores.ids
    var colsize = sensores.colsize
    sensor = sensores.sensores
    var rel_sensores = true
    console.log(sensores);
    console.log(coord1)
    break;


  }
  // console.log(path)
    $.get("cortar_json", {lat1: coord1[0], lon1: coord1[1],lat2: coord2[0], lon2: coord2[1], id: id, sensor: sensor, dist: dist}, function(data, status, xhr){
    var datasets = data

    var i = 0;
    $.each(datasets, function(key, val) {
      val.color = i;
      ++i;
    });

    // insert checkboxes
    if (!$.trim(choiceContainer.html())){
	$.each(datasets, function(key, val) {
	    if (rel_sensores){
		choiceContainer.append("<label style='visibility: hidden; class='btn btn-primary active "+colsize+"' for='"+ids+"2-" + val.label + "'>" +"<input type='checkbox' name='" + key + "' checked='checked' id='"+ids+"2-" + val.label + "'></input>"+"<div id='"+ids+"-"+val.label+"'> "+val.label+" </div>"  + "</label>");
	    }
	    else {
        choiceContainer.append("<label class='btn btn-primary active "+colsize+"' for='"+ids+"2-" + val.label + "'>" +"<input type='checkbox' name='" + key + "' checked='checked' id='"+ids+"2-" + val.label + "'></input>"+"<div id='"+ids+"-"+val.label+"'> "+val.label+" </div>"  + "</label>");
	    }
      });
      choiceContainer.next().append("<label class='btn btn-primary col-xs-6' style='visibility: hidden;'>" + "<div id='"+ids+"-lat'>Lat</div>"  + "</label>");
      choiceContainer.next().append("<label class='btn btn-primary col-xs-6' style='visibility: hidden;'>" + "<div id='"+ids+"-lon'>Lon</div>"  + "</label>");

    }

    if (sensor == "fuerzas"){
      fuerzas_g(datasets)
      return 0
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
	   var options = {
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
              // axisLabel: 'ms',
	      tickFormatter : function suffixFormatter(val, axis) {
		  if (dist == false){
		      return (val / 1000).toFixed(axis.tickDecimals) + " s";
		  }
		  else{
		      return val+ " m";
		  }
	      }
          }],
          yaxes: [{
            axisLabel: ylabel,
          }]
	   }
	    if (sensores){
		delete options["xaxes"][0]["tickFormatter"]
		options["xaxes"][0]["axisLabel"] = xlabel
		options["crosshair"]["mode"] = "xy"
	    }
        plot[ids] = $.plot(plotContainer, data, options);
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
            // j = 1;
            // Now Interpolate
            var y,
            p1 = series.data[j - 1],
            p2 = series.data[j];
            try {
              interpolate(y, p1, p2, ids, pos, series)
            }
            catch (err){
              console.log(err)
            }
            // if (p1 == null) {
            //   y = p2[1];
            // } else if (p2 == null) {
            //   y = p1[1];
            // } else {0
            //   y = p1[1] + (p2[1] - p1[1]) * (pos.x - p1[0]) / (p2[0] - p1[0]);
            // }
            // $("#"+ids+"-lat").text("Lat: "+ (p1[2] + (p2[2] - p1[2]) * (pos.x - p1[0]) / (p2[0] - p1[0])).toFixed(2))
            // $("#"+ids+"-lat").parent().css('visibility', 'visible');
            // $("#"+ids+"-lon").text("Lon: "+(p1[3] + (p2[3] - p1[3]) * (pos.x - p1[0]) / (p2[0] - p1[0])).toFixed(2))
            // $("#"+ids+"-lon").parent().css('visibility', 'visible');
            // $("#"+ids+"-"+series.label).text(series.label+" = "+y.toFixed(2))
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


    }).fail(function(response) {
        console.log("*/-a/fsd-*as/f*-a/sd-*/f-")
	console.log(ids)
	console.log($('#ids'))
	$('#'+ids).hide();
      });
  }

  // No variar el orden
  $("#siglas-acelerador").popover({ trigger: "hover", placement : "left", html : "true", content : ' \
  <div class="col-lg-12"> \
  1: Potenciometro lineal 1 </br> \
  2: Potenciometro lineal 2 </br> \
  3: Potenciometro lineal 3 </br> \
  4: Potenciometro lineal 4 </br> \
  </div> \
  ' });

  $(".siglas").popover({ trigger: "hover", placement : "left", html : "true", content : ' \
  <div class="col-lg-12"> \
  DD: Delantero Derecho </br> \
  DI: Delantero Izquierdo </br> \
  TD: Trasero Derecho </br> \
  TI: Trasero Izquierdo </br> \
  </div> \
  ' });
