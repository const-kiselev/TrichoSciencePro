#include "PatientManager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationDomain("tsp.com");
    app.setApplicationName("TrichoScience.Pro");
    auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (path.isEmpty()) qFatal("Cannot determine settings storage location");
    QDir d(path);
    if (d.mkpath(d.absolutePath()) && QDir::setCurrent(d.absolutePath()))
    {
////    qDebug() << "settings in" << QDir::currentPath();
    QFile f{"beta.txt"};
    if (f.open(QIODevice::WriteOnly | QIODevice::Truncate))
      f.write("tested");
    }
    PatientManager w;



    w.show();

    return app.exec();
}
