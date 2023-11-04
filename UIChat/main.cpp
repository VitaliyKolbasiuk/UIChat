#include "mainwindow.h"
#include "Client/ChatClient.h"

#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
//    std::cout << "Client started" << std::endl;
//    std::thread clientThread(
//        []
//        {
//            ChatClient chatClient;
//            io_context ioContext1;
//            TcpClient client1(ioContext1, chatClient);
//            chatClient.setTcpClient(&client1);
//            client1.execute("127.0.0.1", 1234);
//            ioContext1.run();
//        });
//    clientThread.join();
//    std::cout << "Client ended" << std::endl;
    return a.exec();
}
