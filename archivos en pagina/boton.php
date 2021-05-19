<?php

require_once "php/conexion.php";
$conexion = conexion();

$sql = "INSERT INTO manipulables (ID, humedad_suelo_max, Humedad_suelo_min, cantidad_mesas) VALUES (null, '$_REQUEST[humax]', '$_REQUEST[humin]',6)";
if (mysqli_query($conexion, $sql)) {
    echo "Los dato se ingresaron correctamente";
} else {
    echo "Error: " . $sql . "<br>" . mysqli_error($conexion);
}
mysqli_close($conexion);

?>

<form method = "post" action = "index.
php"> 
	<br>
	<br>
	<input type="submit" 
value="regresar">
</form>