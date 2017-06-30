# ZedBrain platform
# A research application from the Erasmus Brain project at Erasmus Medical Center (Erasmus MC) Neuroscience Department 
# Site: http://www.erasmusbrainproject.com/

# Collaborating institutions: Tecnologico de Costa Rica (TEC), Vineyard EU Project.

**Simulate brain cells using complex biologically accurate models, with the power of multiple ZedBoards**

ZedBrain is an open hardware demo that shows the capabilities of using the Zedboard Zynq platform and Xilinx Vivado HLS tools for hardware accelerated simulations of biologically accurate artificial neuron networks. 

Computing intensive models such as the Hodgkin-Huxley's can be implemented on C or C++ and synthesized on the PL fabric of the Zynq platform for high speed performance. Multiple Zedboards connected through the TCP-IP protocol can extend simulations to networks with thousands of cells, with quick, accurate results.

## Directory structure
PHPFiles/ ------------ Files necessary for the creation of the website.

PythonFiles/ ------------ Python scripts used for simulation and graph plots.

Script_stimuli_gen/ ------------ Ppython scripts necessary to create the network stimulus.

## Instructions (Last revision: 6-28-2017)
To build the website, follow these steps:
1. Install the Apache2 server. 
2. Install PHP 5 or later.
3. Create the directory that will contain the website. If using Ubuntu, you can create a directory such as the following: _/var/www/zedbrain.com/public___html_
4. Give access permissions to the general web directory. In Ubuntu, type **sudo chmod -R 755 /var/www**
5. Give edit permissions in order to be able to modify files in your web directory. For instance: **sudo chown -R $USER:$USER /var/www/zedbrain.com/public_html**
6. Copy the file in _/etc/apache2/sites-available/000-default.conf_ and change the name to **zedbrain.com.conf**
7. You need to add several directives to this file, such as (names to type enhanced in bold): ServerName **"zedbrain.com**, ServerAlias **www.zedbrain.com** and DocumentRoot _/var/www/zedbrain.com/public___html_
8. Enable the website typing at the command prompt: ** sudo a2ensite zedbrain.com.conf**
9. Disable the default site typing then at the command prompt **sudo a2dissite 000-default.conf**
10. Restart Apache to make these changes take effect, by typing at the command prompmt **sudo service apache2 restart**
11. Move php files and python files to the _zedbrain.com/public___html_ directory_
12. Create an extra directory called **css** and move all the CSS files in there.
13. Create an extra directory called **images** in the public_html directory and put all the images.tar files in there. 
14. Create en extra directory called **file** in the public_html directory. This directory is used to upload the stimulation file.
15. Create new files called **dataG.txt** and  **dataG1.txt** in public_html directory and give them write permissions by clicking in properties/permissions/others access/read and write.
16. Give extra permissions to the _public___html_ and _file_ directories  by clicking in properties/permissions/others access/create and delete files.
17. To make the website more flexible, open the file **php.ini** ( _/etc/php5/apache2/php.ini_ ) and increase the size of variables like **post_max_size**, **upload_max_filesize** and **memory_limit**
18. Test the website by using any web browser and typing localhost (Firefox was used in this case).
19. To create a new simulation, create the network stimulus running the script **std_stimulus_gen.py**. Edit this script according to the intended network size and the simulation steps that you need.
20. Upload the stimulus file and run a simulation. Results should appear after a while in the website. 

This website was tested using: Ubuntu 15.04, Apache2, Python 2.7, Firefox 44.0 and PHP5.

## Collaborators

PhD Students: Georgios Smaragdos, Carlos Salazar-Garcia

Undergraduate Assistants: Kaleb Alfaro-Badilla, Daniel Zamora-Umaña, Marco Acuña-Vargas

Advisers: Christos Strydis (Erasmus MC), Alfonso Chacon-Rodriguez (TEC)

## License
Copyright 2017 Erasmus Brain project

BSD-3-Clause

