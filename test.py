#!/usr/bin/python

from mininet.cli import CLI
from mininet.log import lg, info
from mininet.topolib import TreeNet
from mininet.node import Controller, RemoteController, OVSKernelSwitch, UserSwitch
from mininet.net import Mininet
from mininet.link import Link, TCLink
from mininet.nodelib import NAT

def myNetwork():

    net = Mininet(topo=None,
                   build=False, controller=RemoteController, switch=OVSKernelSwitch)

    natIP='10.1.1.2'
    info( '*** Adding controller\n' )
    c0=net.addController(name='c0', ip='192.168.56.102', port=6633)

    info( '*** Add switches\n')
    s1 = net.addSwitch('s1')
    s2 = net.addSwitch('s2')
    s3 = net.addSwitch('s3')
    s4 = net.addSwitch('s4')
    s5 = net.addSwitch('s5')
    s6 = net.addSwitch('s6')
    s7 = net.addSwitch('s7')
    s8 = net.addSwitch('s8')
    s9 = net.addSwitch('s9')

    info( '*** Add hosts\n')
    h1 = net.addHost('h1', ip='10.1.1.3', defaultRoute='via '+ natIP)
    h2 = net.addHost('h2', ip='10.1.1.4')
    h3 = net.addHost('h3', ip='10.1.1.5')
    h4 = net.addHost('h4', ip='10.1.1.6')
    h5 = net.addHost('h5', ip='10.1.1.7')
    h6 = net.addHost('h6', ip='10.1.1.8')
    h7 = net.addHost('h7', ip='10.1.1.9')
    h8 = net.addHost('h8', ip='10.1.1.10')
    h9 = net.addHost('h9', ip='10.1.1.11')
    h10 = net.addHost('h10', ip='10.1.1.12')
    h11 = net.addHost('h11', ip='10.1.1.13')
    h12 = net.addHost('h12', ip='10.1.1.14')

    info( '*** Add links\n')
    net.addLink(s1, s2)
    net.addLink(s1, s3)
    net.addLink(s2, s5)
    net.addLink(s2, s4)
    net.addLink(s2, s6)
    net.addLink(s3, s7)
    net.addLink(s3, s8)
    net.addLink(s3, s9)
    net.addLink(s4, h1)
    net.addLink(s4, h2)
    net.addLink(h3, s5)
    net.addLink(h4, s5)
    net.addLink(h5, s6)
    net.addLink(h6, s6)
    net.addLink(h7, s7)
    net.addLink(h8, s7)
    net.addLink(h9, s8)
    net.addLink(h10, s8)
    net.addLink(h11, s9)
    net.addLink(h12, s9)

    info( '*** Starting network\n')
    nat0 = net.addNAT('nat0', ip=natIP , localIntf='eth0' ,inNamespace=False)
    net.addLink(nat0 , s1)
    #net.addNAT()
    net.build()

    info( '*** Starting controllers\n')
    c0.start()

    info( '*** Starting switches\n')
    s1.start([c0])
    s2.start([c0])
    s3.start([c0])
    s4.start([c0])
    s5.start([c0])
    s6.start([c0])
    s7.start([c0])
    s8.start([c0])
    s9.start([c0])

    info( '*** Post configure switches and hosts\n')

    CLI(net)
    net.stop()


if __name__ == '__main__':
    myNetwork()