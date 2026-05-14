#include "SeaSaltGameController.h"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    try {
        QApplication app(argc, argv);

        SeaSaltGameController gameController;
        QQmlApplicationEngine engine;
        engine.rootContext()->setContextProperty(QStringLiteral("game"), &gameController);

        const QUrl url(QStringLiteral("qrc:/qml/Main.qml"));
        QObject::connect(
            &engine, &QQmlApplicationEngine::objectCreated, &app,
            [url](QObject *obj, const QUrl &objUrl) {
                if (!obj && url == objUrl) {
                    QCoreApplication::exit(-1);
                }
            },
            Qt::QueuedConnection);
        engine.load(url);

        if (engine.rootObjects().isEmpty()) {
            return -1;
        }

        return app.exec();
    } catch (const std::exception &) {
        return -1;
    }
}
