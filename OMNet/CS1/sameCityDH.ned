simple client1
{
    parameters:
        bool sendMsgOnInit = default(false);
    gates:
        input in;
        output out;
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
        tic: client1 {
         	parameters:
         	    sendMsgOnInit = true;   
        }
        toc: client2;
    connections:
        tic.out --> {  delay = 100ms; } --> toc.in;
        tic.in <-- {  delay = 100ms; } <-- toc.out;
}
