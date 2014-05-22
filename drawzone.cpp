#include "drawzone.h"
#include "customevent.h"
#include <QCoreApplication>

#include <QDebug>

DrawZone::DrawZone(QWidget *parent, QWidget *main) :
    QWidget(parent),
    mMain(main)
{
    for(unsigned int i = 0; i < 5; i++)
    {
        skeleton[i].color = Qt::red;
        skeleton[i].isSet = false;
        skeleton[i].untracked = 0;
    }

    skeleton[0].color = Qt::blue;
    skeleton[1].color = Qt::magenta;
    skeleton[2].color = Qt::cyan;
    skeleton[3].color = Qt::yellow;
}

void DrawZone::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(QRect(0, 0, width(), height()), Qt::black);

    /* DRAW GRID */

    /*painter.setPen(QPen(Qt::darkGray, 1));

    for(int x = 0; x <= 640; x += CASE_SIZE)
    {
        for(int y = 0; y <= 480; y += CASE_SIZE)
        {
            painter.drawRect(QRect(x, y, CASE_SIZE, CASE_SIZE));
        }
    }*/

    /* DRAW HIGHTLIGHT RECT */

    hightlight(painter, posX, posY);
    QPoint mouse = this->mapFromGlobal(QCursor::pos());
    hightlight(painter, mouse.x(), mouse.y());
    painter.setBrush(Qt::NoBrush);

    /* DRAW SKELETON */

    lock();

    for(unsigned int i = 0; i < 5; i++)
    {
        if(!skeleton[i].isSet)
            continue;

        if(skeleton[i].untracked > 0)
        {
            skeleton[i].untracked++;
            if(skeleton[i].untracked > 20)
            {
                skeleton[i].isSet = false;
                continue;
            }
        }

        for(unsigned int j = 0; j < 19; j++)
        {
            Jointure joint = skeleton[i].jointures[j];

            if(skeleton[i].data.eSkeletonPositionTrackingState[joint.pos1] == NUI_SKELETON_NOT_TRACKED || skeleton[i].data.eSkeletonPositionTrackingState[joint.pos2] == NUI_SKELETON_NOT_TRACKED)
                continue;

            QPoint pos1 = adjustSize(skeleton[i].data.SkeletonPositions[joint.pos1]);
            QPoint pos2 = adjustSize(skeleton[i].data.SkeletonPositions[joint.pos2]);

            if(skeleton[i].data.eSkeletonPositionTrackingState[joint.pos1] == NUI_SKELETON_POSITION_TRACKED || skeleton[i].data.eSkeletonPositionTrackingState[joint.pos2] == NUI_SKELETON_POSITION_TRACKED)
                painter.setPen(QPen(skeleton[i].color, 3));
            else
                painter.setPen(QPen(Qt::gray, 1));

            painter.drawLine(pos1, pos2);

            if(skeleton[i].data.eSkeletonPositionTrackingState[joint.pos1] == NUI_SKELETON_POSITION_TRACKED || skeleton[i].data.eSkeletonPositionTrackingState[joint.pos2] == NUI_SKELETON_POSITION_TRACKED)
                painter.setPen(QPen(Qt::green, 1));
            else
                painter.setPen(QPen(Qt::darkGray, 1));

            painter.drawEllipse(pos1, 2, 2);
            painter.drawEllipse(pos2, 2, 2);
        }

        QPoint headPos = adjustSize(skeleton[i].data.SkeletonPositions[NUI_SKELETON_POSITION_HEAD]);
        QPoint leftPos = adjustSize(skeleton[i].data.SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT]);
        QPoint rightPos = adjustSize(skeleton[i].data.SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT]);

        painter.setPen(QPen(Qt::red, 5));
        painter.setBrush(Qt::white);

        painter.drawEllipse(headPos, 30, 30);
        painter.drawEllipse(leftPos, 10, 10);
        painter.drawEllipse(rightPos, 10, 10);
    }

    unlock();
}

void DrawZone::mousePressEvent(QMouseEvent* e)
{
    QRect rect = zones[current].rect;

    if(rect.x() == -1)
    {
        QPoint point = lissage(e->pos().x(), e->pos().y());
        zones[current].rect = QRect(point.x(), point.y(), -1, -1);
    }
    else if(rect.width() == -1)
    {
        QPoint point = lissage(e->pos().x(), e->pos().y());
        zones[current].rect = QRect(rect.x(), rect.y(), point.x() - rect.x(), point.y() - rect.y());
        QCoreApplication::postEvent(mMain, new CustomEvent<SelectZoneEvent>());
    }
}

bool DrawZone::addZone(std::string name)
{
    t_zone zone = {true, QRect(-1, -1, -1, -1)};
    zones[name] = zone;
    current = name;
    return true;
}

void DrawZone::hightlight(QPainter& painter, int posX, int posY)
{
    for(int x = 0; x <= this->width(); x += CASE_SIZE)
    {
        if(posX >= x && posX < x + CASE_SIZE)
        {
            for(int y = 0; y <= this->height(); y += CASE_SIZE)
            {
                if(posY >= y && posY < y + CASE_SIZE)
                {
                    painter.setPen(QPen(Qt::blue, 2));
                    painter.setBrush(Qt::white);
                    painter.drawRect(QRect(x, y, CASE_SIZE, CASE_SIZE));
                }
            }
        }
    }
}

QPoint DrawZone::lissage(int posX, int posY)
{
    QPoint point(-1, -1);

    for(int x = 0; x <= this->width(); x += CASE_SIZE)
    {
        if(posX >= x && posX < x + CASE_SIZE)
        {
            point.setX(x);

            for(int y = 0; y <= this->height(); y += CASE_SIZE)
            {
                if(posY >= y && posY < y + CASE_SIZE)
                {
                    point.setY(y);
                }
            }
        }
    }

    return point;
}

QPoint DrawZone::adjustSize(Vector4 skeletonPoint)
{
    LONG x, y;
    USHORT depth;

    NuiTransformSkeletonToDepthImage(skeletonPoint, &x, &y, &depth);

    float pointX = static_cast<float>(x * this->width()) / 320;
    float pointY = static_cast<float>(y * this->height()) / 240;

    return QPoint(pointX, pointY);
}

void DrawZone::SetSkeleton(int id, NUI_SKELETON_DATA data)
{
    if(id >= 5)
        id = 0;

    skeleton[id].data = data;
}

void DrawZone::DrawBone(int id, int joint, NUI_SKELETON_POSITION_INDEX pos1, NUI_SKELETON_POSITION_INDEX pos2)
{
    if(id >= 5)
        id = 0;

    skeleton[id].jointures[joint].pos1 = pos1;
    skeleton[id].jointures[joint].pos2 = pos2;

    skeleton[id].isSet = true;
    skeleton[id].untracked = 0;
}

void DrawZone::RemoveSkeleton(int id)
{
    if(id >= 5)
        id = 0;

    skeleton[id].untracked++;
}

void DrawZone::lock()
{
    mutex.lock();
}

void DrawZone::unlock()
{
    mutex.unlock();
}
