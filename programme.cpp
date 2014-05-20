#include "programme.h"
#include "ui_programme.h"
#include "customevent.h"

#include <QDebug>

Programme::Programme(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Programme),
    drawSkeleton(false)
{
    ui->setupUi(this);

    drawZone = new DrawZone(ui->centralWidget, this);
    drawZone->setGeometry(QRect(300, 0, 640, 480));

    test = new QTimer(this);
    connect(test, SIGNAL(timeout()), this, SLOT(sltDrawSkeleton()));

    connect(ui->btnActiveStream, SIGNAL(clicked()), this, SLOT(sltActiveStream()));
    connect(ui->HAND_LEFT_START, SIGNAL(clicked()), this, SLOT(sltDrawZone()));

    QCoreApplication::postEvent(this, new CustomEvent<StreamEvent>());
}

Programme::~Programme()
{
    delete ui;
}

bool Programme::event(QEvent* event)
{
    if (event->type() == CustomEvent<StreamEvent>::eventType)
    {
        CustomEvent<StreamEvent>* e = static_cast<CustomEvent<StreamEvent>*>(event);
        qDebug() << "Event Stream";
        drawSkeleton = !drawSkeleton;
        test->start(40);

        return true;
    }

    if (event->type() == CustomEvent<SelectZoneEvent>::eventType)
    {
        CustomEvent<SelectZoneEvent>* e = static_cast<CustomEvent<SelectZoneEvent>*>(event);
        qDebug() << "Event SelectZone";

        QRect rect = drawZone->zones[drawZone->current].rect;
        ui->HAND_LEFT_START->setText(QString("[%1;%2|%3;%4]").arg(rect.x()).arg(rect.y()).arg(rect.width()).arg(rect.height()));

        return true;
    }

    return QWidget::event(event);
}

void Programme::sltDrawSkeleton()
{
    if(drawSkeleton)
    {
        drawZone->update();
        test->start(40);
    }
}

void Programme::sltActiveStream()
{
    QCoreApplication::postEvent(this, new CustomEvent<StreamEvent>());
}

void Programme::sltDrawZone()
{
    if(ui->HAND_LEFT_ACTIVE->isChecked())
    {
        drawZone->addZone("hand_left");
    }
}
