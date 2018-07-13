<!DOCTYPE html>
<html>
<head>
	<title>Whiteboard</title>
	<?php include "include/necessities.php"; ?>
</head>
<body style="background-color: grey;">
	
		<svg style="margin-top: 300px; margin-left: 600px; height:500px; width: 500px;" id="mysvg" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" viewBox="0 0 500 500" preserveAspectRatio="xMidYMid meet">
			<g id="local" transform="scale(4)">
		    </g>
		</svg>

	<p id="coords">co-ordinates</p>
</body>
</html>


<!-- https://codepen.io/SitePoint/pen/xEKzjq -->


<script type="text/javascript">
	var
		svg = document.getElementById('mysvg'),
		NS = svg.getAttribute('xmlns'),
		local = svg.getElementById('local'),
		coords = document.getElementById('coords');

	// update co-ordinate output
	svg.addEventListener('mousemove', function(e) {

	  var
	    x = e.clientX,
	    y = e.clientY,
	    svgP = svgPoint(svg, x, y),
	    svgL = svgPoint(local, x, y);

	  // output co-ordinates
	  coords.textContent =
	    '[page: ' + x + ',' + y +
	    '] => [svg space: ' + Math.round(svgP.x) + ',' + Math.round(svgP.y) +
	    '] [local transformed space: ' + Math.round(svgL.x) + ',' + Math.round(svgL.y) + ']'
	    ;
	    //console.log(coords.textContent);
	}, false);

	svg.addEventListener('mousedown', function(){
		svg.addEventListener('mousemove', draw);
	});

	svg.addEventListener('mouseup', function(){
		this.removeEventListener('mousemove', draw);
	});

	function draw(e){
		var
		    t = e.target,
		    x = e.clientX,
		    y = e.clientY,
		    target = (t == svg ? svg : t.parentNode),
		    svgP = svgPoint(target, x, y),
		    circle = document.createElementNS(NS, 'circle');
		  
		circle.setAttributeNS(null, 'cx', Math.round(svgP.x));
		circle.setAttributeNS(null, 'cy', Math.round(svgP.y));
		circle.setAttributeNS(null, 'r', 5);
		target.appendChild(circle);
	}

	// translate page to SVG co-ordinate
	function svgPoint(element, x, y) {
	  
	  var pt = svg.createSVGPoint();
	  pt.x = x;
	  pt.y = y;
	  return pt.matrixTransform(element.getScreenCTM().inverse());
	  
	}
</script>