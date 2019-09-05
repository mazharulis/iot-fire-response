<?php

/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-mysql-database-flamep/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

$servername = "localhost";

// REPLACE with your Database name
$dbname = "fireiot";
// REPLACE with Database user
$username = "root";
// REPLACE with Database user password
$password = "";

// Keep this API Key value to be compatible with the ESP32 code provided in the project page. 
// If you change this value, the ESP32 sketch needs to match
$api_key_value = "tPmAT5Ab3j7F9";

$api_key= $sensor1 = $gas = $sensor2 = $flame = $sensor3 = $gps = $decision = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $api_key = test_input($_POST["api_key"]);
    if($api_key == $api_key_value) {
        $sensor1 = test_input($_POST["sensor1"]);
		$gas = test_input($_POST["gas"]);
        $sensor2 = test_input($_POST["sensor2"]);
		$flame = test_input($_POST["flame"]);
        $sensor3 = test_input($_POST["sensor3"]);
		$gps = test_input($_POST["gps"]);
        $decision = test_input($_POST["decision"]);
        //$value3 = test_input($_POST["value3"]);
        
        // Create connection
        $conn = new mysqli($servername, $username, $password, $dbname);
        // Check connection
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        } 
        
        $sql = "INSERT INTO iotdb (sensor1, gas, sensor2, flame, sensor3, gps, decision)
        VALUES ('" . $sensor1 . "', '" . $gas . "', '" . $sensor2 . "', '" . $flame . "', '" . $sensor3 . "', '" . $gps . "', '" . $decision . "')";
        
        if ($conn->query($sql) === TRUE) {
            echo "New record created successfully";
        } 
        else {
            echo "Error: " . $sql . "<br>" . $conn->error;
        }
    
        $conn->close();
    }
    else {
        echo "Wrong API Key provided.";
    }

}
else {
    echo "No data posted with HTTP POST.";
}

function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}