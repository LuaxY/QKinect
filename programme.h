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
    QTimer* test;
    QKinect* kinect;

    bool event(QEvent* event);

private slots:
    void sltDrawSkeleton();
    void sltActiveStream();
    void sltDrawZone();
    void sltKinectStart();
    void sltKinectStop();
};

#endif // PROGRAMME_H
