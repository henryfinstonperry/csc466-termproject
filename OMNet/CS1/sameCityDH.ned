simple client
{
    parameters:
        bool sendMsgOnInit = default(false);
        int limit = default(2);
    gates:
        input in;
        output out;
}

simple client1 extends client
{
    parameters:
        sendMsgOnInit = true;
}

simple client2 extends client
{
    parameters:
        sendMsgOnInit = false;
}
//
// Two instances (clients and toc) of Txc1 connected both ways.
// clients and toc will pass messages to one another.
//
network CS1DH
{
    @display("bgb=781,558");
    submodules:
        tic: client1;
        toc: client2;
    connections:
        tic.out --> {  delay = 100ms; } --> toc.in;
        tic.in <-- {  delay = 100ms; } <-- toc.out;
}
