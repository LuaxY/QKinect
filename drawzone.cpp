#include "drawzone.h"
#include "customevent.h"
#include <QCoreApplication>

#include <QDebug>

DrawZone::DrawZone(QWidget *parent, QWidget *main) :
    QWidget(parent),
    mMain(main)
{
    posX = 320;
    posY = 290;
    directionX = 2;
    directionY = 1;
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
    painter.setPen(QPen(Qt::red, 3));
    painter.drawLine(QPoint(320, 240), QPoint(posX, posY));

    painter.setPen(QPen(Qt::green, 2));
    painter.drawEllipse(QPoint(320, 240), 5, 5);
    painter.drawEllipse(QPoint(posX, posY), 5, 5);

    if (directionX == 1)
    {
        posX++;

        if (posX >= 370)
            directionX = 2;
    }
    else
    {
        posX--;

        if (posX <= 270)
            directionX = 1;
    }

    if (directionY == 1)
    {
        posY++;

        if (posY >= 290)
            directionY = 2;
    }
    else
    {
        posY--;

        if (posY <= 190)
            directionY = 1;
    }
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
