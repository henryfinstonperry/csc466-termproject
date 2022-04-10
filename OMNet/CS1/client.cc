#include <string.h>
#include <stdio.h>
#include <omnetpp.h>
#include <MyPacket_m.h>


using namespace omnetpp;

class d_serv : public cSimpleModule
{
private:
    simtime_t tick_rate;
    MyPacket *tick;
    int counter;

public:
    d_serv();
    virtual ~d_serv();
protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void sendMessage(MyPacket *msg);

};

// The module class needs to be registered with OMNeT++

Define_Module(d_serv);

d_serv::d_serv()
{
    tick = nullptr;
}

d_serv::~d_serv()
{
    cancelAndDelete(tick);
}

void d_serv::initialize()
{
    counter = 0;
    tick_rate = 0.0015;
    tick = new MyPacket("tick");


    if(par("sendMsgOnInit").boolValue() == true) {
        MyPacket *msg = new MyPacket("initial world snapshot");
        sendMessage(msg);
        scheduleAt(simTime()+tick_rate, tick);
    }
}

void d_serv::sendMessage(MyPacket *msg)
{
    for (int i=0; i< gateSize("out"); i++)
    {
        MyPacket *copy = msg->dup();
        send(copy, "out", i);

    }
}

void d_serv::handleMessage(cMessage *msg)
{

    MyPacket *packet_holder;
    packet_holder = new MyPacket("empty");

    packet_holder = (MyPacket *)msg;

    if (msg == tick)
    {
        counter++;
        EV << "tick " << counter << ", update object states and check if clients need an updated snapshot\n";
        std::string temp = "world snapshot @ tick" + std::to_string(counter);
        const char *str = temp.c_str();
        MyPacket *newMsg = new MyPacket(str);
        sendMessage(newMsg);
        scheduleAt(simTime()+tick_rate, tick);
    }

    if (packet_holder->hasBitError())
    {
        bubble("client message has bit error");
        MyPacket *newMsg = new MyPacket("clientMsg");
        sendMessage(newMsg);
        //scheduleAt(simTime()+tick_rate, tick);
    }
    else
    {
        EV << "client message received.\n";
    }
}





/**
 * Sends back an acknowledgement -- or not.
 */
class client : public cSimpleModule
{
  protected:
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(client);

void client::handleMessage(cMessage *msg)
{
    MyPacket *packet_holder = (MyPacket *)msg;
    if(packet_holder->hasBitError())
    {
            EV <<"bit Error\n";
    }
    if (uniform(0, 1) < 0.3) {
        EV << "\"Losing\" message.\n";
        bubble("message lost");  // making animation more informative...
        delete msg;
    }
    else if (uniform(0, 1) > 0.6) {
            EV << "\"Bit Error\"\n";
            packet_holder->setBitError(true);
        }
    else {
        EV << "Sending back same message as acknowledgement.\n";
        send(msg, "out");
    }
}




