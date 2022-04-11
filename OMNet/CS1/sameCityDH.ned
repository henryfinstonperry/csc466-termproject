simple d_serv
{
    parameters:
        bool sendMsgOnInit = default(false);
    gates:
        input in[];
        output out[];
}

channel channelA_same_city extends ned.DatarateChannel
{
    double distance @unit(km) = 2km;
	datarate = 100Mbps;
    double latency @unit(us) = 9us;
    delay = this.distance / 299792km * 1s + latency;
}

channel channelB_same_city extends ned.DatarateChannel
{
    double distance @unit(km) = 25km;
    datarate = 120Mbps;
    double latency @unit(us) = 5us;
    delay = this.distance / 299792km * 1s + latency;
}

channel channelC_same_city extends ned.DatarateChannel
{
    double distance @unit(km) = 40km;
    datarate = 105Mbps;
    double latency @unit(us) = 7us;
    delay = this.distance / 299792km * 1s + latency;  
}

channel channelA_same_state extends ned.DatarateChannel
{
    double distance @unit(km) = 250km;
	datarate = 100Mbps;
    double latency @unit(us) = 9us;
    delay = this.distance / 299792km * 1s + latency;
}

channel channelB_same_state extends ned.DatarateChannel
{
    double distance @unit(km) = 400km;
    datarate = 120Mbps;
    double latency @unit(us) = 5us;
    delay = this.distance / 299792km * 1s + latency;
}

channel channelC_same_state extends ned.DatarateChannel
{
    double distance @unit(km) = 40km;
    datarate = 105Mbps;
    double latency @unit(us) = 7us;
    delay = this.distance / 299792km * 1s + latency;  
}





simple client
{
    parameters:
        bool sendMsgOnInit = default(false);
        double packet_drop = default(0); //defined in omnetpp.ini
    gates:
        input in;
        output out;
}


network CS1
{
    @display("bgb=781,558");
    submodules:
        Dedicated_server: d_serv {
            parameters:
                sendMsgOnInit = true;
                @display("p=701.376,461.964");
        }
        client1: client {
            @display("p=249.528,461.964");
        }
        client2: client {
            @display("p=320.34,236.04");
        }
        client3: client {
            @display("p=701.376,75.308");
        }
        client4: client {
            @display("p=483.31998,75.308");
        }
    connections:
        Dedicated_server.out++ --> channelA_same_city --> client1.in;
        Dedicated_server.in++ <-- channelA_same_city <-- client1.out;
        Dedicated_server.out++ --> channelB_same_city --> client2.in;
        Dedicated_server.in++ <-- channelB_same_city <-- client2.out;
        Dedicated_server.out++ --> channelC_same_city --> client3.in;
        Dedicated_server.in++ <-- channelC_same_city <-- client3.out;
        Dedicated_server.out++ --> channelC_same_city --> client4.in;
        Dedicated_server.in++ <-- channelC_same_city <-- client4.out;
}

simple node
{
    parameters:
        double packet_drop = default(0);
    gates:
        input in[];
        output out[];
}


network UP1
{
    @display("bgb=781,558");
    submodules:
        node1: node {
            @display("p=400.14398,83.175995");
            
        }
        node2: node {
            @display("p=187.708,245.032");
        }
        node3: node {
            @display("p=566.496,245.032");
        }
        node4: node {
            @display("p=400.14398,436.112");
        }
        node5: node {
            @display("p=393.4,251.776");
        }
    connections:
        node1.out++ --> channelA_same_city --> node2.in++;
        node1.out++ --> channelB_same_city --> node3.in++;
        node1.out++ --> channelC_same_city --> node5.in++;
        node2.out++ --> channelA_same_city --> node1.in++;
        node2.out++ --> channelC_same_city --> node4.in++;
        node2.out++ --> channelC_same_city --> node5.in++;
        node3.out++ --> channelA_same_city --> node1.in++;
        node3.out++ --> channelC_same_city --> node4.in++;
        node3.out++ --> channelB_same_city --> node5.in++;
        node4.out++ --> channelC_same_city --> node2.in++;
        node4.out++ --> channelA_same_city --> node3.in++;
        node4.out++ --> channelC_same_city --> node5.in++;
        node5.out++ --> channelA_same_city --> node1.in++;
        node5.out++ --> channelB_same_city --> node2.in++;
        node5.out++ --> channelA_same_city --> node3.in++;
        node5.out++ --> channelA_same_city --> node4.in++;

}

network UP2
{
    @display("bgb=781,558");
    submodules:
        node1: node {
            @display("p=400.14398,83.175995");
        }
        node2: node {
            @display("p=187.708,245.032");
        }
        node3: node {
            @display("p=566.496,245.032");
        }
        node4: node {
            @display("p=400.14398,436.112");
        }
        node5: node {
            @display("p=393.4,251.776");
        }
    connections:
        node1.out++ --> channelA_same_state --> node2.in++;
        node1.out++ --> channelB_same_state --> node3.in++;
        node1.out++ --> channelC_same_state --> node5.in++;
        node2.out++ --> channelA_same_state --> node1.in++;
        node2.out++ --> channelC_same_state --> node4.in++;
        node2.out++ --> channelC_same_state --> node5.in++;
        node3.out++ --> channelA_same_state --> node1.in++;
        node3.out++ --> channelC_same_state --> node4.in++;
        node3.out++ --> channelB_same_state --> node5.in++;
        node4.out++ --> channelC_same_state --> node2.in++;
        node4.out++ --> channelA_same_state --> node3.in++;
        node4.out++ --> channelC_same_state --> node5.in++;
        node5.out++ --> channelA_same_state --> node1.in++;
        node5.out++ --> channelB_same_state --> node2.in++;
        node5.out++ --> channelA_same_state --> node3.in++;
        node5.out++ --> channelA_same_state --> node4.in++;

}

