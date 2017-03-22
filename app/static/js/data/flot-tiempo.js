//Trazada
$(function() {
  // var d2 = [[42.224581, -8.732471], [42.224325, -8.731725], [42.224096, -8.731065], [42.223774, -8.731172], [42.223415, -8.731054], [42.223107, -8.730708], [42.222640, -8.731089], [42.223028, -8.7316], [42.224581, -8.732471]];

  var id = getCookie("id")

  var choiceContainer = $("#vueltas");
  plotAccordingToChoices();
  choiceContainer.find("input").click(plotAccordingToChoices);

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
    // $.getJSON("static/data/sesiones/"+id+"/trazada.json", function(d2){
    // $.getJSON("static/data/sesiones/"+id+"/vueltas/vuelta_"+key+".json", function(d2){
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
    

});

//Velocidad de las ruedas
$(document).ready(function() {

  // var datasets = {
  //   "usa": {
  //     label: "Del Derecha",
  //     data: [[1988, 483994], [1989, 479060], [1990, 457648], [1991, 401949], [1992, 424705], [1993, 402375], [1994, 377867], [1995, 357382], [1996, 337946], [1997, 336185], [1998, 328611], [1999, 329421], [2000, 342172], [2001, 344932], [2002, 387303], [2003, 440813], [2004, 480451], [2005, 504638], [2006, 528692]]
  //   },
  //   "russia": {
  //     label: "Del Izquierda",
  //     data: [[1988, 218000], [1989, 203000], [1990, 171000], [1992, 42500], [1993, 37600], [1994, 36600], [1995, 21700], [1996, 19200], [1997, 21300], [1998, 13600], [1999, 14000], [2000, 19100], [2001, 21300], [2002, 23600], [2003, 25100], [2004, 26100], [2005, 31100], [2006, 34700]]
  //   },
  //   "uk": {
  //     label: "Tras Derecha",
  //     data: [[1988, 62982], [1989, 62027], [1990, 60696], [1991, 62348], [1992, 58560], [1993, 56393], [1994, 54579], [1995, 50818], [1996, 50554], [1997, 48276], [1998, 47691], [1999, 47529], [2000, 47778], [2001, 48760], [2002, 50949], [2003, 57452], [2004, 60234], [2005, 60076], [2006, 59213]]
  //   },
  //   "germany": {
  //     label: "Tras Izquierda",
  //     data: [[1988, 55627], [1989, 55475], [1990, 58464], [1991, 55134], [1992, 52436], [1993, 47139], [1994, 43962], [1995, 43238], [1996, 42395], [1997, 40854], [1998, 40993], [1999, 41822], [2000, 41147], [2001, 40474], [2002, 40604], [2003, 40044], [2004, 38816], [2005, 38060], [2006, 36984]]
  //   },
  // };

  // hard-code color indices to prevent them from shifting as
  // countries are turned on/off

    var id = getCookie("id")
    path = "static/data/sesiones/"+id+"/ruedas.json";
    $.getJSON(path, function(datasets){
	console.log("siiiiii")
    
    
  var i = 0;
  $.each(datasets, function(key, val) {
    val.color = i;
    ++i;
  });

  // insert checkboxes
  var choiceContainer = $("#choices");
  $.each(datasets, function(key, val) {
    choiceContainer.append("<br/><input type='checkbox' name='" + key +
    "' checked='checked' id='id" + key + "'></input>" +
    "<label for='id" + key + "'>"
    + val.label + "</label>");
  });

  choiceContainer.find("input").click(plotAccordingToChoices);

  function plotAccordingToChoices() {

    var data = [];

    choiceContainer.find("input:checked").each(function () {
      var key = $(this).attr("name");
      if (key && datasets[key]) {
        data.push(datasets[key]);
      }
    });

    if (data.length > 0) {
      $.plot("#flot-pie-chart", data, {
        yaxis: {
          min: 0
        },
        xaxis: {
          tickDecimals: 0
        }
      });
    }
  }

  plotAccordingToChoices();

});
});

//Compresion del Amortiguador
$(document).ready(function() {

  var datasets = {
    "usa": {
      label: "Del Derecho",
      data: [[1988, 483994], [1989, 479060], [1990, 457648], [1991, 401949], [1992, 424705], [1993, 402375], [1994, 377867], [1995, 357382], [1996, 337946], [1997, 336185], [1998, 328611], [1999, 329421], [2000, 342172], [2001, 344932], [2002, 387303], [2003, 440813], [2004, 480451], [2005, 504638], [2006, 528692]]
    },
    "russia": {
      label: "Del Izquierdo",
      data: [[1988, 218000], [1989, 203000], [1990, 171000], [1992, 42500], [1993, 37600], [1994, 36600], [1995, 21700], [1996, 19200], [1997, 21300], [1998, 13600], [1999, 14000], [2000, 19100], [2001, 21300], [2002, 23600], [2003, 25100], [2004, 26100], [2005, 31100], [2006, 34700]]
    },
    "uk": {
      label: "Tras Derecho",
      data: [[1988, 62982], [1989, 62027], [1990, 60696], [1991, 62348], [1992, 58560], [1993, 56393], [1994, 54579], [1995, 50818], [1996, 50554], [1997, 48276], [1998, 47691], [1999, 47529], [2000, 47778], [2001, 48760], [2002, 50949], [2003, 57452], [2004, 60234], [2005, 60076], [2006, 59213]]
    },
    "germany": {
      label: "Tras Izquierdo",
      data: [[1988, 55627], [1989, 55475], [1990, 58464], [1991, 55134], [1992, 52436], [1993, 47139], [1994, 43962], [1995, 43238], [1996, 42395], [1997, 40854], [1998, 40993], [1999, 41822], [2000, 41147], [2001, 40474], [2002, 40604], [2003, 40044], [2004, 38816], [2005, 38060], [2006, 36984]]
    },
  };

  // hard-code color indices to prevent them from shifting as
  // countries are turned on/off

  var i = 0;
  $.each(datasets, function(key, val) {
    val.color = i;
    ++i;
  });

  // insert checkboxes
  var choiceContainer = $("#choices2");
  $.each(datasets, function(key, val) {
    choiceContainer.append("<br/><input type='checkbox' name='" + key +
    "' checked='checked' id='id" + key + "'></input>" +
    "<label for='id" + key + "'>"
    + val.label + "</label>");
  });

  choiceContainer.find("input").click(plotAccordingToChoices);

  function plotAccordingToChoices() {

    var data = [];

    choiceContainer.find("input:checked").each(function () {
      var key = $(this).attr("name");
      if (key && datasets[key]) {
        data.push(datasets[key]);
      }
    });

    if (data.length > 0) {
      $.plot("#flot-line-chart-multi", data, {
        yaxis: {
          min: 0
        },
        xaxis: {
          tickDecimals: 0
        }
      });
    }
  }

  plotAccordingToChoices();


});


//Giro de la direccion
$(function() {
  // var d1 = [];
  // for (var i = 0; i < 14; i += 0.5) {
  // 	d1.push([i, ((Math.tan(3.141596)*Math.tan(3.141596))-Math.sin(3.141596*(i/2)*Math.cos(i*i)))+89]);
  // }
  var id = getCookie("id")
  path = "static/data/sesiones/"+id+"/volante.json";
    $.getJSON(path, function(d2){
    console.log("siiiiii")
    $.plot("#flot-line-chart-moving", [{data:d2.direccion}]);
  });
});

//Porcentaje del acelerador
$(document).ready(function() {

  var datasets = {
    "usa": {
      label: "1",
      data: [[1988, 483994], [1989, 479060], [1990, 457648], [1991, 401949], [1992, 424705], [1993, 402375], [1994, 377867], [1995, 357382], [1996, 337946], [1997, 336185], [1998, 328611], [1999, 329421], [2000, 342172], [2001, 344932], [2002, 387303], [2003, 440813], [2004, 480451], [2005, 504638], [2006, 528692]]
    },
    "russia": {
      label: "2",
      data: [[1988, 218000], [1989, 203000], [1990, 171000], [1992, 42500], [1993, 37600], [1994, 36600], [1995, 21700], [1996, 19200], [1997, 21300], [1998, 13600], [1999, 14000], [2000, 19100], [2001, 21300], [2002, 23600], [2003, 25100], [2004, 26100], [2005, 31100], [2006, 34700]]
    },
    "uk": {
      label: "3",
      data: [[1988, 62982], [1989, 62027], [1990, 60696], [1991, 62348], [1992, 58560], [1993, 56393], [1994, 54579], [1995, 50818], [1996, 50554], [1997, 48276], [1998, 47691], [1999, 47529], [2000, 47778], [2001, 48760], [2002, 50949], [2003, 57452], [2004, 60234], [2005, 60076], [2006, 59213]]
    },
    "germany": {
      label: "4",
      data: [[1988, 55627], [1989, 55475], [1990, 58464], [1991, 55134], [1992, 52436], [1993, 47139], [1994, 43962], [1995, 43238], [1996, 42395], [1997, 40854], [1998, 40993], [1999, 41822], [2000, 41147], [2001, 40474], [2002, 40604], [2003, 40044], [2004, 38816], [2005, 38060], [2006, 36984]]
    },
  };

  // hard-code color indices to prevent them from shifting as
  // countries are turned on/off

  var i = 0;
  $.each(datasets, function(key, val) {
    val.color = i;
    ++i;
  });

  // insert checkboxes
  var choiceContainer = $("#choices3");
  $.each(datasets, function(key, val) {
    choiceContainer.append("<br/><input type='checkbox' name='" + key +
    "' checked='checked' id='id" + key + "'></input>" +
    "<label for='id" + key + "'>"
    + val.label + "</label>");
  });

  choiceContainer.find("input").click(plotAccordingToChoices);

  function plotAccordingToChoices() {

    var data = [];

    choiceContainer.find("input:checked").each(function () {
      var key = $(this).attr("name");
      if (key && datasets[key]) {
        data.push(datasets[key]);
      }
    });

    if (data.length > 0) {
      $.plot("#flot-bar-chart", data, {
        yaxis: {
          min: 0
        },
        xaxis: {
          tickDecimals: 0
        }
      });
    }
  }

  plotAccordingToChoices();


});


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
  $.plot("#flot-bar-chart2", [d2], options);
});

// //Flot Line Chart
// $(document).ready(function() {

//     var offset = 0;
//     plot();

//     function plot() {
//         var sin = [],
//             cos = [];
//         for (var i = 0; i < 12; i += 0.2) {
//             sin.push([i, Math.sin(i + offset)]);
//             cos.push([i, Math.cos(i + offset)]);
//         }

//         var options = {
//             series: {
//                 lines: {
//                     show: true
//                 },
//                 points: {
//                     show: true
//                 }
//             },
//             grid: {
//                 hoverable: true //IMPORTANT! this is needed for tooltip to work
//             },
//             yaxis: {
//                 min: -1.2,
//                 max: 1.2
//             },
//             tooltip: true,
//             tooltipOpts: {
//                 content: "'%s' of %x.1 is %y.4",
//                 shifts: {
//                     x: -60,
//                     y: 25
//                 }
//             }
//         };

//         var plotObj = $.plot($("#flot-line-chart"), [{
//                 data: sin,
//                 label: "sin(x)"
//             }, {
//                 data: cos,
//                 label: "cos(x)"
//             }],
//             options);
//     }
// });

// //Flot Pie Chart
// $(function() {

//     var data = [{
//         label: "Series 0",
//         data: 1
//     }, {
//         label: "Series 1",
//         data: 3
//     }, {
//         label: "Series 2",
//         data: 9
//     }, {
//         label: "Series 3",
//         data: 20
//     }];

//     var plotObj = $.plot($("#flot-pie-chart"), data, {
//         series: {
//             pie: {
//                 show: true
//             }
//         },
//         grid: {
//             hoverable: true
//         },
//         tooltip: true,
//         tooltipOpts: {
//             content: "%p.0%, %s", // show percentages, rounding to 2 decimal places
//             shifts: {
//                 x: 20,
//                 y: 0
//             },
//             defaultTheme: false
//         }
//     });

// });

// //Flot Multiple Axes Line Chart
// $(function() {
//     var oilprices = [
//         [1167692400000, 61.05],
//         [1167778800000, 58.32],
//         [1213308000000, 0.64670],
//         [1213394400000, 0.65060],
//         [1213480800000, 0.65040],
//         [1213567200000, 0.65030],
//         [1213653600000, 0.64810],
//         [1213740000000, 0.64510],
//         [1213826400000, 0.6450],
//         [1213912800000, 0.64410],
//         [1213999200000, 0.64140],
//         [1214085600000, 0.64090],
//         [1214172000000, 0.64090],
//         [1214258400000, 0.64280],
//         [1214344800000, 0.64310],
//         [1214431200000, 0.64180],
//         [1214517600000, 0.63710],
//         [1214604000000, 0.63490],
//         [1214690400000, 0.63330],
//         [1214776800000, 0.63340],
//         [1214863200000, 0.63380],
//         [1214949600000, 0.63420],
//         [1215036000000, 0.6320],
//         [1215122400000, 0.63180],
//         [1215208800000, 0.6370],
//         [1215295200000, 0.63680],
//         [1215381600000, 0.63680],
//         [1215468000000, 0.63830],
//         [1215554400000, 0.63710],
//         [1215640800000, 0.63710],
//         [1215727200000, 0.63550],
//         [1215813600000, 0.6320],
//         [1215900000000, 0.62770],
//         [1215986400000, 0.62760],
//         [1216072800000, 0.62910],
//         [1216159200000, 0.62740],
//         [1216245600000, 0.62930],
//         [1216332000000, 0.63110],
//         [1216418400000, 0.6310],
//         [1216504800000, 0.63120],
//         [1216591200000, 0.63120],
//         [1216677600000, 0.63040],
//         [1216764000000, 0.62940],
//         [1216850400000, 0.63480],
//         [1216936800000, 0.63780],
//         [1217023200000, 0.63680],
//         [1217109600000, 0.63680],
//         [1217196000000, 0.63680],
//         [1217282400000, 0.6360],
//         [1217368800000, 0.6370],
//         [1217455200000, 0.64180],
//         [1217541600000, 0.64110],
//         [1217628000000, 0.64350],
//         [1217714400000, 0.64270],
//         [1217800800000, 0.64270],
//         [1217887200000, 0.64190],
//         [1217973600000, 0.64460],
//         [1218060000000, 0.64680],
//         [1218146400000, 0.64870],
//         [1218232800000, 0.65940],
//         [1218319200000, 0.66660],
//         [1218405600000, 0.66660],
//         [1218492000000, 0.66780],
//         [1218578400000, 0.67120],
//         [1218664800000, 0.67050],
//         [1218751200000, 0.67180],
//         [1218837600000, 0.67840],
//         [1218924000000, 0.68110],
//         [1219010400000, 0.68110],
//         [1219096800000, 0.67940],
//         [1219183200000, 0.68040],
//         [1219269600000, 0.67810],
//         [1219356000000, 0.67560],
//         [1219442400000, 0.67350],
//         [1219528800000, 0.67630],
//         [1219615200000, 0.67620],
//         [1219701600000, 0.67770],
//         [1219788000000, 0.68150],
//         [1219874400000, 0.68020],
//         [1219960800000, 0.6780],
//         [1220047200000, 0.67960],
//         [1220133600000, 0.68170],
//         [1220220000000, 0.68170],
//         [1220306400000, 0.68320],
//         [1220392800000, 0.68770],
//         [1220479200000, 0.69120],
//         [1220565600000, 0.69140],
//         [1220652000000, 0.70090],
//         [1220738400000, 0.70120],
//         [1220824800000, 0.7010],
//         [1220911200000, 0.70050]
//     ];

//     function euroFormatter(v, axis) {
//         return v.toFixed(axis.tickDecimals) + "€";
//     }

//     function doPlot(position) {
//         $.plot($("#flot-line-chart-multi"), [{
//             data: oilprices,
//             label: "Oil price ($)"
//         }, {
//             data: exchangerates,
//             label: "USD/EUR exchange rate",
//             yaxis: 2
//         }], {
//             xaxes: [{
//                 mode: 'time'
//             }],
//             yaxes: [{
//                 min: 0
//             }, {
//                 // align if we are to the right
//                 alignTicksWithAxis: position == "right" ? 1 : null,
//                 position: position,
//                 tickFormatter: euroFormatter
//             }],
//             legend: {
//                 position: 'sw'
//             },
//             grid: {
//                 hoverable: true //IMPORTANT! this is needed for tooltip to work
//             },
//             tooltip: true,
//             tooltipOpts: {
//                 content: "%s for %x was %y",
//                 xDateFormat: "%y-%0m-%0d",

//                 onHover: function(flotItem, $tooltipEl) {
//                     // console.log(flotItem, $tooltipEl);
//                 }
//             }

//         });
//     }

//     doPlot("right");

//     $("button").click(function() {
//         doPlot($(this).text());
//     });
// });

// //Flot Moving Line Chart

// $(function() {

//     var container = $("#flot-line-chart-moving");

//     // Determine how many data points to keep based on the placeholder's initial size;
//     // this gives us a nice high-res plot while avoiding more than one point per pixel.

//     var maximum = container.outerWidth() / 2 || 300;

//     //

//     var data = [];

//     function getRandomData() {

//         if (data.length) {
//             data = data.slice(1);
//         }

//         while (data.length < maximum) {
//             var previous = data.length ? data[data.length - 1] : 50;
//             var y = previous + Math.random() * 10 - 5;
//             data.push(y < 0 ? 0 : y > 100 ? 100 : y);
//         }

//         // zip the generated y values with the x values

//         var res = [];
//         for (var i = 0; i < data.length; ++i) {
//             res.push([i, data[i]])
//         }

//         return res;
//     }

//     //

//     series = [{
//         data: getRandomData(),
//         lines: {
//             fill: true
//         }
//     }];

//     //

//     var plot = $.plot(container, series, {
//         grid: {
//             borderWidth: 1,
//             minBorderMargin: 20,
//             labelMargin: 10,
//             backgroundColor: {
//                 colors: ["#fff", "#e4f4f4"]
//             },
//             margin: {
//                 top: 8,
//                 bottom: 20,
//                 left: 20
//             },
//             markings: function(axes) {
//                 var markings = [];
//                 var xaxis = axes.xaxis;
//                 for (var x = Math.floor(xaxis.min); x < xaxis.max; x += xaxis.tickSize * 2) {
//                     markings.push({
//                         xaxis: {
//                             from: x,
//                             to: x + xaxis.tickSize
//                         },
//                         color: "rgba(232, 232, 255, 0.2)"
//                     });
//                 }
//                 return markings;
//             }
//         },
//         xaxis: {
//             tickFormatter: function() {
//                 return "";
//             }
//         },
//         yaxis: {
//             min: 0,
//             max: 110
//         },
//         legend: {
//             show: true
//         }
//     });

//     // Update the random dataset at 25FPS for a smoothly-animating chart

//     setInterval(function updateRandom() {
//         series[0].data = getRandomData();
//         plot.setData(series);
//         plot.draw();
//     }, 40);

// });

// //Flot Bar Chart

// $(function() {

//     var barOptions = {
//         series: {
//             bars: {
//                 show: true,
//                 barWidth: 43200000
//             }
//         },
//         xaxis: {
//             mode: "time",
//             timeformat: "%m/%d",
//             minTickSize: [1, "day"]
//         },
//         grid: {
//             hoverable: true
//         },
//         legend: {
//             show: false
//         },
//         tooltip: true,
//         tooltipOpts: {
//             content: "x: %x, y: %y"
//         }
//     };
//     var barData = {
//         label: "bar",
//         data: [
//             [1354521600000, 1000],
//             [1355040000000, 2000],
//             [1355223600000, 3000],
//             [1355306400000, 4000],
//             [1355487300000, 5000],
//             [1355571900000, 6000]
//         ]
//     };
//     $.plot($("#flot-bar-chart"), [barData], barOptions);

// });
