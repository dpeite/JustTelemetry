$(function() {

    path = "static/data/dash.json"

    $.getJSON(path, function(d2){
        Morris.Bar({
            element: 'morris-bar-chart',
            data: d2["sesiones"],
            xkey: 'fecha',
            ykeys: ['numero'],
            hideHover: 'auto',
            resize: true,
            hover: false
        });
    }); // Cierre getJSON
}); // Cierre (function())
