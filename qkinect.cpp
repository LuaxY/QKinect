#include "qkinect.h"
#include <QDebug>
#include <exception>

QKinect::QKinect(QObject *parent, DrawZone* drawZone) :
    QThread(parent),
    isRunning(false),
    isInitialized(false),
    mDrawZone(drawZone)
{
}

void QKinect::start()
{
    if(isRunning)
        return;

    qDebug() << "starting Kinect";
    QThread::start();
}

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
    while(isRunning)
    {
        HRESULT res = NuiSkeletonGetNextFrame(1, &mSkeletonFrame);

        NuiTransformSmooth(&mSkeletonFrame, NULL);

        for(int i = 0; i < NUI_SKELETON_COUNT; i++)
        {
            if(mSkeletonFrame.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED)
            {
                mDrawZone->lock();

                mDrawZone->SetSkeleton(i, mSkeletonFrame.SkeletonData[i]);

                // Render Torso
                mDrawZone->DrawBone(i, 0, NUI_SKELETON_POSITION_HEAD, NUI_SKELETON_POSITION_SHOULDER_CENTER);
                mDrawZone->DrawBone(i, 1, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SHOULDER_LEFT);
                mDrawZone->DrawBone(i, 2, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SHOULDER_RIGHT);
                mDrawZone->DrawBone(i, 3, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SPINE);
                mDrawZone->DrawBone(i, 4, NUI_SKELETON_POSITION_SPINE, NUI_SKELETON_POSITION_HIP_CENTER);
                mDrawZone->DrawBone(i, 5, NUI_SKELETON_POSITION_HIP_CENTER, NUI_SKELETON_POSITION_HIP_LEFT);
                mDrawZone->DrawBone(i, 6, NUI_SKELETON_POSITION_HIP_CENTER, NUI_SKELETON_POSITION_HIP_RIGHT);

                // Left Arm
                mDrawZone->DrawBone(i, 7, NUI_SKELETON_POSITION_SHOULDER_LEFT, NUI_SKELETON_POSITION_ELBOW_LEFT);
                mDrawZone->DrawBone(i, 8, NUI_SKELETON_POSITION_ELBOW_LEFT, NUI_SKELETON_POSITION_WRIST_LEFT);
                mDrawZone->DrawBone(i, 9, NUI_SKELETON_POSITION_WRIST_LEFT, NUI_SKELETON_POSITION_HAND_LEFT);

                // Right Arm
                mDrawZone->DrawBone(i, 10, NUI_SKELETON_POSITION_SHOULDER_RIGHT, NUI_SKELETON_POSITION_ELBOW_RIGHT);
                mDrawZone->DrawBone(i, 11, NUI_SKELETON_POSITION_ELBOW_RIGHT, NUI_SKELETON_POSITION_WRIST_RIGHT);
                mDrawZone->DrawBone(i, 12, NUI_SKELETON_POSITION_WRIST_RIGHT, NUI_SKELETON_POSITION_HAND_RIGHT);

                // Left Leg
                mDrawZone->DrawBone(i, 13, NUI_SKELETON_POSITION_HIP_LEFT, NUI_SKELETON_POSITION_KNEE_LEFT);
                mDrawZone->DrawBone(i, 14, NUI_SKELETON_POSITION_KNEE_LEFT, NUI_SKELETON_POSITION_ANKLE_LEFT);
                mDrawZone->DrawBone(i, 15, NUI_SKELETON_POSITION_ANKLE_LEFT, NUI_SKELETON_POSITION_FOOT_LEFT);

                // Right Leg
                mDrawZone->DrawBone(i, 16, NUI_SKELETON_POSITION_HIP_RIGHT, NUI_SKELETON_POSITION_KNEE_RIGHT);
                mDrawZone->DrawBone(i, 17, NUI_SKELETON_POSITION_KNEE_RIGHT, NUI_SKELETON_POSITION_ANKLE_RIGHT);
                mDrawZone->DrawBone(i, 18, NUI_SKELETON_POSITION_ANKLE_RIGHT, NUI_SKELETON_POSITION_FOOT_RIGHT);

                mDrawZone->unlock();
            }
            else if(mSkeletonFrame.SkeletonData[i].eTrackingState == NUI_SKELETON_NOT_TRACKED)
            {
                mDrawZone->RemoveSkeleton(i);
            }
        }

        msleep(30);
    }
}
