#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Client/ChatClient.h"
#include "Client/TcpClient.h"

class Settings;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Settings* m_settings;
public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_SendMessage_clicked();

    void on_Join_released();

    void on_SaveSettings_released();

private:
    Ui::MainWindow *ui;

    std::shared_ptr<QChatClient> m_chatClient;
    io_context m_ioContext1;
    std::shared_ptr<TcpClient>  m_tcpClient;
};
#endif // MAINWINDOW_H
