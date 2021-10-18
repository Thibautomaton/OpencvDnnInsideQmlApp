#ifndef VIDEOSTREAMER_H
#define VIDEOSTREAMER_H

#include <QObject>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>

#include <QImage>
#include <iostream>
#include <stdio.h>
#include <opencv2/core/ocl.hpp>

using namespace cv;

class VideoStreamer: public QObject
{
    Q_OBJECT

public:
    VideoStreamer();
    ~VideoStreamer();

    void streamVideo();
    bool getFileContent(std::string fileName);
    void detect_from_video(Mat &src);
    bool initiateNames();

public slots:
    void openVideoCamera(int input);

private:
    Mat frame;
    VideoCapture cap;
    QTimer tUpdate;
    dnn::Net net;
    std::vector<std::string> Names;

signals:
    void newImage(QImage &);
};

#endif // VIDEOSTREAMER_H
