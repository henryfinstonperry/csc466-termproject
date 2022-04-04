#include <string.h>
#include <stdio.h>
#include <omnetpp.h>

using namespace omnetpp;

class client1 : public cSimpleModule
{
private:
    simtime_t timeout;
    cMessage *timeoutEvent;

public:
    client1();
    virtual ~client1();
protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
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
    timeoutEvent = new cMessage("timeoutEvent");


    if(par("sendMsgOnInit").boolValue() == true) {
        cMessage *msg = new cMessage("clientMsg");
        send(msg, "out");
        scheduleAt(simTime()+timeout, timeoutEvent);
    }
}


void client1::handleMessage(cMessage *msg)
{

    if (msg == timeoutEvent)
    {
        EV << "Timeout expired, resending message and restarting timer\n";
        cMessage *newMsg = new cMessage("clientMsg");
        send(newMsg, "out");
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

        cMessage *newMsg = new cMessage("clientMsg");
        send(newMsg, "out");
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
    if (uniform(0, 1) < 0.3) {
        EV << "\"Losing\" message.\n";
        bubble("message lost");  // making animation more informative...
        delete msg;
    }
    else {
        EV << "Sending back same message as acknowledgement.\n";
        send(msg, "out");
    }
}




