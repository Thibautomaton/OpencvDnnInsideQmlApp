#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "opencvimageprovider.h"
#include "videostreamer.h"
#include <QQmlContext>


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    qDebug()<<"hello";
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    VideoStreamer videoStreamer1;
    VideoStreamer videoStreamer2;

    OpencvImageProvider *liveImageProvider1(new OpencvImageProvider);
    OpencvImageProvider *liveImageProvider2(new OpencvImageProvider);



    engine.rootContext()->setContextProperty("VideoStreamer1", &videoStreamer1);
    engine.rootContext()->setContextProperty("VideoStreamer2", &videoStreamer2);

    engine.rootContext()->setContextProperty("liveImageProvider1", liveImageProvider1);
    engine.rootContext()->setContextProperty("liveImageProvider2", liveImageProvider2);

   engine.addImageProvider("live1", liveImageProvider1);
   engine.addImageProvider("live2", liveImageProvider2);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    QObject::connect(&videoStreamer1, &VideoStreamer::newImage, liveImageProvider1, &OpencvImageProvider::updateImage);
    QObject::connect(&videoStreamer2, &VideoStreamer::newImage, liveImageProvider2, &OpencvImageProvider::updateImage);

    return app.exec();
}
