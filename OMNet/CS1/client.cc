#include <string.h>
#include <stdio.h>
#include <omnetpp.h>

using namespace omnetpp;

/**
 * Derive the client class from cSimpleModule. In the client1 network,
 * both the `tic' and `toc' modules are client objects, created by OMNeT++
 * at the beginning of the simulation.
 */
class client : public cSimpleModule
{
private:
    int counter;
    cMessage *event;  // pointer to the event object which we'll use for timing
    cMessage *clientMsg;  // variable to remember the message until we send it back

public:
    client();
    virtual ~client();
protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

// The module class needs to be registered with OMNeT++
Define_Module(client);

client::client()
{
    // Set the pointer to nullptr, so that the destructor won't crash
    // even if initialize() doesn't get called because of a runtime
    // error or user cancellation during the startup process.
    event = clientMsg = nullptr;
}

client::~client()
{
    // Dispose of dynamically allocated the objects
    cancelAndDelete(event);
    delete clientMsg;
}

void client::initialize()
{
    event = new cMessage("event");

    // No client message yet.
    clientMsg = nullptr;

    counter = par("limit");

    if(par("sendMsgOnInit").boolValue() == true) {
        cMessage *msg = new cMessage("clientMsg");
        send(msg, "out");
    }
}


void client::handleMessage(cMessage *msg)
{
    if (counter == 0)
    {
        delete msg;
    }
    else if (msg == event)
    {
        EV << "Wait period is over, sending back message\n";
        counter--;
        EV <<"Counter = " << counter << "\n";

        msg = clientMsg;
        send(msg, "out");

        clientMsg = nullptr;
    }
    else
    {
        EV << "Message arrived, starting to wait 1 sec...\n";
        clientMsg = msg;
        scheduleAt(simTime()+1.0, event);
    }
}




