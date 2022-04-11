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

channel channelA_same_country extends ned.DatarateChannel
{
    double distance @unit(km) = 450km;
    datarate = 80Mbps;
    double latency @unit(us) = 89us;
    delay = this.distance / 299792km * 1s + latency;
}

channel channelB_same_country extends ned.DatarateChannel
{
    double distance @unit(km) = 2000km;
    datarate = 40Mbps;
    double latency @unit(us) = 39us;
    delay = this.distance / 299792km * 1s + latency;
}

channel channelC_same_country extends ned.DatarateChannel
{
    double distance @unit(km) = 3500km;
    datarate = 105Mbps;
    double latency @unit(us) = 66us;
    delay = this.distance / 299792km * 1s + latency;
}

channel channelA_international extends ned.DatarateChannel
{
    double distance @unit(km) = 450km;
    datarate = 80Mbps;
    double latency @unit(us) = 89us;
    delay = this.distance / 299792km * 1s + latency;
}

channel channelB_international extends ned.DatarateChannel
{
    double distance @unit(km) = 5500km;
    datarate = 120Mbps;
    double latency @unit(us) = 199us;
    delay = this.distance / 299792km * 1s + latency;
}

channel channelC_international extends ned.DatarateChannel
{
    double distance @unit(km) = 11000km;
    datarate = 50Mbps;
    double latency @unit(us) = 66us;
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
            @display("p=177.592,490.064");
        }
        client2: client {
            @display("p=177.592,355.184");
        }
        client3: client {
            @display("p=221.428,239.412");
        }
        client4: client {
            @display("p=283.248,151.73999");
        }
        client5: client {
            @display("p=381.03598,80.928");
        }
        client6: client {
            @display("p=510.296,73.06");
        }
        client7: client {
            @display("p=690.136,73.06");
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
        Dedicated_server.out++ --> channelC_same_city --> client5.in;
        Dedicated_server.in++ <-- channelC_same_city <-- client5.out;
        Dedicated_server.out++ --> channelC_same_city --> client6.in;
        Dedicated_server.in++ <-- channelC_same_city <-- client6.out;
        Dedicated_server.out++ --> channelC_same_city --> client7.in;
        Dedicated_server.in++ <-- channelC_same_city <-- client7.out;
}

network CS2
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
            @display("p=249.528,336.076");
        }
        client3: client {
            @display("p=249.528,232.668");
        }
        client4: client {
            @display("p=324.836,116.895996");
        }
        client5: client {
            @display("p=434.988,73.06");
        }
        client6: client {
            @display("p=554.132,87.672");
        }
        client7: client {
            @display("p=657.54,73.06");
        }
    connections:
        Dedicated_server.out++ --> channelA_same_state --> client1.in;
        Dedicated_server.in++ <-- channelA_same_state <-- client1.out;
        Dedicated_server.out++ --> channelB_same_state --> client2.in;
        Dedicated_server.in++ <-- channelB_same_state <-- client2.out;
        Dedicated_server.out++ --> channelC_same_state --> client3.in;
        Dedicated_server.in++ <-- channelC_same_state <-- client3.out;
        Dedicated_server.out++ --> channelC_same_state --> client4.in;
        Dedicated_server.in++ <-- channelC_same_state <-- client4.out;
        Dedicated_server.out++ --> channelC_same_state --> client5.in;
        Dedicated_server.in++ <-- channelC_same_state <-- client5.out;
        Dedicated_server.out++ --> channelC_same_state --> client6.in;
        Dedicated_server.in++ <-- channelC_same_state <-- client6.out;
        Dedicated_server.out++ --> channelC_same_state --> client7.in;
        Dedicated_server.in++ <-- channelC_same_state <-- client7.out;
}

network CS3
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
        Dedicated_server.out++ --> channelA_same_country --> client1.in;
        Dedicated_server.in++ <-- channelA_same_country <-- client1.out;
        Dedicated_server.out++ --> channelB_same_country --> client2.in;
        Dedicated_server.in++ <-- channelB_same_country <-- client2.out;
        Dedicated_server.out++ --> channelC_same_country --> client3.in;
        Dedicated_server.in++ <-- channelC_same_country <-- client3.out;
        Dedicated_server.out++ --> channelC_same_country --> client4.in;
        Dedicated_server.in++ <-- channelC_same_country <-- client4.out;
}


simple node
{
    parameters:
        double packet_drop = default(0);
        int id;
    gates:
        input in[];
        output out[];
}


network UP1
{
    @display("bgb=781,558");
    submodules:
        node1: node {
            @display("p=101.159996,336.076");
            id = 0;
        }
        node2: node {
            @display("p=213.56,180.964");
            id = 1;
        }
        node3: node {
            @display("p=225.924,411.384");
            id = 2;
        }
        node4: node {
            @display("p=356.30798,96.664");
            id = 3;
        }
        node5: node {
            @display("p=501.304,180.964");
            id = 5;
        }
        node6: node {
            @display("p=479.948,411.384");
            id = 6;
        }
        node7: node {
            @display("p=641.804,300.108");
            id = 7;
        }
    connections:
        node1.out++ --> channelA_same_city --> node2.in++;
        node1.out++ --> channelA_same_city --> node3.in++;
        node2.out++ --> channelA_same_city --> node1.in++;
        node2.out++ --> channelC_same_city --> node4.in++;
        node2.out++ --> channelC_same_city --> node3.in++;
        node3.out++ --> channelA_same_city --> node1.in++;
        node3.out++ --> channelC_same_city --> node2.in++;
        node3.out++ --> channelB_same_city --> node5.in++;
        node3.out++ --> channelB_same_city --> node6.in++;
        node4.out++ --> channelC_same_city --> node2.in++;
        node4.out++ --> channelA_same_city --> node5.in++;
        node5.out++ --> channelA_same_city --> node4.in++;
        node5.out++ --> channelB_same_city --> node3.in++;
        node5.out++ --> channelA_same_city --> node6.in++;
        node5.out++ --> channelA_same_city --> node7.in++;
        node6.out++ --> channelB_same_city --> node3.in++;
        node6.out++ --> channelA_same_city --> node5.in++;
        node6.out++ --> channelA_same_city --> node7.in++;
        node7.out++ --> channelA_same_city --> node6.in++;
        node7.out++ --> channelA_same_city --> node5.in++;

}

network UP2
{
    @display("bgb=781,558");
    submodules:
        node1: node {
            @display("p=101.159996,336.076");
            id = 0;
        }
        node2: node {
            @display("p=213.56,180.964");
            id = 1;
        }
        node3: node {
            @display("p=225.924,411.384");
            id = 2;
        }
        node4: node {
            @display("p=356.30798,96.664");
            id = 3;
        }
        node5: node {
            @display("p=501.304,180.964");
            id = 5;
        }
        node6: node {
            @display("p=479.948,411.384");
            id = 6;
        }
        node7: node {
            @display("p=641.804,300.108");
            id = 7;
        }
    connections:
        node1.out++ --> channelA_same_state --> node2.in++;
        node1.out++ --> channelB_same_state --> node3.in++;
        node2.out++ --> channelA_same_state --> node1.in++;
        node2.out++ --> channelC_same_state --> node4.in++;
        node2.out++ --> channelC_same_state --> node3.in++;
        node3.out++ --> channelA_same_state --> node1.in++;
        node3.out++ --> channelC_same_state --> node2.in++;
        node3.out++ --> channelB_same_state --> node5.in++;
        node3.out++ --> channelB_same_state --> node6.in++;
        node4.out++ --> channelC_same_state --> node2.in++;
        node4.out++ --> channelA_same_state --> node5.in++;
        node5.out++ --> channelA_same_state --> node4.in++;
        node5.out++ --> channelB_same_state --> node3.in++;
        node5.out++ --> channelA_same_state --> node6.in++;
        node5.out++ --> channelA_same_state --> node7.in++;
        node6.out++ --> channelB_same_state --> node3.in++;
        node6.out++ --> channelA_same_state --> node5.in++;
        node6.out++ --> channelA_same_state --> node7.in++;
        node7.out++ --> channelA_same_state --> node6.in++;
}

network UP4
{
    @display("bgb=781,558");
    submodules:
        node1: node {
            @display("p=101.159996,336.076");
            id = 0;
        }
        node2: node {
            @display("p=213.56,180.964");
            id = 1;
        }
        node3: node {
            @display("p=225.924,411.384");
            id = 2;
        }
        node4: node {
            @display("p=356.30798,96.664");
            id = 3;
        }
        node5: node {
            @display("p=501.304,180.964");
            id = 5;
        }
        node6: node {
            @display("p=479.948,411.384");
            id = 6;
        }
        node7: node {
            @display("p=641.804,300.108");
            id = 7;
        }
    connections:
        node1.out++ --> channelA_international --> node2.in++;
        node1.out++ --> channelB_international --> node3.in++;
        node2.out++ --> channelA_international --> node1.in++;
        node2.out++ --> channelC_international --> node4.in++;
        node2.out++ --> channelC_international --> node3.in++;
        node3.out++ --> channelA_international --> node1.in++;
        node3.out++ --> channelC_international --> node2.in++;
        node3.out++ --> channelB_international --> node5.in++;
        node3.out++ --> channelB_international --> node6.in++;
        node4.out++ --> channelC_international --> node2.in++;
        node4.out++ --> channelA_international --> node5.in++;
        node5.out++ --> channelA_international --> node4.in++;
        node5.out++ --> channelB_international --> node3.in++;
        node5.out++ --> channelA_international --> node6.in++;
        node5.out++ --> channelA_international --> node7.in++;
        node6.out++ --> channelB_international --> node3.in++;
        node6.out++ --> channelA_international --> node5.in++;
        node6.out++ --> channelA_international --> node7.in++;
        node7.out++ --> channelA_international --> node6.in++;
}

