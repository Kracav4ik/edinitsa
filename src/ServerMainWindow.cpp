#include <QTime>
#include "ServerMainWindow.h"
#include "transport.h"
#include "enums.h"
#include "messages.h"

const uint32_t BEGIN_CARDS_COUNT = 7;

Card::Data getRandomCard(){
    return Card::ALL_CARDS[qrand() % Card::ALL_CARDS_COUNT];
}

void ServerMainWindow::acceptConnection() {
    printf("NEW CONNECTION ACCEPTED!!! \n");
    QTcpSocket* clientSocket = srv.nextPendingConnection();
    static int i = 1;
    QString name = QString("player %1").arg(i++);
    clients[clientSocket] = new ClientInfo(name);
    connect(clientSocket, SIGNAL(readyRead()),this, SLOT(readyToRead()));
    connect(clientSocket, SIGNAL(disconnected()),this, SLOT(clientDisconnected()));
    clientSocket->write(SetClientNameMessage(name).encodeMessage());
    for (int _ = 0; _ < BEGIN_CARDS_COUNT; ++_){
        Card::Data card = getRandomCard();
        sendCard(clientSocket, card);
    }
    clientSocket->flush();
}

ServerMainWindow::ServerMainWindow() {
    setupUi(this);
    connect(&srv, SIGNAL(newConnection()),this, SLOT(acceptConnection()));
    srv.listen(QHostAddress::Any, 9000);
    show();
}

HandlePair::CallbackType ServerMainWindow::multicastFunc(uint32_t messageType, QTcpSocket* ignoreSocket) {
    return [this, messageType, ignoreSocket](const QByteArray& message) {
        QByteArray answer = createM(messageType, message);
        for (QTcpSocket* clientSocket : clients.keys()) {
            if(clientSocket == ignoreSocket){
                continue;
            }
            clientSocket->write(answer);
            clientSocket->flush();
        }
    };
}

void ServerMainWindow::readyToRead() {
    QTcpSocket* socket = (QTcpSocket *) sender();
    int available = (int) socket->bytesAvailable();
    QByteArray data = socket->readAll();
    reader.processBytes(data, {
            {STRING_MESSAGE, [this, available, socket](const QByteArray& message) {
                StringMessage m(message);
                QString dataStr = m.str;
                printf("Got data: %i bytes\n%s\n", available, message.data());
                QString s = "[" + QTime::currentTime().toString() + "] <" + clients[socket]->name + "> " + dataStr;
                textEdit->append(s);

                QByteArray answer = StringMessage(s).encodeMessage();
                for (QTcpSocket* clientSocket : clients.keys()) {
                    clientSocket->write(answer);
                    clientSocket->flush();
                }
            }},
            {PLACE_CARD_MESSAGE, [this, socket](const QByteArray& message){
                PlaceCardMessage m(message);
                if(!cards.contains(m.uid)){
                    QString s = QString("card with uid %1 not found\n").arg(m.uid);
                    printf("%s", s.toUtf8().data());
                    socket->write(StringMessage(s).encodeMessage());
                    socket->flush();
                    return;
                }
                socket->write(CardPlacedMessage(m.uid, cards[m.uid]).encodeMessage());
                socket->flush();

                Card::Data card = getRandomCard();
                sendCard(socket, card);
            }},
    });

}

void ServerMainWindow::clientDisconnected() {
    QTcpSocket* socket = (QTcpSocket *) sender();
    delete clients.take(socket);
    socket->deleteLater();
}

void ServerMainWindow::sendCard(QTcpSocket* socket, Card::Data card) {
    cards[nextCard] = card;
    socket->write(CardAddedMessage(nextCard++, card).encodeMessage());
    socket->flush();
}
