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
    int game_state;
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
    virtual void sendToClient(const char *client_name, MyPacket *msg);
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
    game_state = 0;


    if(par("sendMsgOnInit").boolValue() == true) {
        MyPacket *msg = new MyPacket("initial world snapshot");
        msg->setGame_state(game_state);
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

void d_serv::sendToClient(const char *client_name, MyPacket *msg)
{
    if(client_name == std::string("client1"))
    {
        send(msg, "out", 0);
    }
    else if (client_name == std::string("client2"))
    {
        send(msg, "out", 1);
    }
    else if (client_name == std::string("client3"))
    {
        send(msg, "out", 2);
    }
    else if (client_name == std::string("client4"))
    {
        send(msg, "out", 3);
    }
}

void d_serv::handleMessage(cMessage *msg)
{
    MyPacket *packet_holder;
    packet_holder = new MyPacket("empty");
    packet_holder = (MyPacket *)msg;

    if (msg == tick)
    {
        game_state++;
        counter++;
        std::string temp = "world snapshot @ tick" + std::to_string(counter);
        const char *str = temp.c_str();
        MyPacket *newMsg = new MyPacket(str);
        newMsg->setGame_state(game_state);
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
        const char *msgName = msg->getName();
        EV << msgName <<"\n";
        if(msgName == std::string("ack")){
            EV <<"ack recieved";
        }
        else
        {
            updateClientList(msg->getName());
            MyPacket *ack = new MyPacket("ack");
            ack->setStartTime(packet_holder->getStartTime());
            sendToClient(msg->getName(), ack);
        }
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

    cLongHistogram rTTStats;
    cOutVector rTTVector;

    double packet_drop_rate;
    int game_state;
  protected:
    virtual void handleMessage(cMessage *msg) override;
    virtual void initialize() override;
    virtual void finish() override;
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
    game_state = 0;
    if(par("packet_drop").doubleValue() == true){
        packet_drop_rate = double(par("packet_drop"));
    }
    player_input_timer = uniform(0.1,1);
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
            newMsg->setStartTime(simTime());
            send(newMsg, "out");
            player_input_timer = uniform(0.1,1);
            scheduleAt(simTime()+player_input_timer, player_input);
    }
    else if(packet_holder->hasBitError())
    {
            bubble("an incoming packet was lost");
    }
    else {
        const char *pkt_name = packet_holder->getName();
        if(pkt_name == std::string("ack"))
        {
            simtime_t trip_time = (packet_holder->getArrivalTime() - packet_holder->getStartTime()) * 1000;
            rTTVector.record(trip_time);
            rTTStats.collect(trip_time);
        }
        else
        {
            //recieved update from server. Update state, and send ack response
            game_state = packet_holder->getGame_state();
            MyPacket *newMsg = new MyPacket("ack");
            newMsg->setStartTime(packet_holder->getSendingTime());
            send(newMsg, "out");
        }
    }
}

void client::finish(){
        EV << "rtt, min:    " << rTTStats.getMin() << endl;
        EV << "rtt, max:    " << rTTStats.getMax() << endl;
        EV << "rtt, mean:   " << rTTStats.getMean() << endl;
        EV << "rtt, stddev: " << rTTStats.getStddev() << endl;


        rTTStats.recordAs("rtt");
}


class node : public cSimpleModule
{
private:
    long numSent;
    long numReceived;
    int expecting_acks;
    //cLongHistogram hopCountStats;
    //cOutVector hopCountVector;
    cLongHistogram rTTStats;
    cOutVector rTTVector;

    bool game_state;
    simtime_t user_changed_world_timer;
    MyPacket *user_changed_world;
    double packet_drop_rate;
    int state_counter;
    int id;

    simtime_t delay_timer;
    MyPacket *delay_msg;

protected:
    virtual void flip_game_state();
    virtual void handleMessage(cMessage *msg) override;
    virtual void initialize() override;
    virtual void sendMessages(MyPacket *msg);
    virtual void finish() override;

public:
    node();
    virtual ~node();
};

Define_Module(node);

node::node()
{
    user_changed_world = nullptr;
    delay_msg = nullptr;
}

node::~node()
{
    cancelAndDelete(user_changed_world);
    cancelAndDelete(delay_msg);
}

void node::initialize()
{
    expecting_acks = 0;
    numSent = 0;
    numReceived = 0;
    WATCH(numSent);
    WATCH(numReceived);
    id = par("id");
    state_counter = 0;
    if(par("packet_drop").doubleValue() == true)
    {
        packet_drop_rate = double(par("packet_drop"));
    }
    user_changed_world_timer = uniform(0.1,1);
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
        numSent++;
        MyPacket *copy = msg->dup();
        send(copy, "out", i);
    }
}


void node::handleMessage(cMessage *msg)
{
    numReceived++;
    MyPacket *packet_holder = (MyPacket *)msg;
    //"user input", send update to network
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

        newMsg->setGame_state(state_counter);
        newMsg->setHopcount(0);
        newMsg->setSrcAddr(id);
        newMsg->setStartTime(simTime());
        if (uniform(0, 1) < packet_drop_rate) newMsg->setBitError(true);
        expecting_acks = gateSize("out");
        sendMessages(newMsg);

        //schedule next "user input"
        user_changed_world_timer = uniform(0.1,1);
        scheduleAt(simTime()+user_changed_world_timer, user_changed_world);
    }
    else if(packet_holder->hasBitError())
    {
        bubble("an incoming packet lost");
    }
    else
    {
        if(packet_holder->getName() == std::string("ack")){
            if(expecting_acks > 0){
                expecting_acks--;
                bubble("ack");
                simtime_t end = packet_holder->getArrivalTime();
                simtime_t start = packet_holder->getStartTime();
                simtime_t rtt = (end - start) * 1000;
                rTTVector.record(rtt);
                rTTStats.collect(rtt);
            }
        }
        else
        {
            int other_state = packet_holder->getGame_state();
            int hopcount = packet_holder->getHopcount();
            if(other_state > state_counter){
                // build ACK message
                //send ack back to sender
                MyPacket *ack = new MyPacket("ack");
                //if this is the first hop, get start time from source
                if(hopcount < 1){
                    packet_holder->setStartTime(packet_holder->getSendingTime());
                }
                //setStart time for ack to start time from og message
                ack->setStartTime(packet_holder->getStartTime());
                ack->setLastHopAddr(id);
                sendMessages(ack);



                //hop count tracking for stats
                EV <<"hopcount: " << hopcount << "\n";
                //hopCountVector.record(hopcount);
                //hopCountStats.collect(hopcount);
                hopcount++;
                packet_holder->setHopcount(hopcount);

                //send packets to neighbors

                sendMessages(packet_holder);

                //update clients state version
                state_counter = other_state;


                //get and set game state
                const char *msg_game_state = msg->getName();
                if(msg_game_state == std::string("true"))
                {
                    game_state = true;
                    //bubble("game state: true.");
                    EV << getName() << " state: " << state_counter <<"\n";
                }
                else if (msg_game_state == std::string("false"))
                {
                    game_state = false;
                    //bubble("game state: false.");
                    EV << getName() << " state: " << state_counter <<"\n";
                }
                else
                {
                    //should never reach here
                    bubble("received packet with bad state");
                }
            } // else ignore message because it is less up to date than the current node
        }
    }
}

void node::finish(){
        EV << "Sent:     " << numSent << endl;
        EV << "Received: " << numReceived << endl;
        //EV << "Hop count, min:    " << hopCountStats.getMin() << endl;
        //EV << "Hop count, max:    " << hopCountStats.getMax() << endl;
        //EV << "Hop count, mean:   " << hopCountStats.getMean() << endl;
        //EV << "Hop count, stddev: " << hopCountStats.getStddev() << endl;
        EV << "rtt, min:    " << rTTStats.getMin() << endl;
        EV << "rtt, max:    " << rTTStats.getMax() << endl;
        EV << "rtt, mean:   " << rTTStats.getMean() << endl;
        EV << "rtt, stddev: " << rTTStats.getStddev() << endl;

        recordScalar("#sent", numSent);
        recordScalar("#received", numReceived);

        //hopCountStats.recordAs("hop count");
        rTTStats.recordAs("rtt");
}




