#include "ClientMainWindow.h"
#include "transport.h"
#include "enums.h"
#include "messages.h"
#include <QInputDialog>

ClientMainWindow::ClientMainWindow()
        : client(new QTcpSocket(this))
{
    client->setObjectName("socket");
    setupUi(this);
    show();
}

void ClientMainWindow::on_socket_readyRead() {
    reader.processBytes(client->readAll(), {
            {STRING_MESSAGE, [this](const QByteArray& message){
                StringMessage m(message);
                textEdit->append(m.str);
            }},
            {SET_CLIENT_NAME_MESSAGE, [this](const QByteArray& message){
                SetClientNameMessage m(message);
                setWindowTitle("Name is " + m.name);
            }},
    });
}

void ClientMainWindow::on_socket_connected() {
    buttonSend->setEnabled(true);
    printf("Connected\n");
}

void ClientMainWindow::on_socket_error(QAbstractSocket::SocketError error) {
    printf("Errrrrrror: %d\n", error);
}

void ClientMainWindow::on_socket_stateChanged(QAbstractSocket::SocketState state) {
    printf("Staaaaaate: %d\n", state);
    switch (state){
        case QAbstractSocket::UnconnectedState:
            buttonSend->setEnabled(false);
            buttonConnect->setEnabled(true);
            break;
        default:
            break;
    }
}

void ClientMainWindow::on_canvas_debugInfo(int linesCount, int paintTime) {
//    static TextProgress p(20);
//    debug->setText(QString("linesCount: %1, paintTime: %2 ms |%3|").arg(linesCount).arg(paintTime).arg(p.get()));
}


bool ClientMainWindow::isConnected() {
    return buttonSend->isEnabled();
}

template<typename MsgClass, typename... ArgTypes>
void ClientMainWindow::sendMessage(ArgTypes... args) {
    if(!isConnected()){
        return;
    }
    client->write(MsgClass(args...).encodeMessage());
    client->flush();
}

void ClientMainWindow::on_buttonSend_clicked(){
    if(lineEdit->text().isEmpty()){
        return;
    }

    sendMessage<StringMessage>(lineEdit->text());
    lineEdit->setText("");
}

void ClientMainWindow::on_buttonConnect_clicked(){
    buttonConnect->setEnabled(false);
    client->connectToHost("localhost", 9000);
}
