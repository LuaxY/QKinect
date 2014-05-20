#include "drawzone.h"

DrawZone::DrawZone(QWidget *parent) :
    QWidget(parent)
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
    painter.setPen(QPen(Qt::gray, 1));

    for(int x = 0; x <= 640; x += 20)
    {
        for(int y = 0; y <= 480; y += 20)
        {
            painter.drawRect(QRect(x, y, 20, 20));
        }
    }

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

bool DrawZone::addRect(float x, float y, float w, float h)
{
    rectangles.push_back(QRect(x, y, w, h));
    return true;
}

void DrawZone::hightlight(QPainter& painter, int posX, int posY)
{
    for(int x = 0; x <= 640; x += 20)
    {
        if(posX >= x && posX < x+20)
        {
            for(int y = 0; y <= 480; y += 20)
            {
                if(posY >= y && posY < y+20)
                {
                    painter.setPen(QPen(Qt::blue, 2));
                    painter.setBrush(Qt::white);
                    painter.drawRect(QRect(x, y, 20, 20));
                }
            }
        }
    }
}
