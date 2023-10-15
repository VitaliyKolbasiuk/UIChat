#include <iostream>

#include "ChatClient.h"
#include "Terminal.h"

int main(int argc, char *argv[])
{
    std::cout << "Client started" << std::endl;
        std::thread clientThread(
            []
            {
                ChatClient chatClient;
                Terminal terminal(chatClient);
                io_context ioContext1;
                TcpClient client1(ioContext1, chatClient);
                chatClient.setTcpClient(&client1);
                terminal.run();
                client1.execute("127.0.0.1", 1234);
                ioContext1.run();
            });
    clientThread.join();
    std::cout << "Client ended" << std::endl;
}
