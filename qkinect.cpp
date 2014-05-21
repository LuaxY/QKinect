#include "qkinect.h"
#include <QDebug>
#include <exception>

QKinect::QKinect(QObject *parent) :
    QThread(parent),
    isRunning(false),
    isInitialized(false)
{
}

void QKinect::start()
{
    qDebug() << "starting Kinect";
    if(!isRunning)
        QThread::start();
}

void QKinect::stop()
{
    if(isRunning)
        isRunning = false;

    qDebug() << "stoping Kinect";
    wait();

    NuiShutdown();
    isInitialized = false;
}

void QKinect::run()
{
    try
    {
        qDebug() << "initialization...";
        init();

        qDebug() << "tracking...";
        tracking();
    }
    catch(std::exception& e)
    {
        qDebug() << "Exception: " << e.what();
    }
}

void QKinect::init()
{
    HRESULT res = NuiInitialize(NUI_INITIALIZE_FLAG_USES_SKELETON);

    if(res != S_OK)
    {
        throw std::logic_error("No Kinect found...");
    }

    res = NuiSkeletonTrackingEnable(NULL, 0);

    if(res != S_OK)
        throw std::logic_error("Cannot initialize Skeleton Tracking.");

    isInitialized = true;
    isRunning = true;
}

void QKinect::tracking()
{
    int count = 1;
    while(isRunning)
    {
        HRESULT res = NuiSkeletonGetNextFrame(1, &mSkeletonFrame);

        bool foundSkeleton = false;
        for(int i = 0; i < NUI_SKELETON_COUNT; i++)
        {
            if(mSkeletonFrame.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED)
                foundSkeleton = true;
        }

        if(foundSkeleton)
        {
            qDebug() << "found skeleton " << count;
            count++;
        }

        msleep(30);
    }
}
