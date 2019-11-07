#include <QApplication>
#include <QDir>
#include <QStandardPaths>
#include "TrichoSciencePro.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("TrichoScience Pro");

    app.setOrganizationDomain("tsp.com");
    app.setApplicationName("TrichoSciencePro");
    auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (path.isEmpty()) qFatal("Cannot determine settings storage location");
    QDir d(path);

    if(!d.exists())
        d.mkpath(".");
    QFile f{"beta-test.txt"};


    if (QDir::setCurrent(d.absolutePath()))
    {
        ////    qDebug() << "settings in" << QDir::currentPath();
        if(f.open(QIODevice::ReadOnly))
        {

            QTextStream stream(&f);
            QString line = stream.readLine();
            qDebug() << line;
            bool ok;
            double version = line.toDouble(&ok);
            if(version<0.36)
            {
                if(d.cd("data"))
                {
                    d.removeRecursively();
                    d.cdUp();
                }
                if(d.cd("tmp"))
                {
                    d.removeRecursively();
                    d.cdUp();
                }

            }


        }
        f.close();
        if (f.open(QIODevice::WriteOnly | QIODevice::Truncate))
            f.write("0.36");

    }
    f.close();

    TrichoSciencePro TSP;
    TSP.start();

    return app.exec();
}
