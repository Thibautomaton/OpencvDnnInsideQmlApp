#include "videostreamer.h"
#include <fstream>
#include <QDebug>
#include <QFile>

VideoStreamer::VideoStreamer()
{
    qDebug()<<"hey";
    connect(&tUpdate,&QTimer::timeout,this,&VideoStreamer::streamVideo);

    net = dnn::readNetFromTensorflow("frozen_inference_graph_V2.pb","ssd_mobilenet_v2_coco_2018_03_29.pbtxt");
    if (net.empty())
    {
        qDebug()<<"init the model net error";
        exit(-1);
    }
    bool result = getFileContent("COCO_labels.txt");
    if(!result)
    {
        qDebug() << "loading labels failed";
        exit(-1);
    }
}

VideoStreamer::~VideoStreamer()
{
    cap.release();
    tUpdate.stop();
}

void VideoStreamer::streamVideo()
{
    float f;
    float FPS[16];
    int i, Fcnt=0;
    Mat frame;
    std::chrono::steady_clock::time_point Tbegin, Tend;

    Tbegin = std::chrono::steady_clock::now();
    cap>>frame;

    detect_from_video(frame);

    Tend = std::chrono::steady_clock::now();

    f = std::chrono::duration_cast <std::chrono::milliseconds> (Tend - Tbegin).count();
    if(f>0.0) FPS[((Fcnt++)&0x0F)]=1000.0/f;
    for(f=0.0, i=0;i<16;i++){ f+=FPS[i]; }
    putText(frame, format("FPS %0.2f", f/16),Point(10,20),FONT_HERSHEY_SIMPLEX,0.6, Scalar(0, 0, 255));

    QImage img = QImage( frame.data,frame.cols,frame.rows, QImage::Format_RGB888).rgbSwapped();
    emit newImage(img);
}

void VideoStreamer::openVideoCamera(int input)
{
    cap.open(input);

    double fps = cap.get(cv::CAP_PROP_FPS);
    tUpdate.start(1000/fps);
}

bool VideoStreamer::initiateNames()
{

    QFile file("qrc:/labels/COCO_labels.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    in.setCodec("UTF-8");
    while(!in.atEnd())
    {
        QString str = in.readLine();

        if(str.size()>0) Names.push_back(str.toStdString());
        qDebug()<<str;
    }
    return true;
}

bool VideoStreamer::getFileContent(std::string fileName)
{
    // Open the File
    std::ifstream in(fileName.c_str());
    // Check if object is valid
    if(!in.is_open()) return false;

    std::string str;
    // Read the next line from File untill it reaches the end.
    while (std::getline(in, str))
    {
        // Line contains string of length > 0 then save it in vector
        if(str.size()>0) Names.push_back(str);
    }
    // Close The File
    in.close();
    return true;
}

void VideoStreamer::detect_from_video(Mat &src)
{
    Mat blobimg = dnn::blobFromImage(src, 1.0, Size(300, 300), 0.0, true);

    net.setInput(blobimg);

    Mat detection = net.forward("detection_out");
//	cout << detection.size[2]<<" "<< detection.size[3] << endl;
    Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());

    const float confidence_threshold = 0.25;
    for(int i=0; i<detectionMat.rows; i++){
        float detect_confidence = detectionMat.at<float>(i, 2);

        if(detect_confidence > confidence_threshold){
            size_t det_index = (size_t)detectionMat.at<float>(i, 1);
            float x1 = detectionMat.at<float>(i, 3)*src.cols;
            float y1 = detectionMat.at<float>(i, 4)*src.rows;
            float x2 = detectionMat.at<float>(i, 5)*src.cols;
            float y2 = detectionMat.at<float>(i, 6)*src.rows;
            Rect rec((int)x1, (int)y1, (int)(x2 - x1), (int)(y2 - y1));
            rectangle(src,rec, Scalar(0, 0, 255), 1, 8, 0);
            putText(src, format("%s", Names[det_index].c_str()), Point(x1, y1-5) ,FONT_HERSHEY_SIMPLEX,0.5, Scalar(0, 0, 255), 1, 8, 0);
        }
    }
}
