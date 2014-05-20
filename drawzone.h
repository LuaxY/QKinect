#ifndef DRAWZONE_H
#define DRAWZONE_H

#include <QWidget>
#include <QPainter>
#include <vector>

class DrawZone : public QWidget
{
    Q_OBJECT
public:
    explicit DrawZone(QWidget *parent = 0);
    bool addRect(float x, float y, float w, float h);

private:
    void paintEvent(QPaintEvent*);
    int posX;
    int posY;
    int directionX;
    int directionY;

    QVector<QRect> rectangles;

    void hightlight(QPainter& painter, int posX, int posY);
};

#endif // DRAWZONE_H
