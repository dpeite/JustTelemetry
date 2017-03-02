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
