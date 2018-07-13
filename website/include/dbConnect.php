<?php
// Ansluter till MySQL och den angivna databasen
$dsn = "mysql:host=localhost; dbname=shariele_grafapplikation;";
$login = "Shariele";
$password = "vveSuQbwgRlgNEop";
$options = array(PDO::ATTR_EMULATE_PREPARES => false,
	PDO::ATTR_ERRMODE =>PDO::ERRMODE_EXCEPTION,"charset=utf8");
$pdo = new PDO($dsn, $login, $password, $options);
?>