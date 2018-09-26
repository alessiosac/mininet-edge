#Mininet test for Edge network

Follow the [instructions](http://mininet.org/download/) for installing Mininet.
A minimal environment comprises:
    
    $ sudo apt-get install mininet 

Remember to use Python v2 instead of v3, since Mininet API are provided for v2 only.

Run this program with:
    
    $ sudo python topology.py
       
In order to simulate client's traffic run the `client-traffic.py` too.

Note, if during the execution submit.py script crashes for some reason or you terminate it using CTRL+C, 
make sure to clean mininet environment using:

    $ sudo mn -c
    
    
##Random topology

Before executing the Mininet script, create a python file capable of creating a 
random topology with `N` switches, `E` links, `H` hosts and `MAX_BW` in the links, 
attached to the controller at IP `Controller_IP`
run the following commands:

    $ gcc -o random-topology random-topology.c
    $ ./random-topology N E H MAX_BW Controller_IP
    
For instance, a possible configuration could be:

    $ ./random-topology 10 10 10 100
    
Now deploy the topology on Mininet:

    $ sudo python test_topology.py