#ifndef CREATECHATROOM_H
#define CREATECHATROOM_H

#include <QDialog>

class QChatClient;

namespace Ui {
class CreateChatRoom;
}

class CreateChatRoom : public QDialog
{
    Q_OBJECT

public:
    explicit CreateChatRoom(QChatClient& chatClient,QWidget *parent = nullptr);
    ~CreateChatRoom();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::CreateChatRoom *ui;
    QChatClient& m_chatClient;
};

#endif // CREATECHATROOM_H
