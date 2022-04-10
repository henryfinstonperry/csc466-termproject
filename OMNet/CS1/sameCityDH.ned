

simple d_serv
{
    parameters:
        bool sendMsgOnInit = default(false);
    gates:
        input in[];
        output out[];
}

channel channelC extends ned.DatarateChannel
{
    datarate = 100Mbps;
    delay = 5us;
    
}

simple client
{
    parameters:
        bool sendMsgOnInit = default(false);
    gates:
        input in;
        output out;
}
//
// Two instances (clients and toc) of Txc1 connected both ways.
// clients and toc will pass messages to one another.
//
network CS1DH
{
    @display("bgb=781,558");
    submodules:
        node0: d_serv {
            parameters:
                sendMsgOnInit = true;
                @display("p=701.376,461.964");
        }
        node1: client {
            @display("p=249.528,461.964");
        }
        node2: client {
            @display("p=320.34,236.04");
        }
        node3: client {
            @display("p=701.376,75.308");
        }
        node4: client {
            @display("p=483.31998,75.308");
        }
    connections:
        node0.out++ --> channelC --> node1.in;
        node0.in++ <-- channelC <-- node1.out;
        node0.out++ --> channelC --> node2.in;
        node0.in++ <-- channelC <-- node2.out;
        node0.out++ --> channelC --> node3.in;
        node0.in++ <-- channelC <-- node3.out;
        node0.out++ --> channelC --> node4.in;
        node0.in++ <-- channelC <-- node4.out;
}
