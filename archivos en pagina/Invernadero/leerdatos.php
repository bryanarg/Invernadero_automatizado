<?php

 $servername = "localhost";
$database = "u492392758_inverka";
$username = "u492392758_Arguello";
$password = "Arguello_19";

// Create connection
$con = mysqli_connect($servername, $username, $password, $database);

if (mysqli_connect_errno()) {
    echo "Failed to connect to MySQL: " . mysqli_connect_error();
    exit();
}

$sql = "SELECT humedad_suelo_max, humedad_suelo_min, cantidad_mesas FROM manipulables ORDER BY ID DESC LIMIT 1";

if ($result = mysqli_query($con, $sql)) {
    // Fetch one and one row
    while ($row = mysqli_fetch_row($result)) {
    echo "max=$row[0]; min=$row[1]; cme=$row[2];";
    
    }
    mysqli_free_result($result);
}

mysqli_close($con);



?>