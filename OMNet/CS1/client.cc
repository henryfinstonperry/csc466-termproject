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
    tick_rate = 0.25;
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
        EV << "client message has bit error\n";
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
private:
    simtime_t player_input_timer;
    MyPacket *player_input;
  protected:
    virtual void handleMessage(cMessage *msg) override;
    virtual void initialize() override;
  public:
      client();
      virtual ~client();
};

Define_Module(client);

client::client()
{
    player_input = nullptr;
}

client::~client()
{
    cancelAndDelete(player_input);
}

void client::initialize()
{
    player_input_timer = uniform(0,1);
    player_input = new MyPacket(getName());
    scheduleAt(simTime()+player_input_timer, player_input);
}

void client::handleMessage(cMessage *msg)
{
    MyPacket *packet_holder = (MyPacket *)msg;
    if (msg == player_input){
            MyPacket *newMsg = new MyPacket(getName());
            if (uniform(0, 1) < 0.002) {
                        newMsg->setBitError(true);
             }
            send(newMsg, "out");
            player_input_timer = uniform(0,1);
            scheduleAt(simTime()+player_input_timer, player_input);
    }
    else if(packet_holder->hasBitError())
    {
            bubble("bit Error");
    }

}




