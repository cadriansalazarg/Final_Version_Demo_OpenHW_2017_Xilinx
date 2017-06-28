<!------------------------------------------------------------------------------------------------
#FILE NAME : info.php
#Written by: Daniel Zamora_Umaña.
#This code is a script to create a webpage. It was tested and run with Apache server.
#CREATED : 2016
#LAST ACTUALIZATION : 27/06/2017
#PHP 5 HTML5
#Copyright 2017 Erasmus Brain Project
-------------------------------------------------------------------------------------------------->


<html>
	<head>
		<title>About System Test</title>
		<link rel="stylesheet" type="text/css" href="css/stylesInfo.css">
	</head>
	<body>
		<header>
			<nav>
				<ul>
				
					<li><a href="index.php" class="button">Main</a></li>
					
				</ul>
			</nav>
		</header>
		<section class="main">
			
			<center>
			<br>
			<h1> Using the ZedBrain hardware simulator </h1>
			<br>
			<HR>
			<br>
			<br>
			
			<p>The system allows the user either to simulate a particular network in response to a stimulus file, or post-processing data from previous simulations stored in TXT files. Details of each of the system's functions are given next. </p>
			<br>
			<br>
			</center>
		<div>	
			<input type="submit" id="save" name="save" value="Save" class="button" /> 
			<br>
			<br><br><br><br>
			<p> Saves to an output text file the network's response previously simulated.</p>
			<HR>
			<input type="submit" id="load" name="load" value="Load" class="button" />
			<br>
			<br><br><br><br>
			<p>Loads a text file with a previous simulation. To apply results, press the refresh button.</p>
			<HR>
			<input type="submit" id="clear" name="clear" value="Clear Data" class="button" />
			<br>
			<br><br><br><br>
			<p>Clears the graph space.</p>
			<HR>
			<input type="submit" id="refresh" name="refresh" value="Refresh" class="button" />
			<br>
			<br><br><br><br>
			<p>Plots time response for one neuron. The graph represents the output from the neuron specified in the dialog box.</p>
			<HR>
			<input type="submit" id="choose" name="choose" value="Choose a file" class="button" />
			<br>
			<br><br><br><br>
			<p>Uploads a file with the initialization for every neuron to be simulated. This file includes the initialization constants that define the network type (in the current case, an inferior olivary neural network is offered as the sole example), the number of time steps to be simulated, and the input stimuli for each cell.</p>
			<HR>
			<input type="submit" id="send" name="send" value="Send" class="button" />
			<br>
			<br><br><br><br>
			<p>Pressing this button starts the simulation. A pop-up message alerts the user when the simulation is complete. </p>
			<HR>
			<input type="submit" id="scan" name="scan" value="Scan Devices" class="button" />
			<br>
			<br><br><br><br>
			<p>Scans for devices (FPGAs) connected. </p>
			<HR>

		</div>
		
		</section>
		<footer>
					
			<nav>
				<ul>    
					<div style="position: absolute; left: 1px;">
					<img class="image" src="images/DCIlogo.png" width="580" height="200" />
					</div>
					<img class="image2" src="images/Erasmuslogo.png" />
						
				</ul>
			</nav>
			
			
		</footer>	
	</body>
</html>

<!------------------------------------------------------------------------------------------------
#FILE NAME : info.php
#Written by: Daniel Zamora_Umaña.
#This code is a script to create a webpage. It was tested and run with Apache server.
#CREATED : 2016
#LAST ACTUALIZATION : 27/06/2017
#PHP 5 HTML5
#Copyright 2017 Erasmus Brain Project
-------------------------------------------------------------------------------------------------->

