#include <iostream>

#include "ChatInterfaces.h"
#include "Chat.h"

int main(int argc, char *argv[])
{
    std::cout << "Server started" << std::endl;
    // server
    std::thread serverThread(
        []
        {
            io_context serverIoContext;
            Chat chat;

            IServer* server = createServer(serverIoContext, chat, 1234);
            server->execute();
        });
    serverThread.join();
    std::cout << "Server ended" << std::endl;
}
