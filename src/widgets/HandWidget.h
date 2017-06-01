#pragma once

#include <QDockWidget>
#include <QHash>

#include "ui_hand.h"
#include "CardData.h"

class CardWidget;
class QVBoxLayout;

class HandWidget : public QDockWidget, private Ui::HandWidget{
Q_OBJECT
private:
    QHash<uint32_t, CardWidget*> uidToCard;
    QVBoxLayout* getButtonsLayout();

public slots:
    void appendCard(uint32_t uid, Card::Data card);
    void deleteCard(uint32_t uid);

public:
    explicit HandWidget(QWidget* parent);
    void clearHand();

signals:
    void placeCardClicked(uint32_t uid);
};
