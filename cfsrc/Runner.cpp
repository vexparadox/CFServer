#include "Runner.hpp"
Runner::Runner(Server* s){
    //if the server is false
    if(s){
        this->server = s;
    }else{
        std::cout << "Incorrect Server creation, making a default..." << std::endl;
        this->server = new Server();
    }
    this->run.store(true);
    std::thread inputThread(&Runner::takeInput, this);
    //loop for ever!
    //detect connects and events
    while(run.load()){
        ENetEvent event;
        //wait upto half a second for an event
        while (enet_host_service (server->getHost(), &event, 500) > 0)
        {
            server->sendPacket();
            switch (event.type)
            {
            case ENET_EVENT_TYPE_CONNECT:
                std::cout << "A new client connected from " << event.peer->address.host << event.peer->address.port << std::endl; 
                /* Store any relevant client information here. */

                break;
            case ENET_EVENT_TYPE_RECEIVE:
                std::cout << "A packet of length "<<event.packet->dataLength <<" containing " << event.packet->data <<" was received from " <<event.peer->data<< "on channel "<<event.channelID << "." << std::endl;
                enet_packet_destroy (event.packet);
                
                break;
            
            case ENET_EVENT_TYPE_DISCONNECT:
                std::cout << "Someone disconnected" << std::endl;
                /* Reset the peer's client information. */
                event.peer -> data = NULL;
            }
        }
    }
    run.store(false);
    inputThread.join();
}

void Runner::takeInput(){
    std::string buffer;
    while (run.load()){
        std::cin >> buffer;
        if(buffers == "help"){
            std::cout << "exit - safely close the server" << std::endl;
        }else if (buffer == "exit"){
            run.store(false);
        }else{
            std::cout << "Not a recognised command." << std::endl;
        }
    }

}

Runner::~Runner(){
    enet_deinitialize();
}