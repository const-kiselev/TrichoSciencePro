#include "ImageEditor.h"
#include <QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("TrichoScience Pro");

    app.setOrganizationDomain("tsp.com");
    app.setApplicationName("imageEditor.TrichoScience.Pro");
    auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (path.isEmpty()) qFatal("Cannot determine settings storage location");
    QDir d(path);
    if (d.mkpath(d.absolutePath()) && QDir::setCurrent(d.absolutePath()))
    {
////    qDebug() << "settings in" << QDir::currentPath();
//    QFile f{"settings.txt"};
//    if (f.open(QIODevice::WriteOnly | QIODevice::Truncate))
//      f.write("Hello, World");
    }


    TSPimageEditor w;


    w.show();


    return app.exec();
}
