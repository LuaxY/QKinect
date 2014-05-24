#include "qkinect.h"
#include <QDebug>
#include <exception>

/**
 * @brief QKinect::QKinect : constructeur
 * @param parent : classe parent
 * @param drawZone : zone de dessin
 */
QKinect::QKinect(QObject *parent, DrawZone* drawZone) :
    QThread(parent),
    isRunning(false),
    isInitialized(false),
    mDrawZone(drawZone)
{
}

/**
 * @brief QKinect::start : démarrage de la capture de données
 */
void QKinect::start()
{
    if(isRunning)
        return;

    qDebug() << "starting Kinect";
    QThread::start();
}

/**
 * @brief QKinect::stop : arrêt de la capture de données
 */
void QKinect::stop()
{
    if(isRunning)
        isRunning = false;

    qDebug() << "stopping Kinect";

    wait();
    NuiSkeletonTrackingDisable();
    //NuiShutdown();

    isInitialized = false;
}

/**
 * @brief QKinect::run : fonction executer par QThread
 */
void QKinect::run()
{
    try
    {
        // Initialisation de la Kinect
        qDebug() << "initialization...";
        init();

        // Boucle de capture des données
        qDebug() << "tracking...";
        tracking();
    }
    catch(std::exception& e)
    {
        qDebug() << "Exception: " << e.what();
    }
}

/**
 * @brief QKinect::init : initialisation de la Kinect
 */
void QKinect::init()
{
    // Initialisation de capteur squeletique
    HRESULT res = NuiInitialize(NUI_INITIALIZE_FLAG_USES_SKELETON);

    if(res != S_OK)
    {
        throw std::logic_error("No Kinect found...");
    }

    // Démarrage de la capture
    res = NuiSkeletonTrackingEnable(NULL, 0);

    if(res != S_OK)
        throw std::logic_error("Cannot initialize Skeleton Tracking.");

    isInitialized = true;
    isRunning = true;
}

/**
 * @brief QKinect::tracking : Boucle principale de capture
 */
void QKinect::tracking()
{
    // Tant que la capture est demandé
    while(isRunning)
    {
        // Récupération de la frame suivante
        HRESULT res = NuiSkeletonGetNextFrame(1, &mSkeletonFrame);

        NuiTransformSmooth(&mSkeletonFrame, NULL);

        for(int i = 0; i < NUI_SKELETON_COUNT; i++)
        {
            if(mSkeletonFrame.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED)
            {
                // vérouillage du mutex du vecteur des point du squelette
                mDrawZone->lock();

                // Enregistrement des coordonnée de chaque points pour la zone de dessin
                mDrawZone->SetSkeleton(i, mSkeletonFrame.SkeletonData[i]);

                // Jointure du torse
                mDrawZone->DrawBone(i, 0, NUI_SKELETON_POSITION_HEAD, NUI_SKELETON_POSITION_SHOULDER_CENTER);
                mDrawZone->DrawBone(i, 1, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SHOULDER_LEFT);
                mDrawZone->DrawBone(i, 2, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SHOULDER_RIGHT);
                mDrawZone->DrawBone(i, 3, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SPINE);
                mDrawZone->DrawBone(i, 4, NUI_SKELETON_POSITION_SPINE, NUI_SKELETON_POSITION_HIP_CENTER);
                mDrawZone->DrawBone(i, 5, NUI_SKELETON_POSITION_HIP_CENTER, NUI_SKELETON_POSITION_HIP_LEFT);
                mDrawZone->DrawBone(i, 6, NUI_SKELETON_POSITION_HIP_CENTER, NUI_SKELETON_POSITION_HIP_RIGHT);

                // Joiunture bras gauche
                mDrawZone->DrawBone(i, 7, NUI_SKELETON_POSITION_SHOULDER_LEFT, NUI_SKELETON_POSITION_ELBOW_LEFT);
                mDrawZone->DrawBone(i, 8, NUI_SKELETON_POSITION_ELBOW_LEFT, NUI_SKELETON_POSITION_WRIST_LEFT);
                mDrawZone->DrawBone(i, 9, NUI_SKELETON_POSITION_WRIST_LEFT, NUI_SKELETON_POSITION_HAND_LEFT);

                // Jointure bras droit
                mDrawZone->DrawBone(i, 10, NUI_SKELETON_POSITION_SHOULDER_RIGHT, NUI_SKELETON_POSITION_ELBOW_RIGHT);
                mDrawZone->DrawBone(i, 11, NUI_SKELETON_POSITION_ELBOW_RIGHT, NUI_SKELETON_POSITION_WRIST_RIGHT);
                mDrawZone->DrawBone(i, 12, NUI_SKELETON_POSITION_WRIST_RIGHT, NUI_SKELETON_POSITION_HAND_RIGHT);

                // Jointure jambe gauche
                mDrawZone->DrawBone(i, 13, NUI_SKELETON_POSITION_HIP_LEFT, NUI_SKELETON_POSITION_KNEE_LEFT);
                mDrawZone->DrawBone(i, 14, NUI_SKELETON_POSITION_KNEE_LEFT, NUI_SKELETON_POSITION_ANKLE_LEFT);
                mDrawZone->DrawBone(i, 15, NUI_SKELETON_POSITION_ANKLE_LEFT, NUI_SKELETON_POSITION_FOOT_LEFT);

                // Jointure jambe droite
                mDrawZone->DrawBone(i, 16, NUI_SKELETON_POSITION_HIP_RIGHT, NUI_SKELETON_POSITION_KNEE_RIGHT);
                mDrawZone->DrawBone(i, 17, NUI_SKELETON_POSITION_KNEE_RIGHT, NUI_SKELETON_POSITION_ANKLE_RIGHT);
                mDrawZone->DrawBone(i, 18, NUI_SKELETON_POSITION_ANKLE_RIGHT, NUI_SKELETON_POSITION_FOOT_RIGHT);

                // dévérouillage du mutex du vecteur des point du squelette
                mDrawZone->unlock();
            }
            else if(mSkeletonFrame.SkeletonData[i].eTrackingState == NUI_SKELETON_NOT_TRACKED)
            {
                // Squelette n'est plus traquer, on le retire de la zone de dessin
                mDrawZone->RemoveSkeleton(i);
            }
        }

        // sleep de 30ms pour éviter de pourrir la ressource CPU :)
        msleep(30);
    }
}
