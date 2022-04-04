#include <string.h>
#include <stdio.h>
#include <omnetpp.h>


using namespace omnetpp;

class client1 : public cSimpleModule
{
private:
    simtime_t timeout;
    cPacket *timeoutEvent;

public:
    client1();
    virtual ~client1();
protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void sendMessage(cMessage *msg);

};

// The module class needs to be registered with OMNeT++

Define_Module(client1);

client1::client1()
{
    timeoutEvent = nullptr;
}

client1::~client1()
{
    cancelAndDelete(timeoutEvent);
}

void client1::initialize()
{
    timeout = 1.0;
    timeoutEvent = new cPacket("timeoutEvent");


    if(par("sendMsgOnInit").boolValue() == true) {
        cPacket *msg = new cPacket("clientMsg");
        sendMessage(msg);
        scheduleAt(simTime()+timeout, timeoutEvent);
    }
}

void client1::sendMessage(cMessage *msg)
{
    for (int i=0; i< gateSize("out"); i++)
    {
        cMessage *copy = msg->dup();
        send(copy, "out", i);
    }
}

void client1::handleMessage(cMessage *msg)
{
    cPacket *packet_holder;
    packet_holder = new cPacket("empty");
    if(msg->isPacket()){
        packet_holder = (cPacket *)msg;
    }
    if (msg == timeoutEvent)
    {
        EV << "Timeout expired, resending message and restarting timer\n";
        cPacket *newMsg = new cPacket("clientMsg");
        sendMessage(newMsg);
        scheduleAt(simTime()+timeout, timeoutEvent);
    }
    else if (packet_holder->hasBitError())
    {
        EV << "Ack has bit error, resending message\n";
        cPacket *newMsg = new cPacket("clientMsg");
        sendMessage(newMsg);
        scheduleAt(simTime()+timeout, timeoutEvent);
    }
    // message arrived
    // Acknowledgement received -- delete the received message and cancel
    // the timeout event.
    else
    {
        EV << "Timer cancelled.\n";
        cancelEvent(timeoutEvent);
        delete msg;

        cPacket *newMsg = new cPacket("clientMsg");
        sendMessage(msg);
        scheduleAt(simTime()+timeout, timeoutEvent);
    }
}




/**
 * Sends back an acknowledgement -- or not.
 */
class client2 : public cSimpleModule
{
  protected:
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(client2);

void client2::handleMessage(cMessage *msg)
{
    cPacket *packet_holder = (cPacket *)msg;
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
            bubble("Bit error");  // making animation more informative...
            packet_holder->setBitError(true);
        }
    else {
        EV << "Sending back same message as acknowledgement.\n";
        send(msg, "out");
    }
}




