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

#define CASE_SIZE 10

typedef struct t_zone
{
    bool active;
    QRect rect;
} Zone;

typedef struct t_jointure
{
    NUI_SKELETON_POSITION_INDEX pos1;
    NUI_SKELETON_POSITION_INDEX pos2;
} Jointure;

typedef struct t_skeleton
{
    Qt::GlobalColor color;
    bool isSet;
    int untracked;
    NUI_SKELETON_DATA data;
    Jointure jointures[19];
} Skeleton;

class DrawZone : public QWidget
{
    Q_OBJECT
public:
    explicit DrawZone(QWidget *parent = 0, QWidget *main = 0);
    bool addZone(std::string);
    QPoint validCase();
    void SetSkeleton(int id, NUI_SKELETON_DATA data);
    void DrawBone(int id, int joint, NUI_SKELETON_POSITION_INDEX pos1, NUI_SKELETON_POSITION_INDEX pos2);
    void RemoveSkeleton(int id);

    // mutex
    void lock();
    void unlock();

    std::string current;
    std::map<std::string, t_zone> zones;

private:
    int posX;
    int posY;
    int directionX;
    int directionY;
    QWidget* mMain;
    Skeleton skeleton[5];
    QMutex mutex;

    // event
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);

    void hightlight(QPainter& painter, int posX, int posY);
    QPoint lissage(int x, int y);
    QPoint adjustSize(Vector4 skeletonPoint);
};

#endif // DRAWZONE_H
