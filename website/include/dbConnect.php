<?php
// Ansluter till MySQL och den angivna databasen
$dsn = "mysql:host=localhost; dbname=whiteboard_train_data;";
$login = "root";
$password = "";
$options = array(PDO::ATTR_EMULATE_PREPARES => false,
	PDO::ATTR_ERRMODE =>PDO::ERRMODE_EXCEPTION,"charset=utf8");
$pdo = new PDO($dsn, $login, $password, $options);
?>