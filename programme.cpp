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

    this->setFixedSize(this->width(), this->height());

    drawZone = new DrawZone(ui->centralWidget, this);
    drawZone->setGeometry(QRect(300, 0, 640, 480));

    kinect = new QKinect(this, drawZone);

    drawTimer = new QTimer(this);
    connect(drawTimer, SIGNAL(timeout()), this, SLOT(sltDrawSkeleton()));

    // boutons
    connect(ui->btnStopAll, SIGNAL(clicked()), this, SLOT(sltStopAll()));
    connect(ui->btnKinectStart, SIGNAL(clicked()), this, SLOT(sltKinectStart()));
    connect(ui->btnKinectStop, SIGNAL(clicked()), this, SLOT(sltKinectStop()));

    // actions
    connect(ui->actionQuitter, SIGNAL(triggered()), this, SLOT(sltCloseApp()));
    connect(ui->actionKinectStart, SIGNAL(triggered()), this, SLOT(sltKinectStart()));
    connect(ui->actionKinectStop, SIGNAL(triggered()), SLOT(sltKinectStop()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(sltDisplayAbout()));

    // contrôle
    connect(ui->isSendTrames, SIGNAL(toggled(bool)), this, SLOT(sltChangeSendTrames(bool)));
    connect(ui->btnResetTrames, SIGNAL(clicked()), this, SLOT(sltResetTrames()));

    //QCoreApplication::postEvent(this, new CustomEvent<StreamEvent>());

    ui->statusBar->showMessage("Module NUI initialisee");
    drawSkeleton = true;
    drawTimer->start(40);
}

Programme::~Programme()
{
    sltKinectStop();
    delete kinect;
    delete ui;
}

void Programme::closeEvent(QCloseEvent* event)
{
    sltCloseApp();
    event->ignore();
}

void Programme::sltCloseApp()
{
    QApplication::exit(0);
}

bool Programme::event(QEvent* event)
{
    if (event->type() == CustomEvent<StopAll>::eventType)
    {
        CustomEvent<StopAll>* e = static_cast<CustomEvent<StopAll>*>(event);
        qDebug() << "Stop all Event";
        ui->statusBar->showMessage("ARRET D'URGENCE");
        drawZone->mIsDrawing = false;
        drawZone->ResetTrames();
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
    drawZone->mIsDrawing = true;
    kinect->start();
    ui->statusBar->showMessage("Kinect lancee");
}

void Programme::sltKinectStop()
{
    //drawSkeleton = false;
    drawZone->mIsDrawing = false;
    kinect->stop();
    ui->statusBar->showMessage("Kinect arretee");
}

void Programme::sltChangeSendTrames(bool actived)
{
    drawZone->SendTrames(actived);
}

void Programme::sltResetTrames()
{
    drawZone->ResetTrames();
}

void Programme::sltDisplayAbout()
{
    aboutUi = new About();
    aboutUi->show();
}
