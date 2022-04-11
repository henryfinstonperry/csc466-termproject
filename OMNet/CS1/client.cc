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
    bool clients[4];
public:
    d_serv();
    virtual ~d_serv();
protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void sendMessageToAll(MyPacket *msg);
    virtual void sendWorldUpdates(MyPacket *msg);
    virtual void updateClientList(const char *client_name);

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
        sendMessageToAll(msg);
        scheduleAt(simTime()+tick_rate, tick);
    }
}

void d_serv::sendMessageToAll(MyPacket *msg)
{
    for (int i=0; i< gateSize("out"); i++)
    {
        MyPacket *copy = msg->dup();
        send(copy, "out", i);

    }
}

void d_serv::sendWorldUpdates(MyPacket *msg)
{
    for (int i=0; i< gateSize("out"); i++)
    {
        if(clients[i]){
            MyPacket *copy = msg->dup();
            send(copy, "out", i);
        }
    }
    for (int i = 0; i < sizeof(clients); i++)
    {
        clients[i] = false;
    }
}

void d_serv::updateClientList(const char *client_name)
{
    bubble(client_name);
    if(client_name == std::string("client1"))
    {
        clients[0] = true;
    }
    else if (client_name == std::string("client2"))
    {
        clients[1] = true;
    }
    else if (client_name == std::string("client3"))
    {
        clients[2] = true;
    }
    else if (client_name == std::string("client4"))
    {
        clients[3] = true;
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

        std::string temp = "world snapshot @ tick" + std::to_string(counter);
        const char *str = temp.c_str();
        MyPacket *newMsg = new MyPacket(str);
        if(counter % 5 == 0){
            sendMessageToAll(newMsg);
        } else {
            sendWorldUpdates(newMsg);
        }
        delete newMsg;
        scheduleAt(simTime()+tick_rate, tick);
    }
    if (packet_holder->hasBitError())
    {
        EV << "client message has bit error\n";
    }
    else
    {
        EV << "client message received from client: " << msg->getName() << "\n";
        updateClientList(msg->getName());
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
    double packet_drop_rate;
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
    if(par("packet_drop").doubleValue() == true){
        packet_drop_rate = double(par("packet_drop"));
    }
    player_input_timer = uniform(0,1);
    player_input = new MyPacket(getName());
    scheduleAt(simTime()+player_input_timer, player_input);
}

void client::handleMessage(cMessage *msg)
{
    MyPacket *packet_holder = (MyPacket *)msg;
    if (msg == player_input){
            MyPacket *newMsg = new MyPacket(getName());
            if (uniform(0, 1) < packet_drop_rate) {
                        newMsg->setBitError(true);
             }
            send(newMsg, "out");
            player_input_timer = uniform(0.1,1);
            scheduleAt(simTime()+player_input_timer, player_input);
    }
    else if(packet_holder->hasBitError())
    {
            bubble("an incoming packet was lost");
    }
}


class node : public cSimpleModule
{
private:
    bool game_state;
    simtime_t user_changed_world_timer;
    MyPacket *user_changed_world;
    double packet_drop_rate;
    int state_counter;
protected:
    virtual void flip_game_state();
    virtual void handleMessage(cMessage *msg) override;
    virtual void initialize() override;
    virtual void sendMessages(MyPacket *msg);

public:
    node();
    virtual ~node();
};

Define_Module(node);

node::node()
{
    user_changed_world = nullptr;
}

node::~node()
{
    cancelAndDelete(user_changed_world);
}

void node::initialize()
{
    state_counter = 0;
    if(par("packet_drop").doubleValue() == true)
    {
        packet_drop_rate = double(par("packet_drop"));
    }
    user_changed_world_timer = uniform(2,5);
    user_changed_world = new MyPacket("self");
    game_state = false;
    scheduleAt(simTime()+user_changed_world_timer, user_changed_world);
}

void node::flip_game_state()
{
    game_state = !game_state;
}

void node::sendMessages(MyPacket *msg)
{
    for (int i=0; i< gateSize("out"); i++)
    {
        MyPacket *copy = msg->dup();
        send(copy, "out", i);
    }
}

void node::handleMessage(cMessage *msg)
{
    MyPacket *packet_holder = (MyPacket *)msg;
    if(msg == user_changed_world)
    {
        flip_game_state();
        state_counter++;
        MyPacket *newMsg = nullptr;
        if(game_state){
            newMsg = new MyPacket("true");
        }else{
            newMsg = new MyPacket("false");
        }
        newMsg->setSomeField(state_counter);
        if (uniform(0, 1) < packet_drop_rate)
        {
            newMsg->setBitError(true);
        }
        sendMessages(newMsg);
        scheduleAt(simTime()+user_changed_world_timer, user_changed_world);
    }
    else if(packet_holder->hasBitError())
    {
        bubble("an incoming packet lost");
    }
    else
    {
        int other_state = packet_holder->getSomeField();
        if(other_state > state_counter){
            sendMessages(packet_holder);
            state_counter = other_state;

            const char *msg_game_state = msg->getName();
            if(msg_game_state == std::string("true"))
            {
                game_state = true;
                bubble("game state: true.");
                EV << getName() << " state: " << state_counter <<"\n";
            }
            else if (msg_game_state == std::string("false"))
            {
                game_state = false;
                bubble("game state: false.");
                EV << getName() << " state: " << state_counter <<"\n";
            }
            else
            {
                bubble("received packet with bad name");
            }
        }
    }
}

