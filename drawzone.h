#ifndef DRAWZONE_H
#define DRAWZONE_H

#include <QWidget>
#include <QPainter>
#include <string>
#include <map>
#include <QMouseEvent>
#include <NuiApi.h>
#include <vector>
#include <QMutex>

#include "TrameDMX.h"

typedef struct t_skeleton
{
    Qt::GlobalColor color;
    bool isSet;
    int untracked;
    NUI_SKELETON_DATA data;
} Skeleton;

class DrawZone : public QWidget
{
    Q_OBJECT
public:
    explicit DrawZone(QWidget *parent = 0, QWidget *main = 0);

    // functions
    void SetSkeleton(int id, NUI_SKELETON_DATA data);
    void RemoveSkeleton(int id);
    void SendTrames(bool actived);
    void ResetTrames();
    bool mIsDrawing;

    // mutex
    void lock();
    void unlock();

private:
    // variables
    QWidget* mMain;
    Skeleton mSkeleton[5];
    QMutex mMutex;
    TrameDMX* mTrame;

    bool mIsTrameActived;
    bool mSendTrames;

    // event
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);

    // functions
    QPoint AdjustSize(Vector4 skeletonPoint);
    void DrawJointure(QPainter& painter, int id, NUI_SKELETON_POSITION_INDEX posA, NUI_SKELETON_POSITION_INDEX posB);
};

#endif // DRAWZONE_H
