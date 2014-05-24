#include "drawzone.h"
#include "customevent.h"
#include <QCoreApplication>

#include <QDebug>

/**
 * @brief DrawZone::DrawZone : constructeur
 * @param parent : classe parrent
 * @param main : IHM
 */
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

    // Affectation d'un couleur différente pour chaque squelette
    skeleton[0].color = Qt::blue;
    skeleton[1].color = Qt::magenta;
    skeleton[2].color = Qt::cyan;
    skeleton[3].color = Qt::yellow;
}

/**
 * @brief DrawZone::paintEvent : fonction event qui permet de dessiner
 */
void DrawZone::paintEvent(QPaintEvent*)
{
    // Préparation de la zone de dessin
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

    /* AFFICHAGE DES SQUELETTES */

    lock(); // mutex pour bloquer le vecteur de données des squelettes (thread safe)

    for(unsigned int i = 0; i < 5; i++)
    {
        // Si le squelettes n'a pas encore de données on passe
        if(!skeleton[i].isSet)
            continue;

        // Si le squelettes n'est plus traquer depuis 20 frames on le supprime
        if(skeleton[i].untracked > 0)
        {
            skeleton[i].untracked++;
            if(skeleton[i].untracked > 20)
            {
                skeleton[i].isSet = false;
                continue;
            }
        }

        // On dessine chaque jointure du squelette
        for(unsigned int j = 0; j < 19; j++)
        {
            Jointure joint = skeleton[i].jointures[j];

            // Si la position de l'un des deux point est inconnue, on passe
            if(skeleton[i].data.eSkeletonPositionTrackingState[joint.pos1] == NUI_SKELETON_NOT_TRACKED || skeleton[i].data.eSkeletonPositionTrackingState[joint.pos2] == NUI_SKELETON_NOT_TRACKED)
                continue;

            // Ajustement des coordonnées en fonction de la zone de dessin
            QPoint pos1 = adjustSize(skeleton[i].data.SkeletonPositions[joint.pos1]);
            QPoint pos2 = adjustSize(skeleton[i].data.SkeletonPositions[joint.pos2]);

            // Si la jointures est traquer (TRACKED) on dessine la jointure avec la couleur respectif du squelette
            // Si c'est pas le cas (INFERRED) on dessine la jointure en gris
            if(skeleton[i].data.eSkeletonPositionTrackingState[joint.pos1] == NUI_SKELETON_POSITION_TRACKED || skeleton[i].data.eSkeletonPositionTrackingState[joint.pos2] == NUI_SKELETON_POSITION_TRACKED)
                painter.setPen(QPen(skeleton[i].color, 3));
            else
                painter.setPen(QPen(Qt::gray, 1));

            // Ligne de la jointure
            painter.drawLine(pos1, pos2);

            // Même chose pour les extrémitée de la jointure, TRACKED = vert, INFERRED = vert foncé
            if(skeleton[i].data.eSkeletonPositionTrackingState[joint.pos1] == NUI_SKELETON_POSITION_TRACKED || skeleton[i].data.eSkeletonPositionTrackingState[joint.pos2] == NUI_SKELETON_POSITION_TRACKED)
                painter.setPen(QPen(Qt::green, 1));
            else
                painter.setPen(QPen(Qt::darkGray, 1));

            // Point de chaque extremitée
            painter.drawEllipse(pos1, 2, 2);
            painter.drawEllipse(pos2, 2, 2);
        }

        // For fun : On grossi les point de la tête et des main pour les mettre en valeur
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
    /*QRect rect = zones[current].rect;

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
    }*/
}

bool DrawZone::addZone(std::string name)
{
    /*t_zone zone = {true, QRect(-1, -1, -1, -1)};
    zones[name] = zone;
    current = name;
    return true;*/
}

void DrawZone::hightlight(QPainter& painter, int posX, int posY)
{
    /*for(int x = 0; x <= this->width(); x += CASE_SIZE)
    {
        if(posX >= x && posX < x + CASE_SIZE)
        {
            for(int y = 0; y <= this->height(); y += CASE_SIZE)
            {
                if(posY >= y && posY < y + CASE_SIZE)
                {*/
                    painter.setPen(QPen(Qt::blue, 2));
                    painter.setBrush(Qt::white);
                    //painter.drawRect(QRect(x, y, CASE_SIZE, CASE_SIZE));
                    painter.drawRect(QRect(posX - 5, posY - 5, CASE_SIZE, CASE_SIZE));
                /*}
            }
        }
    }*/
}

QPoint DrawZone::lissage(int posX, int posY)
{
    /*QPoint point(-1, -1);

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

    return point;*/
}

/**
 * @brief DrawZone::adjustSize : ajuste la position des point en fonction de la taille de la zone de dessin
 * @param skeletonPoint : point d'origine
 * @return : point ajustée
 */
QPoint DrawZone::adjustSize(Vector4 skeletonPoint)
{
    LONG x, y;
    USHORT depth;

    NuiTransformSkeletonToDepthImage(skeletonPoint, &x, &y, &depth);

    float pointX = static_cast<float>(x * this->width()) / 320;
    float pointY = static_cast<float>(y * this->height()) / 240;

    return QPoint(pointX, pointY);
}

/**
 * @brief DrawZone::SetSkeleton : enregistre les coordonnées de chauqe points du squelette
 * @param id : numéro du squelette
 * @param data : coordonnées de chaque points
 */
void DrawZone::SetSkeleton(int id, NUI_SKELETON_DATA data)
{
    if(id >= 5)
        id = 0;

    skeleton[id].data = data;
}

/**
 * @brief DrawZone::DrawBone : enregistre les jointure a dessiner (+ indique que le squelette est toujours traqué)
 * @param id : numéro du squelette
 * @param joint : numéro de la jointure
 * @param pos1 : position d'un extremitée
 * @param pos2 : position de l'autre extremité
 */
void DrawZone::DrawBone(int id, int joint, NUI_SKELETON_POSITION_INDEX pos1, NUI_SKELETON_POSITION_INDEX pos2)
{
    if(id >= 5)
        id = 0;

    skeleton[id].jointures[joint].pos1 = pos1;
    skeleton[id].jointures[joint].pos2 = pos2;

    skeleton[id].isSet = true;
    skeleton[id].untracked = 0;
}

/**
 * @brief DrawZone::RemoveSkeleton : supprime une squelette du dessin
 * @param id : numéro du squelette
 */
void DrawZone::RemoveSkeleton(int id)
{
    if(id >= 5)
        id = 0;

    skeleton[id].untracked++;
}

/**
 * @brief DrawZone::lock : vérouille le mutex du vecteur de données des squelettes
 */
void DrawZone::lock()
{
    mutex.lock();
}

/**
 * @brief DrawZone::unlock : dévérouille le mutex du vecteur de données des squelettes
 */
void DrawZone::unlock()
{
    mutex.unlock();
}
