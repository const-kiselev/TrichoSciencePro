#ifndef IE_REPORT_H
#define IE_REPORT_H

#include <QObject>
#include <QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QCategoryAxis>
#include "ie_compute.h"

/*!
  \brief Класс формирования отчета редактора изображения.
    Политика использования:
        Экземпляр создается, инициализируется, исполняется и удаляется. Нет необходимости длительного использования.
        \todo Класс ДОЛЖЕН не зависеть от слоев модели (не знать о них и не требовать их существования). Подразумевается, что будут только входные данные от вычислений `IE_Compute::OutputData` + данные модели (данные пациента и всякое такое).
        \todo Есть необходимость сделать структуру ComputeOut
*/


class IE_Report: public QObject
{
    Q_OBJECT
public:
    explicit IE_Report(_global_ie * pieg);

    //! \todo РЕАЛИЗОВАТЬ
    void init(IE_Compute::OutputData computeData) {}
    //! \todo РЕАЛИЗОВАТЬ
    void makeReport() {}
    //! \todo добавить путь где сохранять
    //! \todo РЕАЛИЗОВАТЬ
    int saveReport(){}
    //! \todo добавить параметр
    //! \todo РЕАЛИЗОВАТЬ
    void addDocObjToFirstReportSection(){}

    void makeHairDensityReport_dialog(IE_Compute::OutputData computeData);
    void makeHairDiameterReport_dialog(IE_Compute::OutputData computeData);
    void makeTrichoscopyReport_dialog(IE_Compute::OutputData computeData);



private:
    _global_ie *_p_ie_global_data;
};

#endif // IE_REPORT_H
