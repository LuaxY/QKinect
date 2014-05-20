#ifndef DRAWZONE_H
#define DRAWZONE_H

#include <QWidget>
#include <QPainter>
#include <string>
#include <map>
#include <tuple>
#include <QMouseEvent>

#define CASE_SIZE 10

typedef struct t_zone {
    bool active;
    QRect rect;
} Zone;

class DrawZone : public QWidget
{
    Q_OBJECT
public:
    explicit DrawZone(QWidget *parent = 0, QWidget *main = 0);
    bool addZone(std::string);
    QPoint validCase();

    std::string current;
    std::map<std::string, t_zone> zones;

private:
    int posX;
    int posY;
    int directionX;
    int directionY;
    QWidget* mMain;

    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);

    void hightlight(QPainter& painter, int posX, int posY);
    QPoint lissage(int x, int y);
};

#endif // DRAWZONE_H
