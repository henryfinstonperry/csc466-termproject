simple Txc1
{
    gates:
        input in;
        output out;
}

network Gamenet1
{
    submodules:
        p1: Txc1;
        p2: Txc1;
    connections:
        p1.out --> { delay = 100ms; } --> p2.in;
        p1.in <-- { delay = 100ms; } <-- p2.out;
}