# ZedBrain platform
# A research application from the Erasmus Brain project at Erasmus Medical Center (Erasmus MC) Neuroscience Department 
# Site: http://www.erasmusbrainproject.com/

# Collaborating institutions: Tecnologico de Costa Rica (TEC)

**Simulate brain cells using complex biologically accurate models, with the power of multiple ZedBoards**

ZedBrain is an open hardware demo that shows the capabilities of using the Zedboard Zynq platform and Xilinx Vivado HLS tools for hardware accelerated simulations of biologically accurate artificial neuron networks. 

Computing intensive models such as the Hodgkin-Huxley's can be implemented on C or C++ and synthesized on the PL fabric of the Zynq platform for high speed performance. Multiple Zedboards connected through the TCP-IP protocol can extend simulations to networks with thousands of cells, with quick, accurate results.

## Directory structure
PHPFiles/ ------------ this directory contains the files necessary to create the website.

PythonFiles/ ------------ this directory contains the python scripts used in the simulation and graph creation. In Ubuntu, 


## Instructions (Last revision: 6-28-2017)
To build the website, several steps have to be executed:
1. Install Apache2 server. 
2. Install PHP 5 or later.
3. Create a directory that will hold the website. In Ubuntu, create the directory like this **/var/www/zedbrain.com/public_html**.
4. Give extra permissions to the general web directory. In Ubuntu, type **sudo chmod -R 755 /var/www**.
5. Make a copy to the file in **/etc/apache2/sites-available/000-default.conf** and change the name by zedbrain.com.conf
6. Change several directives in this file like ServerName and DocumentRoot according the new website.
7. Restart Apache to make these changes take effect.
8. Move php files and python files to the directory **public_html**.
9. Create extra directory called css and move the CSS files in there.
10. To make the website more flexible, open the file in the route **/etc/php5/apache2** and increase the size of variables like **post_max_size**, **upload_max_filesize** and **memory_limit**.

To test the website, it was used Ubuntu 15.04, Apache2, Python 2.7 and PHP5.

## Collaborators

PhD Students: Georgios Smaragdos, Carlos Salazar-Garcia

Undergraduate Assistants: Kaleb Alfaro-Badilla, Daniel Zamora-Umaña, Marco Acuña-Vargas

Advisers: Christos Strydis (Erasmus MC), Alfonso Chacon-Rodriguez (TEC)


