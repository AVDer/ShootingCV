$("#targetImage").hide();

$("#orgImageButton").click(() => {
    $("#targetCanvas").hide();
    $("#targetImage").show();
    $("#targetImage").attr("src", "http://localhost:8001/original.png");
}) 

$("#blurImageButton").click(() => {
    $("#targetCanvas").hide();
    $("#targetImage").show();
    $("#targetImage").attr("src", "http://localhost:8001/blurry.png");
})

$("#arImageButton").click(() => {
    $("#targetCanvas").hide();
    $("#targetImage").show();
    $("#targetImage").attr("src", "http://localhost:8001/augmented.png");
})

$("#vrImageButton").click(() => {
    $("#targetCanvas").show();
    $("#targetImage").hide();
})