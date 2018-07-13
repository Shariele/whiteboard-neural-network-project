<?php 
function createChart($name){
	include "dbConnect.php";

	$sql = "INSERT INTO charts(name) VALUES('$name')";
	$sth = $pdo->prepare($sql);
	$sth->execute();

	// $sql = "SELECT id FROM charts WHERE name = \"$name\"";
	// $sth = $pdo->prepare($sql);
	// $sth->execute();

	// $result = $sth->fetchAll();
	// foreach ($result as $key) {
	// 	$id = $key['id'];
	// }

	// $sql = "INSERT INTO chartValues(chartId, value) VALUES($id, $value)";
	// $sth = $pdo->prepare($sql);
	// $sth->execute();
}

function addChartValue($id, $value, $date){
	include "dbConnect.php";

	$sql = "INSERT INTO chartvalues(chartid, value, date) VALUES($id, $value, $date)";
	$sth = $pdo->prepare($sql);
	$sth->execute();
}



?>
