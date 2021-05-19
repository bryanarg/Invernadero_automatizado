<?php 
	function conexion(){
		return mysqli_connect(	'localhost', //normalmente no es necesario cambiar localhost aunque este en un servidor en linea ya que la pagina y la base de datos estan en el mismo ser
					'Usuario',
					'Contraseña',
					'Base de Datos');
	}
 ?>
