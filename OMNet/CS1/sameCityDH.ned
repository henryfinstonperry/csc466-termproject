

simple client1
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
    delay = 100us;
}

simple client2
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
        node0: client1 {
            parameters:
                sendMsgOnInit = true;
                @display("p=701.376,461.964");
        }
        node1: client2 {
            @display("p=446.228,478.824");
        }
        node2: client2 {
            @display("p=481.072,338.324");
        }
        node3: client2 {
            @display("p=701.376,249.528");
        }
        node4: client2 {
            @display("p=555.256,219.18");
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
