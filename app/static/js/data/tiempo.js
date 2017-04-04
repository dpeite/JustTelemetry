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

