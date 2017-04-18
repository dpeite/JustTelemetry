$(function() {
  trazada()
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
  $.plot("#flot-G", [d2], options);
});

function interpolate(y, p1, p2, ids, pos, series){
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
