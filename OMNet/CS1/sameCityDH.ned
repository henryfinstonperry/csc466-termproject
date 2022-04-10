

simple d_serv
{
    parameters:
        bool sendMsgOnInit = default(false);
        bool clients;
    gates:
        input in[];
        output out[];
}

channel channelA extends ned.DatarateChannel
{
    datarate = 100Mbps;
    delay = 5us;    
}

channel channelB extends ned.DatarateChannel
{
    datarate = 100Mbps;
    delay = 5us;    
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
        Dedicated_server.out++ --> channelC --> client1.in;
        Dedicated_server.in++ <-- channelC <-- client1.out;
        Dedicated_server.out++ --> channelC --> client2.in;
        Dedicated_server.in++ <-- channelC <-- client2.out;
        Dedicated_server.out++ --> channelC --> client3.in;
        Dedicated_server.in++ <-- channelC <-- client3.out;
        Dedicated_server.out++ --> channelC --> client4.in;
        Dedicated_server.in++ <-- channelC <-- client4.out;
}
