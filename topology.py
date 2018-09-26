from mininet.net import Mininet
from mininet.topo import Topo
from mininet.log import setLogLevel
from mininet.cli import CLI
from mininet.node import RemoteController, OVSKernelSwitch, UserSwitch


class SwitchesTopo(Topo):
    def __init__(self, **opts):
        # Initialize topology and default options
        Topo.__init__(self, **opts)

        s10 = self.addSwitch('s10', dpid='000000000000000A', protocols='OpenFlow13')
        s11 = self.addSwitch('s11', dpid='000000000000000B', protocols='OpenFlow13')
        s12 = self.addSwitch('s12', dpid='000000000000000C', protocols='OpenFlow13')
        s13 = self.addSwitch('s13', dpid='000000000000000D', protocols='OpenFlow13')
        s14 = self.addSwitch('s14', dpid='000000000000000E', protocols='OpenFlow13')
        s15 = self.addSwitch('s15', dpid='000000000000000F', protocols='OpenFlow13')

        h1 = self.addHost('h1', ip='10.0.0.1', mac='000000000001')
        h2 = self.addHost('h2', ip='10.0.0.2', mac='000000000002')
        h3 = self.addHost('h3', ip='10.0.0.3', mac='000000000003')

        self.addLink(s10, h1)
        self.addLink(s12, h2)
        self.addLink(s11, h3)

        '''self.addLink(s10, s13) #sa/2-sd/1
        self.addLink(s10, s13) #sa/3-sd/2
        self.addLink(s11, s14) #sb/2-se/1
        self.addLink(s11, s14) #sb/3-se/2
        self.addLink(s11, s14) #sb/4-se/3
        self.addLink(s12, s15) #sc/2-sf/1
        self.addLink(s12, s15) #sc/3-sf/3
        self.addLink(s13, s14) #sd/3-se/4
        self.addLink(s14, s15) #se/5-sf/3
        self.addLink(s13, s15) #sd/4-sf/4'''


def simpleTest():
    "Create and test a simple network"
    topo = SwitchesTopo()
    net = Mininet(topo, switch=OVSKernelSwitch, controller=RemoteController)
    c1 = RemoteController('c1', ip='192.168.56.102')
    net.addController(c1)
    net.staticArp()
    net.start()
    # print "Dumping host connections
    CLI(net)
    net.stop()


def vlanTest():
    "Create and test a simple network"
    topo = SwitchesTopo()
    net = Mininet(topo, switch=OVSKernelSwitch, controller=RemoteController)
    c1 =  RemoteController( 'c1', ip='192.168.56.1' )
    net.addController(c1)
    net.staticArp()
    net.start()
    #print "Dumping host connections"
    h1 = net.get('h1')
    h1.cmd('ifconfig h1-eth0 0')
    h1.cmd('vconfig add h1-eth0 10')
    h1.cmd('ifconfig h1-eth0.10 up')
    h1.cmd('ip addr add 10.0.0.1/8 brd + dev h1-eth0.10')

    h3 = net.get('h3')
    h3.cmd('ifconfig h3-eth0 0')
    h3.cmd('vconfig add h3-eth0 10')
    h3.cmd('ifconfig h3-eth0.10 up')
    h3.cmd('ip addr add 10.0.0.2/8 brd + dev h3-eth0.10')

    CLI(net)
    net.stop()


if __name__ == '__main__':
    # Tell mininet to print useful information
    setLogLevel('info')
    simpleTest()
