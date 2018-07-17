<?php 

include "include/dbConnect.php";

$matrix = json_decode($_REQUEST['matrix']);
$number = $_REQUEST['number'];

// echo $number;

// echo $matrix[0][0];
$matrixId = matrixId();	
// echo "matrixId: ".$matrixId;

for($i = 0; $i < 32; $i++){
	for($j = 0; $j < 32; $j++){
		echo $matrix[$j][$i]." ";
	}
	echo "<br>";
}

// try {
// 	$pdo->beginTransaction();

// 	$sql = "INSERT INTO matrix (matrix_id, col, row, value) VALUES (?, ?, ?, ?)";
// 	$stmt = $pdo->prepare($sql);
// 	for($x = 0; $x < 32; $x++)
// 		for($y = 0; $y < 32; $y++)
// 			$stmt->execute([$matrixId, $x, $y, $matrix[$x][$y]]);

// 	$sql = "INSERT INTO numbers (number, matrix_id) VALUES (?, ?)";
// 	$pdo->prepare($sql)->execute([$number, $matrixId]);

// 	$pdo->commit();
// }catch (Exception $e){
// 	$pdo->rollback();
// 	throw $e;
// }

function matrixId(){
	include "include/dbConnect.php";


	$sql = "SELECT DISTINCT matrix_id FROM matrix";
	$stmt = $pdo->prepare($sql);
	$stmt->execute();
	$row = $stmt->fetchAll(PDO::FETCH_ASSOC);
	// var_dump($row);
	if(!$row)
		return 0;
	else
		return count($row);

	
}


 ?>


