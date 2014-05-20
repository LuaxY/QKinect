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

    drawZone = new DrawZone(ui->centralWidget);
    drawZone->setGeometry(QRect(230, 0, 640, 480));

    test = new QTimer(this);
    connect(test, SIGNAL(timeout()), this, SLOT(sltDrawSkeleton()));

    connect(ui->btnActiveStream, SIGNAL(clicked()), this, SLOT(sltActiveStream()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(sltDrawZone()));

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
        test->start(10);

        return true;
    }

    return QWidget::event(event);
}

void Programme::paintEvent(QPaintEvent*)
{
    /*if (drawSkeleton)
    {
        QPainter painter(drawZone);
        QPen pen;
        pen.setColor(QColor("red"));

        painter.setPen(pen);
        painter.drawEllipse(QPoint(50, 50), 10, 20);
    }*/
}

void Programme::sltDrawSkeleton()
{
    if(drawSkeleton)
    {
        drawZone->update();
        test->start(10);
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
        drawZone->addRect(ui->HAND_LEFT_X->value(), ui->HAND_LEFT_Y->value(), 50, 50);
    }
}
