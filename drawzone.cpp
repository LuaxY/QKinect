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
    // Initialisation de chaque squelettes
    for(unsigned int i = 0; i < 5; i++)
    {
        mSkeleton[i].color = Qt::red;
        mSkeleton[i].isSet = false;
        mSkeleton[i].untracked = 0;
    }

    // Affectation d'un couleur différente pour chaque squelette
    mSkeleton[0].color = Qt::blue;
    mSkeleton[1].color = Qt::magenta;
    mSkeleton[2].color = Qt::cyan;
    mSkeleton[3].color = Qt::yellow;
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

    /* AFFICHAGE DES SQUELETTES */

    // Vérouillage du mutex pour le vecteur des points de chaque squelettes (thread safe)
    lock();

    // Pour chaques squelettes :
    for(unsigned int i = 0; i < 5; i++)
    {
        // Si le squelettes n'a pas encore de données on passe
        if(!mSkeleton[i].isSet)
            continue;

        // Si le squelettes n'est plus traquer depuis 20 frames on le supprime
        if(mSkeleton[i].untracked > 0)
        {
            mSkeleton[i].untracked++;
            if(mSkeleton[i].untracked > 20)
            {
                mSkeleton[i].isSet = false;
                continue;
            }
        }

        // On dessine chaque jointure du squelette :

        // Jointures du torse
        DrawJointure(painter, i, NUI_SKELETON_POSITION_HEAD, NUI_SKELETON_POSITION_SHOULDER_CENTER);
        DrawJointure(painter, i, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SHOULDER_LEFT);
        DrawJointure(painter, i, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SHOULDER_RIGHT);
        DrawJointure(painter, i, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SPINE);
        DrawJointure(painter, i, NUI_SKELETON_POSITION_SPINE, NUI_SKELETON_POSITION_HIP_CENTER);
        DrawJointure(painter, i, NUI_SKELETON_POSITION_HIP_CENTER, NUI_SKELETON_POSITION_HIP_LEFT);
        DrawJointure(painter, i, NUI_SKELETON_POSITION_HIP_CENTER, NUI_SKELETON_POSITION_HIP_RIGHT);

        // Joiuntures bras gauche
        DrawJointure(painter, i, NUI_SKELETON_POSITION_SHOULDER_LEFT, NUI_SKELETON_POSITION_ELBOW_LEFT);
        DrawJointure(painter, i, NUI_SKELETON_POSITION_ELBOW_LEFT, NUI_SKELETON_POSITION_WRIST_LEFT);
        DrawJointure(painter, i, NUI_SKELETON_POSITION_WRIST_LEFT, NUI_SKELETON_POSITION_HAND_LEFT);

        // Jointures bras droit
        DrawJointure(painter, i, NUI_SKELETON_POSITION_SHOULDER_RIGHT, NUI_SKELETON_POSITION_ELBOW_RIGHT);
        DrawJointure(painter, i, NUI_SKELETON_POSITION_ELBOW_RIGHT, NUI_SKELETON_POSITION_WRIST_RIGHT);
        DrawJointure(painter, i, NUI_SKELETON_POSITION_WRIST_RIGHT, NUI_SKELETON_POSITION_HAND_RIGHT);

        // Jointures jambe gauche
        DrawJointure(painter, i, NUI_SKELETON_POSITION_HIP_LEFT, NUI_SKELETON_POSITION_KNEE_LEFT);
        DrawJointure(painter, i, NUI_SKELETON_POSITION_KNEE_LEFT, NUI_SKELETON_POSITION_ANKLE_LEFT);
        DrawJointure(painter, i, NUI_SKELETON_POSITION_ANKLE_LEFT, NUI_SKELETON_POSITION_FOOT_LEFT);

        // Jointures jambe droite
        DrawJointure(painter, i, NUI_SKELETON_POSITION_HIP_RIGHT, NUI_SKELETON_POSITION_KNEE_RIGHT);
        DrawJointure(painter, i, NUI_SKELETON_POSITION_KNEE_RIGHT, NUI_SKELETON_POSITION_ANKLE_RIGHT);
        DrawJointure(painter, i, NUI_SKELETON_POSITION_ANKLE_RIGHT, NUI_SKELETON_POSITION_FOOT_RIGHT);

        // For fun : On grossi les points de la tête et des main pour les mettre en valeur
        QPoint headPos  = AdjustSize(mSkeleton[i].data.SkeletonPositions[NUI_SKELETON_POSITION_HEAD]);
        QPoint leftPos  = AdjustSize(mSkeleton[i].data.SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT]);
        QPoint rightPos = AdjustSize(mSkeleton[i].data.SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT]);

        painter.setPen(QPen(Qt::red, 5));
        painter.setBrush(Qt::white);

        painter.drawEllipse(headPos, 30, 30);
        painter.drawEllipse(leftPos, 10, 10);
        painter.drawEllipse(rightPos, 10, 10);
    }

    // Dévérouillage du mutex pour le vecteur des points de chaque squelettes
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

/**
 * @brief DrawZone::adjustSize : ajuste la position des point en fonction de la taille de la zone de dessin
 * @param mSkeletonPoint : point d'origine
 * @return : point ajustée
 */
QPoint DrawZone::AdjustSize(Vector4 skeletonPoint)
{
    LONG x, y;
    USHORT depth;

    NuiTransformSkeletonToDepthImage(skeletonPoint, &x, &y, &depth);

    float pointX = static_cast<float>(x * this->width()) / 320;
    float pointY = static_cast<float>(y * this->height()) / 240;

    return QPoint(pointX, pointY);
}

/**
 * @brief DrawZone::SetmSkeleton : enregistre les coordonnées de chauqe points du squelette
 * @param id : numéro du squelette
 * @param data : coordonnées de chaque points
 */
void DrawZone::SetSkeleton(int id, NUI_SKELETON_DATA data)
{
    if(id >= 5)
        id = 0;

    mSkeleton[id].data = data;
    mSkeleton[id].isSet = true;
    mSkeleton[id].untracked = 0;
}

/**
 * @brief DrawZone::RemovemSkeleton : supprime une squelette du dessin
 * @param id : numéro du squelette
 */
void DrawZone::RemoveSkeleton(int id)
{
    if(id >= 5)
        id = 0;

    mSkeleton[id].untracked++;
}

/**
 * @brief DrawZone::DrawJointure : dessine une jointure entre deux point
 * @param painter : référence vers l'objet QPainter de la zone de dessin
 * @param id : numéro du squelette
 * @param posA : position du point A
 * @param posB : position du point B
 */
void DrawZone::DrawJointure(QPainter &painter, int id, NUI_SKELETON_POSITION_INDEX posA, NUI_SKELETON_POSITION_INDEX posB)
{
    NUI_SKELETON_POSITION_TRACKING_STATE statePointA = mSkeleton[id].data.eSkeletonPositionTrackingState[posA];
    NUI_SKELETON_POSITION_TRACKING_STATE statePointB = mSkeleton[id].data.eSkeletonPositionTrackingState[posB];

    // Si la position de l'un des deux point est inconnue, on passe
    if(statePointA == NUI_SKELETON_NOT_TRACKED || statePointB == NUI_SKELETON_NOT_TRACKED)
        return;

    // Ajustement des coordonnées en fonction de la zone de dessin
    QPoint pointA = AdjustSize(mSkeleton[id].data.SkeletonPositions[posA]);
    QPoint pointB = AdjustSize(mSkeleton[id].data.SkeletonPositions[posB]);

    // Si la jointures est traquer (TRACKED) on dessine la jointure avec la couleur respectif du squelette
    // Si c'est pas le cas (INFERRED) on dessine la jointure en gris
    if(statePointA == NUI_SKELETON_POSITION_TRACKED || statePointB == NUI_SKELETON_POSITION_TRACKED)
        painter.setPen(QPen(mSkeleton[id].color, 3));
    else
        painter.setPen(QPen(Qt::gray, 1));

    // Ligne de la jointure
    painter.drawLine(pointA, pointB);

    // Même chose pour les extrémitée de la jointure, TRACKED = vert, INFERRED = vert foncé
    if(statePointA == NUI_SKELETON_POSITION_TRACKED || statePointB == NUI_SKELETON_POSITION_TRACKED)
        painter.setPen(QPen(Qt::green, 1));
    else
        painter.setPen(QPen(Qt::darkGray, 1));

    // Point de chaque extremitée
    painter.drawEllipse(pointA, 2, 2);
    painter.drawEllipse(pointB, 2, 2);
}

/**
 * @brief DrawZone::lock : vérouille le mutex du vecteur de données des squelettes
 */
void DrawZone::lock()
{
    mMutex.lock();
}

/**
 * @brief DrawZone::unlock : dévérouille le mutex du vecteur de données des squelettes
 */
void DrawZone::unlock()
{
    mMutex.unlock();
}
