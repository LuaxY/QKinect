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

    ui->lineIP->setAlignment(Qt::AlignHCenter);

    drawZone = new DrawZone(ui->centralWidget, this);
    drawZone->setGeometry(QRect(300, 0, 640, 480));

    kinect = new QKinect(this, drawZone);

    drawTimer = new QTimer(this);
    connect(drawTimer, SIGNAL(timeout()), this, SLOT(sltDrawSkeleton()));

    connect(ui->btnStopAll, SIGNAL(clicked()), this, SLOT(sltStopAll()));
    connect(ui->btnKinectStart, SIGNAL(clicked()), this, SLOT(sltKinectStart()));
    connect(ui->btnKinectStop, SIGNAL(clicked()), this, SLOT(sltKinectStop()));
    connect(ui->isSendTrames, SIGNAL(toggled(bool)), this, SLOT(sltChangeSendTrames(bool)));

    //QCoreApplication::postEvent(this, new CustomEvent<StreamEvent>());

    ui->statusBar->showMessage("Module NUI initialiser");
    drawSkeleton = true;
    drawTimer->start(40);
}

Programme::~Programme()
{
    sltKinectStop();
    delete kinect;
    delete ui;
}

bool Programme::event(QEvent* event)
{
    if (event->type() == CustomEvent<StopAll>::eventType)
    {
        CustomEvent<StopAll>* e = static_cast<CustomEvent<StopAll>*>(event);
        qDebug() << "Stop all Event";
        ui->statusBar->showMessage("ARRET D'URGENCE");
        drawSkeleton = false;
        kinect->stop();
        drawTimer->stop();

        return true;
    }

    /*if (event->type() == CustomEvent<SelectZoneEvent>::eventType)
    {
        CustomEvent<SelectZoneEvent>* e = static_cast<CustomEvent<SelectZoneEvent>*>(event);
        qDebug() << "Event SelectZone";

        QRect rect = drawZone->zones[drawZone->current].rect;
        ui->HAND_LEFT_START->setText(QString("[%1;%2|%3;%4]").arg(rect.x()).arg(rect.y()).arg(rect.width()).arg(rect.height()));

        return true;
    }*/

    return QWidget::event(event);
}

void Programme::sltDrawSkeleton()
{
    if(drawSkeleton)
    {
        drawZone->update();
    }
}

void Programme::sltStopAll()
{
    QCoreApplication::postEvent(this, new CustomEvent<StopAll>());
}

void Programme::sltDrawZone()
{
    /*if(ui->HAND_LEFT_ACTIVE->isChecked())
    {
        drawZone->addZone("hand_left");
    }*/
}

void Programme::sltKinectStart()
{
    //drawSkeleton = true;
    kinect->start();
    ui->statusBar->showMessage("Kinect lancee");
}

void Programme::sltKinectStop()
{
    //drawSkeleton = false;
    kinect->stop();
    ui->statusBar->showMessage("Kinect arrêtee");
}

void Programme::sltChangeSendTrames(bool actived)
{
    drawZone->SendTrames(actived);
}
