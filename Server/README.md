# ZedBrain platform
# A research application from the Erasmus Brain project at Erasmus Medical Center (Erasmus MC) Neuroscience Department 
# Site: http://www.erasmusbrainproject.com/

# Collaborating institutions: Tecnologico de Costa Rica (TEC)

**Simulate brain cells using complex biologically accurate models, with the power of multiple ZedBoards**

ZedBrain is an open hardware demo that shows the capabilities of using the Zedboard Zynq platform and Xilinx Vivado HLS tools for hardware accelerated simulations of biologically accurate artificial neuron networks. 

Computing intensive models such as the Hodgkin-Huxley's can be implemented on C or C++ and synthesized on the PL fabric of the Zynq platform for high speed performance. Multiple Zedboards connected through the TCP-IP protocol can extend simulations to networks with thousands of cells, with quick, accurate results.

## Directory structure
PHPFiles/ ------------ this directory contains the files necessary to create the website.

PythonFiles/ ------------ this directory contains the python scripts used in the simulation and graph creation.

Script_stimuli_gen/ ------------ this directory contains the python scripts used in to create the network stimulus.

## Instructions (Last revision: 6-28-2017)
To build the website, several steps have to be executed:
1. Install Apache2 server. 
2. Install PHP 5 or later.
3. Create a directory that will hold the website. In Ubuntu, create the directory like this _/var/www/zedbrain.com/public___html_.
4. Give extra permissions to the general web directory. In Ubuntu, type **sudo chmod -R 755 /var/www**
5. Give edit permissions to be able to modify files in your web directory **sudo chown -R $USER:$USER /var/www/zedbrain.com/public_html**
6. Make a copy to the file in _/etc/apache2/sites-available/000-default.conf_ and change the name by **zedbrain.com.conf**
7. Add several directives in this file like ServerName **zedbrain.com**, ServerAlias **www.zedbrain.com** and DocumentRoot _/var/www/zedbrain.com/public___html_.
8. Enable the website typing **sudo a2ensite zedbrain.com.conf**
9. Disable the default site typing **sudo a2dissite 000-default.conf**
10. Restart Apache to make these changes take effect typing **sudo service apache2 restart**
11. Move php files and python files to the directory _zedbrain.com/public___html_.
12. Create extra directory called **css** and move the CSS files in there.
13. Create extra directory called **images** in public_html directory and put all the images.tar files in there. 
14. Create extra directory called **file** in public_html directory. This directory is used to upload the stimulation file.
15. Create new files called **dataG.txt** and  **dataG1.txt** in public_html directory and give them write permissions by clicking in properties/permissions/others access/read and write.
16. Give to _public___html_ directory and _file_ directory extra permissions by clicking in properties/permissions/others access/create and delete files.
17. To make the website more flexible, open the file **php.ini** in the route _/etc/php5/apache2/php.ini_ and increase the size of variables like **post_max_size**, **upload_max_filesize** and **memory_limit**
18. Test the website by using to Firefox web browser and typing localhost.
19. To create a new simulation, create the network stimulus running the script **std_stimulus_gen.py**. Edit this script according network size and the simulation steps that you need.
20. Upload the stimulus file and run a simulation to get results and see them in the website. 

To test the website, it was used Ubuntu 15.04, Apache2, Python 2.7, Firefox 44.0 and PHP5.

## Collaborators

PhD Students: Georgios Smaragdos, Carlos Salazar-Garcia

Undergraduate Assistants: Kaleb Alfaro-Badilla, Daniel Zamora-Umaña, Marco Acuña-Vargas

Advisers: Christos Strydis (Erasmus MC), Alfonso Chacon-Rodriguez (TEC)

## License
Copyright 2017 Erasmus Brain project

BSD-3-Clause

