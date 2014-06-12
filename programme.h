#ifndef PROGRAMME_H
#define PROGRAMME_H

#include <QMainWindow>
#include <QPainter>
#include <QWidget>
#include <QTimer>
#include <QMouseEvent>

#include "drawzone.h"
#include "qkinect.h"

namespace Ui {
class Programme;
}

class Programme : public QMainWindow
{
    Q_OBJECT

public:
    explicit Programme(QWidget *parent = 0);
    ~Programme();

private:
    Ui::Programme *ui;
    DrawZone* drawZone;
    bool drawSkeleton;
    QTimer* drawTimer;
    QKinect* kinect;

    bool event(QEvent* event);

private slots:
    void sltDrawSkeleton();
    void sltStopAll();
    void sltDrawZone();
    void sltKinectStart();
    void sltKinectStop();
    void sltChangeSendTrames(bool actived);
};

#endif // PROGRAMME_H
