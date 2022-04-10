simple d_serv
{
    parameters:
        bool sendMsgOnInit = default(false);
    gates:
        input in[];
        output out[];
}

channel channelA extends ned.DatarateChannel
{
    datarate = 100Mbps;
    delay = 10us;    
}

channel channelB extends ned.DatarateChannel
{
    datarate = 110Mbps;
    delay = 7us;    
}

channel channelC extends ned.DatarateChannel
{
    datarate = 120Mbps;
    delay = 5us;    
}



simple client
{
    parameters:
        bool sendMsgOnInit = default(false);
        double packet_drop = default(0);
    gates:
        input in;
        output out;
}


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
        Dedicated_server.out++ --> channelA --> client1.in;
        Dedicated_server.in++ <-- channelA <-- client1.out;
        Dedicated_server.out++ --> channelB --> client2.in;
        Dedicated_server.in++ <-- channelB <-- client2.out;
        Dedicated_server.out++ --> channelC --> client3.in;
        Dedicated_server.in++ <-- channelC <-- client3.out;
        Dedicated_server.out++ --> channelC --> client4.in;
        Dedicated_server.in++ <-- channelC <-- client4.out;
}
