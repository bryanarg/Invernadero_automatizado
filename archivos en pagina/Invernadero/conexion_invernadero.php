<?php
$servername = "localhost";
$database = "u492392758_inverka";
$username = "u492392758_Arguello";
$password = "Arguello_19";

// Create connection
$conn = mysqli_connect($servername, $username, $password, $database);
// Check connection
if (!$conn) {
      die("Connection failed: " . mysqli_connect_error());
}

echo "Connected successfully";

$sql = "INSERT INTO datos (ID, fecha_hora, hum, temp, mesa1, mesa2, mesa3, mesa4, mesa5, mesa6, lux1, lux2) VALUES (null, DATE_SUB(NOW(), INTERVAL 6 HOUR), '$_GET[hum]',  '$_GET[temp]', '$_GET[mesa1]', '$_GET[mesa2]', '$_GET[mesa3]', '$_GET[mesa4]', '$_GET[mesa5]', '$_GET[mesa6]', '$_GET[lux1]', '$_GET[lux2]' )";
if (mysqli_query($conn, $sql)) {
      echo "New record created successfully";
} else {
      echo "Error: " . $sql . "<br>" . mysqli_error($conn);
}
mysqli_close($conn);
?>