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
    // Initialisation de capteur squeletique
    HRESULT res = NuiInitialize(NUI_INITIALIZE_FLAG_USES_SKELETON);

    if(res != S_OK)
    {
        //throw std::logic_error("No Kinect found...");
        qDebug() << "No Kinect found...";
    }
    else
    {
        isInitialized = true;
    }
}

/**
 * @brief QKinect::start : démarrage de la capture de données
 */
void QKinect::start()
{
    if(isRunning)
        return;

    if(isInitialized)
    {
        qDebug() << "starting Kinect";
        QThread::start();
    }
}

/**
 * @brief QKinect::stop : arrêt de la capture de données
 */
void QKinect::stop()
{
    if(isRunning && isInitialized)
    {
        isRunning = false;

        qDebug() << "stopping Kinect";

        wait();
        NuiSkeletonTrackingDisable();
        //NuiShutdown();

        //isInitialized = false;
    }
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
    // Démarrage de la capture
    HRESULT res = NuiSkeletonTrackingEnable(NULL, 0);

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
                // Vérouillage du mutex pour le vecteur des points de chaque squelettes
                mDrawZone->lock();

                // Enregistrement des coordonnée de chaque points pour la zone de dessin
                mDrawZone->SetSkeleton(i, mSkeletonFrame.SkeletonData[i]);

                // Dévérouillage du mutex pour le vecteur des points de chaque squelettes
                mDrawZone->unlock();
            }
            else if(mSkeletonFrame.SkeletonData[i].eTrackingState == NUI_SKELETON_NOT_TRACKED)
            {
                // Squelette n'est plus traqué, on le retire de la zone de dessin
                mDrawZone->RemoveSkeleton(i);
            }
        }

        // Sleep de 30ms pour éviter de pourrir la ressource CPU :)
        msleep(30);
    }
}
