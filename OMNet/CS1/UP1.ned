
simple node
{
    parameters:
        double packet_drop = default(0);
    gates:
        input in[];
        output out[];
}

channel channelD extends ned.DatarateChannel
{
    datarate = 100Mbps;
    delay = 10us;
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
        node1.out++ --> {  datarate = 100Mbps; delay = 10us; } --> node2.in++;
        node1.out++ --> {  datarate = 100Mbps; delay = 10us; } --> node3.in++;
        node1.out++ --> {  datarate = 100Mbps; delay = 10us; } --> node5.in++;
        node2.out++ --> {  datarate = 100Mbps; delay = 10us; } --> node1.in++;
        node2.out++ --> {  datarate = 100Mbps; delay = 10us; } --> node4.in++;
        node2.out++ --> {  datarate = 100Mbps; delay = 10us; } --> node5.in++;
        node3.out++ --> {  datarate = 100Mbps; delay = 10us; } --> node1.in++;
        node3.out++ --> {  datarate = 100Mbps; delay = 10us; } --> node4.in++;
        node3.out++ --> {  datarate = 100Mbps; delay = 10us; } --> node5.in++;
        node4.out++ --> {  datarate = 100Mbps; delay = 10us; } --> node2.in++;
        node4.out++ --> {  datarate = 100Mbps; delay = 10us; } --> node3.in++;
        node4.out++ --> {  datarate = 100Mbps; delay = 10us; } --> node5.in++;
        node5.out++ --> {  datarate = 100Mbps; delay = 10us; } --> node1.in++;
        node5.out++ --> {  datarate = 100Mbps; delay = 10us; } --> node2.in++;
        node5.out++ --> {  datarate = 100Mbps; delay = 10us; } --> node3.in++;
        node5.out++ --> {  datarate = 100Mbps; delay = 10us; } --> node4.in++;

}
