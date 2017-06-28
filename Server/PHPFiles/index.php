<!------------------------------------------------------------------------------------------------
#FILE NAME : index.php
#Written by: Daniel Zamora_Umaña.
#This code is a script to create a website. It was tested and run with Apache server.
#CREATED : 2016
#LAST ACTUALIZATION : 27/06/2017
#PHP 5 HTML5
#Copyright 2017 Erasmus Brain Project
-------------------------------------------------------------------------------------------------->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">

	<head>
		<title>ZedBrain</title>
		
		<link rel="stylesheet" type="text/css" href="css/styles.css">
	</head> 

<!--Script function to pop-up windows-------------------------------------------------------------->
<script>
function myFunction(number,value)
{
if(number==1){alert("File uploaded, Computing new Simulation.");} 
else if(number==2){alert("Select a File with the stimulus.");}
}
</script>


	<body>		
<!--Header section & buttons----------------------------------------------------------------------->		
		<header>
			<nav>

				<form id="button" name="form2" method="POST" enctype="multipart/form-data" action="">
				<input type="submit" id="clear" name="clear" value="Clear Data" class="button" />
				</form>

				<form id="zeds" name="form3" method="POST" enctype="multipart/form-data" action="">
				<input type="submit" id="zeds" name="zeds" value="Scan Devices" class="button" />
				</form>
					</li>
				<a href="dataG.txt" download="dataG.txt" class="button" >Save</a>
				<form id="reload" name="form2" method="POST" enctype="multipart/form-data" action="">
				<form action="">
				<input type="file" id="file2" name="file2" class="inputfile"/>
				<label for="file2">Load</label>
				<input type="submit" id="reload" name="reload" value="Refresh" class="button" />		
				<a href="info.php" class="button2">Info</a>
			
			</nav>
		</header>
			<h1 align="center" > ZedBrain </h1>
			
			<HR>

<!--Images section, Brain representation and devices availables ----------------------------------------------------------------------->
		
		<div class="flex-container">
			<img src="images/zedboard_disp.png" width="409" height="238" align="center"/>
		</div>


		<div class="flex-container">
				<img id="img1" src="images/brainFrame_off.png" alt="Brain" usemap="#brainmap" width="409" height="268" align="middle">
				<map name="brainmap">
 				<area shape="circle" coords="270,180,25" alt="IO" href="javascript:(function(){document.getElementById('img1').src='images/brainFrame_IO.png';})()">
 				<area shape="circle" coords="200,210,25" alt="PC" href="javascript:(function(){document.getElementById('img1').src='images/brainFrame_PC.png';})()">
 				<area shape="circle" coords="310,225,25" alt="DCN" href="javascript:(function(){document.getElementById('img1').src='images/brainFrame_DCN.png';})()">
				</map>
		</div>

<!--Graphics section, the graphic representations are show here. -------------------------------------------------------------------->
		
		<div class="flex-containergrahp" style="position: absolute; top: 600px; right: 0px;">
				<center>
				<label for="neu">Choose neuron response to display</label>
				<input type="text" id="neu" name="neu" placeholder="Number to Plot" class="corra">
				<center>
 				</form>
				</form>
				<img src="images/myfig.png" width="690" height="350" />
		</div>
		<div class="flex-containergrahp" style="position: absolute; top: 600px; left: 0px;">
				<br><br><br>
				<img src="images/Network.png" width="690" height="350" />
		</div>
		<div class="flex-container3D" style="position: absolute; top: 1070px; right:300px">
				<a class="hver" href="#nogo" title="Volumetric plot of axon response for neuron population."><img src="images/anim.gif" alt="Volumetric plot."  /></a>
		</div>

<!--Side menu.---------------------------------------------------------------------------------------------------------->
		
		<aside>
				<form id="enviar" name="form1" method="POST" enctype="multipart/form-data" >
				<form action="">
				<input type="file" id="file" name="file" class="inputfile" onclick="myFunction(2)"/>
				<label for="file">Choose a file</label>
				<br><br><br>
				<label for="Zedboard">FPGA Devices</label>
				<input type="text" id="Zedboard" name="Zedboard" placeholder="Value(Default 1)">
				<br>
				<label for="Nsize">Neuronal Network Size</label>
				<input type="text" id="Nsize" name="Nsize" placeholder="Value...">
				<br>
				<label for="Step">Simulation Steps</label>
				<input type="text" id="Step" name="Step" placeholder="Value...">
					    
				<input type="submit" id="send" name="send" value="send" onclick="myFunction(1)">
				</form>
				</form>
		</aside>
				


		
		<br><br><br><br><br><br><br><br><br><br><br><br><br><br><br>
		<br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br>
		<br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br>
		<br><br><br><br><br><br><br><br><br><br><br><br><br><br>

<!--Footer Section ------------------------------------------------------------------------------------------------>
		<footer>
					
			<nav>  
			
				<img class="imageFooterLeft" src="images/DCIlogo.png" width="580" height="200" />
			
				<img class="imageFooterRight" src="images/Erasmuslogo.png" />
						
			</nav>
		</footer>

<!---------------------------------------------------------------------------------------------------- PHP BLOCK -->
	<?php
	if(isset($_POST['reload'])){
		$newname = 'dataLoaded.txt';
		if (move_uploaded_file($_FILES['file2']['tmp_name'] , $newname)){
			$origen='dataLoaded.txt';
			$destino='dataG.txt';
			if (copy($origen, $destino)) {
   				 	echo '<script> alert ("Your file was loaded, ready to plot."); </script>';
			}
			unlink('dataLoaded.txt');
		}
		$b=1;
		$x=$_POST['neu'];
		if (empty($x)) {
			$y=1;
		} else {
			$y=intval($x);
			}
		echo '<script> alert ("Axonal Response Voltage from neuron  '. ($y) .'"); </script>';

		$lines = file("dataG.txt");
		for ($i=0; $i < count($lines) ; $i++) { 
	 		$new_array = explode("\t", $lines[$i]);
	 		$ydata[$i] = $new_array[$y-1];
			$ydata[$i]=rtrim($ydata[$i]);
			$nsize=sizeof($new_array);}

		$config='';
	 	for ($a=0; $a <count($ydata) ; $a+=$b) { 	
	 		$config=$config.$ydata[$a]. PHP_EOL;
	 	}
		$myfile = fopen("dataG1.txt", "w") or die("Unable to open file!");	
	 	fwrite($myfile, $config);
		fclose($myfile);
		$command = 'python Axongraphic.py ' . $y;
		$output = passthru($command);
		$commandRaster = 'python Rastergraphic.py ' . 1 . ' ' . ($nsize-1);
		$output = passthru($commandRaster);
		$commandVolumetric = 'python Volumetricgraphic.py ' . ($nsize-1);
		$output = passthru($commandVolumetric);
		}

	if(isset($_POST['clear'])){
		$lines = file("dataG.txt");
	 	$new_array = explode("\t", $lines[1]);
		$nsize=sizeof($new_array);
		$myfile = fopen("dataG1.txt", "w") or die("Unable to open file!");
		$txt = "0";
		fwrite($myfile, $txt);
		fclose($myfile);
		$command = 'python Axongraphic.py ' . 0;
		$output = passthru($command);
		$commandRaster = 'python Rastergraphic.py ' . 0 . ' ' . ($nsize-1);
		$output = passthru($commandRaster);
		echo '<script> alert("Graphic Cleared"); </script>';
		}
	if(isset($_POST['send'])){
		$newname = 'data.txt';
		move_uploaded_file($_FILES["file"]["tmp_name"],'file/'.$newname);
		$Nsize =$_POST['Nsize'];
		$step =$_POST['Step'];
		$Zedboards =$_POST['Zedboard'];
		if (empty($Zedboards)) {
			$Zedboards=1;}
		$command = 'python ethernet.py ' . $Nsize . ' ' . $step . ' ' . $Zedboards;
		#$output = passthru($command);
		echo '<script> alert("Simulation Complete!"); </script>';

		}

	if(isset($_POST['zeds'])){
		$output2 = shell_exec('nmap -n 192.168.1.0-18');
		$position = strrpos($output2,"hosts up");
		$y= $output2[$position -2];
		if (($y-1) < 5 & ($y-1) > 0){
			$origen='images/zedboard'.strval($y-1).'.png';}
		else{
			$origen='images/zedboard0.png';}
		$destino='images/zedboard_disp.png';
		copy($origen,$destino);
		}
	?>
<!---------------------------------------------------------------------------------------------------- PHP BLOCK -->


	</body>
</html>


<!------------------------------------------------------------------------------------------------
#FILE NAME : index.php
#Written by: Daniel Zamora_Umaña.
#This code is a script to create a website. It was tested and run with Apache server.
#CREATED : 2016
#LAST ACTUALIZATION : 27/06/2017
#PHP 5 HTML 5
#Copyright 2017 Erasmus Brain Project
-------------------------------------------------------------------------------------------------->
