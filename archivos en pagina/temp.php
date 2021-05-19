
<?php 

require_once "php/conexion.php";
$conexion = conexion();


$sql = "SELECT fecha_hora, temp FROM datos WHERE ID Between 2000 And 2999";
$result = mysqli_query($conexion,$sql);

$valoresY = array();
$valoresX = array();

while ($ver = mysqli_fetch_row($result)) {
		$valoresY[] = $ver[1]; //temp
		$valoresX[] = $ver[0]; //fecha
	}

	$datosX = json_encode($valoresX);
	$datosY = json_encode($valoresY);
	
?>
	<div id="graficaLineal2"></div>


	<script type="text/javascript">

		function crearCadenaLineal(json){
			var parsed = JSON.parse(json);
			var arr = [];
			for (var x in parsed) {
				arr.push(parsed[x]);
			}

			return arr;
		}

	</script>

	<script type="text/javascript">

		datosX = crearCadenaLineal('<?php echo $datosX; ?>');
		datosY = crearCadenaLineal('<?php echo $datosY; ?>');

		var trace1 = {
			x: datosX,
			y: datosY,
			type: 'scatter',
			line: {
				color: 'red',
				width: 2
			},
			marker: {
				color: 'blue',
				size: 12
			}
		}

		var layout = {
			title: 'Temperatura',
			xaxis: {
				title: 'Fechas'
			},
			yaxis: {
				title: 'Temperatura'
			}
		};

		var data = [trace1];

		Plotly.newPlot('graficaLineal2', data, layout);
	</script>