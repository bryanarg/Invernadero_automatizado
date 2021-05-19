
<?php 

require_once "php/conexion.php";
$conexion = conexion();


$sql = "SELECT fecha_hora, mesa4  FROM datos ORDER BY ID DESC LIMIT 25000";
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
	<div id="graficaLineal6"></div>


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
				color: 'green',
				size: 12
			}
		}

		var layout = {
			title: 'Mesa No.4',
			xaxis: {
				title: 'Hora'
			},
			yaxis: {
				title: 'Nivel de Humedad'
			}
		};

		var data = [trace1];

		Plotly.newPlot('graficaLineal6', data, layout);
	</script>