#ifndef CUSTOMEVENT_H
#define CUSTOMEVENT_H

#include <QEvent>

enum EventNames { StreamEvent };

template<EventNames E>
class CustomEvent : public QEvent
{
public:
    CustomEvent() :
        QEvent(registeredType())
        //QEvent((QEvent::Type)2000)
    {
    }

    static QEvent::Type eventType;

private:
    static QEvent::Type registeredType()
    {
        if (eventType == QEvent::None)
        {
            int generatedType = QEvent::registerEventType();
            eventType = static_cast<QEvent::Type>(generatedType);
        }

        return eventType;
    }

};

QEvent::Type CustomEvent<StreamEvent>::eventType = QEvent::None;

#endif // CUSTOMEVENT_H
