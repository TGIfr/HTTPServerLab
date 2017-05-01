#include <iostream>
#include <time.h>

#include <progbase-cpp/net.h>
#include <sstream>
#include <vector>
#include <iterator>
#include "include/processor.h"


using namespace std;
using namespace progbase::net;


int main(int argc, char * argv[]) {
    vector<Chemical> chemicals;
    chemicals.push_back(Chemical("Oxygen", 16, "Oxydyzer"));
    chemicals.push_back(Chemical("Lysergsäurediethylamid", 323.4, "Psycho"));
    chemicals.push_back(Chemical("DihidrohenaMonooksid", 18, "Solvent"));
    chemicals.push_back(Chemical("tetrahydrocannabinol", 314, "Psycho"));
    chemicals.push_back(Chemical("methylcarbinol", 46, "Depressant"));

    const int serverPort = 8006;
    TcpListener listener;
    NetMessage message(10000);
    try {
        listener.bind(IpAddress("127.0.0.1", serverPort));
        listener.start();
        while (true) {
            cout << ">> Waiting for clients at " << serverPort << "..." << endl;
            TcpClient * client = listener.accept();
            client->receive(message);
            cout << ">> Received: " << endl << message.dataAsString() << endl;
            std::string data = message.dataAsString();
            auto tokens = getTokens(data);
            int status = analyzeTokens(tokens);
            string backMessage = makeResponse(tokens, status, chemicals);
            cout << "RESOPNSE \n\n" << backMessage;
            message.setDataString(backMessage);
            client->send(message);
            cout << ">> Response sent." << endl;
            delete client;
        }
    } catch(NetException const & exc) {
        cerr << exc.what() << endl;
    }

}


