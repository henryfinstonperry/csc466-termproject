simple Txc1
{
    gates:
        input in;
        output out;
}

channel C extends ned.DatarateChannel
{
     datarate = 100Mbps;
     delay = 100us;
     ber = 0.5;    
}

//
// Two instances (tic and toc) of Txc1 connected both ways.
// Tic and toc will pass messages to one another.
//
network Tictoc1
{
    submodules:
        tic: Txc1;
        toc: Txc1;
    connections:
        tic.out --> C --> toc.in;
        tic.in <-- C <-- toc.out;
}