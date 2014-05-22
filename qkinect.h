#ifndef QKINECT_H
#define QKINECT_H

#include <QThread>
#include <NuiApi.h>
#include "drawzone.h"

#define WIDTH 640
#define HEIGHT 480

class QKinect : public QThread
{
    Q_OBJECT
public:
    explicit QKinect(QObject *parent = 0, DrawZone* drawZone = 0);

protected:
    void run();

private:
    bool isRunning;
    bool isInitialized;

    NUI_SKELETON_FRAME mSkeletonFrame;
    DrawZone* mDrawZone;

    void init();
    void tracking();

signals:

public slots:
    void start();
    void stop();
};

#endif // QKINECT_H
