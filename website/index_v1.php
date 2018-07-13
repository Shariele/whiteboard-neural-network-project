<!DOCTYPE html>
<html>
<head>
	<title>Whiteboard</title>
	<?php include "include/necessities.php"; ?>
</head>
<body style="background-color: grey;">
	<canvas style="margin:0;padding:0;position:relative;left:600px;top:250px;" id="canvasArea" width="500" height="500"></canvas>
</body>
</html>

<script type="text/javascript">

	$(document).ready(function(){
		var canvas = document.getElementById("canvasArea");
		var ctx = canvas.getContext("2d");

		var imgData = ctx.createImageData(500,500);

		for (var i = 0; i < imgData.data.length; i += 4){
			imgData.data[i+0]=255;
			imgData.data[i+1]=255;
			imgData.data[i+2]=255;
			imgData.data[i+3]=255;
		}
		ctx.putImageData(imgData,10,10);

		canvas.addEventListener('mousedown', function(){
			this.addEventListener('mousemove', draw);
		});

		canvas.addEventListener('mouseup', function(){
			this.removeEventListener('mousemove', draw);
			console.log(imgData);
		});

		function draw(e) {
		    var pos = getMousePos(canvas, e);
		    posx = pos.x;
		    posy = pos.y;
		    var pixel = ctx.getImageData(posx, posy, 1, 1);
		    console.log(pixel);
		    pixel.data[0] = 0;
		    pixel.data[1] = 0;
		    pixel.data[2] = 0;

		    ctx.putImageData(pixel, posx, posy);


		    ctx.fillStyle = 0;
		    ctx.fillRect(posx-2, posy-2, 5, 5);
		}

		function getMousePos(canvas, evt) {
		    var rect = canvas.getBoundingClientRect();
		    return {
		      x: evt.clientX - rect.left,
		      y: evt.clientY - rect.top
		    };
		}
	});

	// Cred till:
	// https://stackoverflow.com/questions/34483940/best-way-to-run-mousemove-only-on-mousedown-with-pure-javascript
	// https://stackoverflow.com/questions/17130395/real-mouse-position-in-canvas
	// https://www.w3schools.com/tags/canvas_createimagedata.asp
	// https://developer.mozilla.org/en-US/docs/Web/API/Canvas_API/Tutorial/Pixel_manipulation_with_canvas

</script>