<!DOCTYPE html>
<html>
<head>
	<title>Whiteboard</title>
	<?php include "include/necessities.php"; ?>
</head>
<body style="background-color: grey;">
	<div id="resultHere"></div>

	<div class="btn btn-lg btn-success" id="saveButton">Save!</div>	
	<input type="text" name="value" id="value">
	<svg style="margin-top: 300px; margin-left: 600px; height:512px; width: 512px;" id="mysvg" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" viewBox="0 0 500 500" preserveAspectRatio="xMidYMid meet">
		<g id="local" transform="scale(4)">
	    </g>
	</svg>

	<p id="coords">co-ordinates</p>
</body>
</html>


<!-- Cred för mycket hjälp -->
<!-- https://codepen.io/SitePoint/pen/xEKzjq -->


<script type="text/javascript">
	var mSize = 512;
	var ratio = 16;

	var mappingMatrix = new Array(mSize/ratio); 	//32
	var matrix = new Array(mSize);

	for (var i = 0; i < mSize; i++)
		matrix[i] = new Array(mSize);

	for (var i = 0; i < mSize/ratio; i++)
		mappingMatrix[i] = new Array(mSize/ratio);

	for(var i = 0; i < mSize/ratio; i++)
		for(var j = 0; j < mSize/ratio; j++)
			mappingMatrix[i][j] = 0;

	for(var i = 0; i < mSize; i++)
		for(var j = 0; j < mSize; j++)
			matrix[i][j] = 0;

	var
		svg = document.getElementById('mysvg'),
		NS = svg.getAttribute('xmlns'),
		local = svg.getElementById('local'),
		coords = document.getElementById('coords');

	svg.addEventListener('mousedown', function(){
			svg.addEventListener('mousemove', draw);
	});

	svg.addEventListener('mouseup', function(){
		this.removeEventListener('mousemove', draw);
		console.log(matrix);
	});

	function draw(e){
		var
		    t = e.target,
		    x = e.clientX,
		    y = e.clientY,
		    target = (t == svg ? svg : t.parentNode),
		    svgP = svgPoint(target, x, y),
		    circle = document.createElementNS(NS, 'circle');

		xpos = Math.round(svgP.x);
	  	ypos = Math.round(svgP.y);
		  
		circle.setAttributeNS(null, 'cx', xpos);
		circle.setAttributeNS(null, 'cy', ypos);
		circle.setAttributeNS(null, 'r', 5);
		target.appendChild(circle);

		// Set to one to show pixel has been filled
		matrix[xpos][ypos] = 1;
		
	}

	// translate page to SVG co-ordinate
	function svgPoint(element, x, y) {
	  
	  var pt = svg.createSVGPoint();
	  pt.x = x;
	  pt.y = y;
	  return pt.matrixTransform(element.getScreenCTM().inverse());
	  
	}


	saveBtn = document.getElementById("saveButton");
	saveButton.addEventListener("click", function() {
		notifyBackEnd();

		// Save user data to mysql and then tell node js server
		// and give it the user's matrix id.
		mappMatrix();

	    $.ajax({
	        type: "POST",
	        url: "scripts.php",
	        data: {
	            matrix: JSON.stringify(mappingMatrix),
	            number: document.getElementById("value").value
	            // action: "changePage"
	        },
	        success:  function(data){
	            $('#resultHere').html(data);
	            console.log(JSON.stringify(mappingMatrix));
	        }
	    });
	});

	// Mapps the bigger matrix to a smaller one.
	function mappMatrix (){
		for(var mY = 0; mY < mSize; mY = mY + ratio){
			for(var mX = 0; mX < mSize; mX = mX + ratio){
				for(var x = mX; x < mX + ratio; x++){
					for(var y = mY; y < mY + ratio; y++){
						if(matrix[x][y] === 1)
							mappingMatrix[(mX/ratio)][(mY/ratio)] = 1;
					}
				}
			}
		}
		console.log(mappingMatrix);
	}

	function notifyBackEnd(){
		var data = {};
		data.info = "Message from website~~";

		$.ajax({
			url: 'http://localhost:4000/notification',
			type: 'POST',
			contentType: 'application/json',
			data: JSON.stringify(data),
			success: function(data) {
				console.log(data);
				console.log("success");
			},
			error : function(e) {
				console.log("ERROR: ", e);
			}
		});
	}
</script>