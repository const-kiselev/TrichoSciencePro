#include <QApplication>
#include <QDir>
#include <QStandardPaths>
#include "TrichoSciencePro.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("TrichoScience Pro");

    app.setOrganizationDomain("tsp.com");
    app.setApplicationName("TrichoScience.Pro");
    auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (path.isEmpty()) qFatal("Cannot determine settings storage location");
    QDir d(path);
    if (d.mkpath(d.absolutePath()) && QDir::setCurrent(d.absolutePath()))
    {
////    qDebug() << "settings in" << QDir::currentPath();
    QFile f{"beta-test.txt"};
    if (f.open(QIODevice::WriteOnly | QIODevice::Truncate))
      f.write("version 0.1");
    }

    TrichoSciencePro TSP;
    TSP.start();



    return app.exec();
}
