#pragma once

#include <QString>
#include <QByteArray>
#include <QPoint>
#include <QVector>

#include "enums.h"
#include "transport.h"
#include "CardData.h"

struct StringMessage : MessageBase{
    QString str;

    explicit StringMessage(const QString& str)
            : MessageBase(STRING_MESSAGE), str(str) {}

    QByteArray encodeMessage() const override {
        return createM(type, str.toUtf8());
    }

    explicit StringMessage(const QByteArray& data)
            : MessageBase(STRING_MESSAGE) {
        str = QString(data);
    }
};

struct SetClientNameMessage : MessageBase{
    QString name;

    explicit SetClientNameMessage(const QString& name)
            : MessageBase(SET_CLIENT_NAME_MESSAGE), name(name) {}

    QByteArray encodeMessage() const override {
        return createM(type, name.toUtf8());
    }

    explicit SetClientNameMessage(const QByteArray& data)
            : MessageBase(SET_CLIENT_NAME_MESSAGE) {
        name = QString(data);
    }
};

struct PlaceCardMessage : MessageBase{
    uint32_t uid;

    explicit PlaceCardMessage(uint32_t uid)
            : MessageBase(PLACE_CARD_MESSAGE), uid(uid) {}

    QByteArray encodeMessage() const override {
        return createM(type, encode((uint32_t)uid));
    }

    explicit PlaceCardMessage(const QByteArray& data)
            : MessageBase(PLACE_CARD_MESSAGE) {
        QByteArray m = data;

        uid = decodeAndShift(m);
    }
};

struct CardPlacedMessage : MessageBase{
    uint32_t uid;
    Card::Data card;

    CardPlacedMessage(uint32_t uid, const Card::Data& card)
            : MessageBase(CARD_PLACED_MESSAGE), uid(uid), card(card) {}

    QByteArray encodeMessage() const override {
        QByteArray array;

        array.append(encode((uint32_t)uid));

        array.append(card.asByte);

        return createM(type, array);
    }

    explicit CardPlacedMessage(const QByteArray& data)
            : MessageBase(CARD_PLACED_MESSAGE) {
        QByteArray m = data;

        uid = decodeAndShift(m);

        card.asByte = (uint8_t)m[0];
        m = m.mid(1);
    }
};

struct CardAddedMessage : MessageBase{
    uint32_t uid;
    Card::Data card;

    CardAddedMessage(uint32_t uid, const Card::Data& card)
            : MessageBase(CARD_ADDED_MESSAGE), uid(uid), card(card) {}

    QByteArray encodeMessage() const override {
        QByteArray array;

        array.append(encode((uint32_t)uid));

        array.append(card.asByte);

        return createM(type, array);
    }

    explicit CardAddedMessage(const QByteArray& data)
            : MessageBase(CARD_ADDED_MESSAGE) {
        QByteArray m = data;

        uid = decodeAndShift(m);

        card.asByte = (uint8_t)m[0];
        m = m.mid(1);
    }
};
