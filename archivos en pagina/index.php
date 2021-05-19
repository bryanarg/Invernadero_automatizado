<!DOCTYPE html>
<html>
<head>
	<title>Inverka</title>
	<link rel="stylesheet" type="text/css" href="librerias/bootstrap/css/bootstrap.css">
	<script src="librerias/jquery-3.3.1.min.js"></script>
	<script src="librerias/plotly-latest.min.js"></script>
	
</head>
<body>
	<div class="container-fluid">
		<div class="row">
			<div class="col-sm-12">
				<div class="panel panel-primary">
					<div class="panel panel-heading">
						Automatización de Invernadero
					</div>
					<div class="panel panel-body">
						<div class="row">
							<div class="col-xs-6">
								<div id="cargaLineal"></div>
							</div>
							<div class="col-xs-6">
								<div id="cargaLineal2"></div>
							</div>
							<div class="col-sm-6">
								<div id="cargaLineal3"></div>
							</div>
							<div class="col-sm-6">
								<div id="cargaLineal4"></div>
							</div>
							<div class="col-sm-6">
								<div id="cargaLineal5"></div>
							</div>
							<div class="col-sm-6">
								<div id="cargaLineal6"></div>
							</div>
							<div class="col-sm-6">
								<div id="cargaLineal7"></div>
							</div>
							<div class="col-sm-6">
								<div id="cargaLineal8"></div>
							</div>
							<div class="col-sm-6">
								<div id="cargaLinealLux1"></div>
							</div>
							<div class="col-sm-6">
								<div id="cargaLinealLux2"></div>
							</div>
							<div class="col-sm-12">

							<form method = "post" action = "boton.php">
								<h3>
								!precaucion! no deje ningun recuadro en blanco
								</h3>
								ingrese cantidades:
								<br>
								<br>
								<input type="text" name="humax">
								<br>
									Humedad Maxima
								<br>
								<input type="text" name="humin">
								<br>
									Humedad Minima
								<br>
								<br>
								<br>
								<input type="submit" value="aceptar">

							</form>
							</div>
						</div>
						</div>
						</div>
					</div>
				</div>
			</div>
		</div>
	</div>
</body>
</html>

<script type="text/javascript">
	$(document).ready(function(){
		setInterval(
			function(){
				$('#cargaLineal').load('hum.php');
				$('#cargaLineal2').load('temp.php');
				$('#cargaLineal3').load('mesa1.php');
				$('#cargaLineal4').load('mesa2.php');
				$('#cargaLineal5').load('mesa3.php');
				$('#cargaLineal6').load('mesa4.php');
				$('#cargaLineal7').load('mesa5.php');
				$('#cargaLineal8').load('mesa6.php');
				$('#cargaLinealLux1').load('lux1.php');
				$('#cargaLinealLux2').load('lux2.php');
			}, 2000
		
		);
	});
</script>



