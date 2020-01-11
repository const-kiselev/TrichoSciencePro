#include "ie_report.h"
#include "ie_compute.h"

IE_Report::IE_Report(_global_ie *pieg, QObject *parent) : QObject(parent),  _p_ie_global_data(pieg)
{
    m_pWidget = new QWidget();
    connect(m_pWidget, &QWidget::destroyed, this, &IE_Report::deleteLater);
}



// static
void IE_Report::makeReport(QObject *parent, _global_ie *pieg, IE_Compute::InputData id)
{
    IE_Report * pReport = new IE_Report(pieg, parent);
    pReport->makeReport(id);
}

void IE_Report::makeReport(IE_Compute::InputData id)
{
    switch (id.reportType) {
    case IE_ReportType::HairDensity:
        makeHairDensityReport_dialog( IE_Compute::makeCompute(_p_ie_global_data, id, parent()) );
        break;
    }
}

void IE_Report::makeHairDensityReport_dialog(IE_Compute::OutputData computeData)
{
    QChart *chart = new QChart();
    QChartView *chartView = new QChartView(chart);

    QBarSet *set0 = new QBarSet("Всего волос");
    *set0 << computeData.density.totalHairQuantityInCm2;
    QBarSet *set1 = new QBarSet("Терминальных волос");
    *set1 << computeData.density.terminHairInCm2;
    QBarSet *set2 = new QBarSet("Веллус");
    *set2 << computeData.density.vellusHairInCm2;

    QBarSeries *series = new QBarSeries();
    series->append(set0);
    series->append(set1);
    series->append(set2);

    series->setLabelsFormat("@value");
    series->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);
    series->setLabelsVisible();

    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chartView->setRenderHint(QPainter::Antialiasing);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0,300);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);



    QCategoryAxis *axisY3 = new QCategoryAxis;
    axisY3->append(" ", 190);
    axisY3->append("Норма", 230);
    axisY3->append(" ", 300);
    axisY3->setShadesPen(Qt::NoPen);
    axisY3->setShadesBrush(QBrush(QColor(0x99, 0xcc, 0xcc, 0x55)));
    axisY3->setShadesVisible(true);

    chart->addAxis(axisY3, Qt::AlignRight);
    series->attachAxis(axisY3);


    m_pWidget->setWindowFlag(Qt::Tool);
    m_pWidget->setWindowTitle("[Отчет] Плотность волос");
    QVBoxLayout *pVertBoxLayout = new QVBoxLayout(m_pWidget);
    pVertBoxLayout->addWidget(chartView);


//    length->setNum(0);
//    QHBoxLayout *pHorBoxLayout = new QHBoxLayout(this)






    QLabel *pLabel;
//    pLabel->setNum(i);
    pLabel = new QLabel(QString("Всего измеренно волос: %1. Это %2 на см.кв.")  .arg(computeData.density.hairQuantity)
                                                                                .arg(computeData.density.totalHairQuantityInCm2), m_pWidget);
    pVertBoxLayout->addWidget(pLabel);
    pLabel = new QLabel(QString("Из них терминальных волос: %1. Это %2 на см.кв. или %3 %").arg(computeData.density.terminalQuantity).
                                                                                            arg(computeData.density.terminHairInCm2).
                                                                                            arg(computeData.density.percentTermin), m_pWidget);
    pVertBoxLayout->addWidget(pLabel);
    pLabel = new QLabel(QString("Веллус: %1. Это %2 на см.кв. или %3 %"). arg(computeData.density.vellusQuantity).arg(computeData.density.vellusHairInCm2).
                                                                                            arg(computeData.density.percentVellus), m_pWidget);
    pVertBoxLayout->addWidget(pLabel);
    pLabel = new QLabel(QString("Площадь изображения: %1 %2.кв.").arg(computeData.density.squareInCm2).arg(UnitTypeTitle[UnitType::cm]), m_pWidget);
    pVertBoxLayout->addWidget(pLabel);

    m_pWidget->setMinimumWidth(500);
    m_pWidget->setMinimumHeight(500);
    m_pWidget->show();
}

void IE_Report::makeHairDiameterReport_dialog(IE_Compute::OutputData computeData)
{
    QBarSet *set1 = new QBarSet("Диаметр измеренного волоса");
    set1->append(computeData.diameter.diametersListInUm);
    QChart *chart = new QChart();
    QChartView *chartView = new QChartView(chart);

    QBarSet *set0 = new QBarSet("Средний диамтер");
    *set0 << computeData.diameter.midDiameterAll;



    QBarSeries *series = new QBarSeries();
    series->append(set0);
    series->append(set1);

//    series->setLabelsFormat("@value");
//    series->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);
//    series->setLabelsVisible();

    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chartView->setRenderHint(QPainter::Antialiasing);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0,140);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);



    QCategoryAxis *axisY3 = new QCategoryAxis;
    axisY3->append(" ", _p_ie_global_data->convertUnitedWithForamtF(_p_ie_global_data->getThreshold_thinHair(), UnitType::um));
    axisY3->append("Норма",_p_ie_global_data->convertUnitedWithForamtF(_p_ie_global_data->getThreshold_mediumHair(), UnitType::um) );
    axisY3->append(" ", 140);
    axisY3->setShadesPen(Qt::NoPen);
    axisY3->setShadesBrush(QBrush(QColor(0x99, 0xcc, 0xcc, 0x55)));
    axisY3->setShadesVisible(true);

    chart->addAxis(axisY3, Qt::AlignRight);
    series->attachAxis(axisY3);


    m_pWidget->setWindowFlag(Qt::Tool);
    m_pWidget->setWindowTitle("[Отчет] Диаметр волос");
    QVBoxLayout *pVertBoxLayout = new QVBoxLayout(m_pWidget);
    pVertBoxLayout->addWidget(chartView);


//    length->setNum(0);
//    QHBoxLayout *pHorBoxLayout = new QHBoxLayout(this)


    QLabel *pLabel;
    pLabel = new QLabel(QString("Средний диаметр всех терминальных волос: %1.").arg(_p_ie_global_data->convertUnitedWithForamtF(computeData.diameter.midDiameterTermin, UnitType::um)), m_pWidget);
    pVertBoxLayout->addWidget(pLabel);
    pLabel = new QLabel(QString("Всего измерено %1 волос.").arg(computeData.diameter.hairQuantity), m_pWidget);
    pVertBoxLayout->addWidget(pLabel);
    pLabel = new QLabel(QString("Средний диаметр всех волос = %1.").arg(_p_ie_global_data->convertUnitedWithForamtF(computeData.diameter.midDiameterAll, UnitType::um)), m_pWidget);
    pVertBoxLayout->addWidget(pLabel);
    pLabel = new QLabel(QString("Измерено терминальных волос: %1% (%2 на %3 кв.%4.)")
                        .arg(computeData.diameter.percentTermin)
                        .arg(computeData.diameter.terminalQuantity)
                        .arg(computeData.diameter.squareInUnitedUnits)
                        .arg(UnitTypeTitle[_p_ie_global_data->getUnitType()]), m_pWidget
            );
    pVertBoxLayout->addWidget(pLabel);
    pLabel = new QLabel(QString("Измерено веллусных волос: %1% (%2 на %3 кв.%4.)")
                        .arg(computeData.diameter.percentVellus)
                        .arg(computeData.diameter.vellusQuantity)
                        .arg(computeData.diameter.squareInUnitedUnits)
                        .arg(UnitTypeTitle[_p_ie_global_data->getUnitType()]), m_pWidget
            );
    pVertBoxLayout->addWidget(pLabel);
    pLabel = new QLabel(QString("\nСреди терминальных:\nТонких волос (%1 – %2 %3): %4%")
                            .arg(_p_ie_global_data->convertUnitedWithForamtF(_p_ie_global_data->getThreshold_TW(), UnitType::um))
                            .arg(_p_ie_global_data->convertUnitedWithForamtF(_p_ie_global_data->getThreshold_thinHair(), UnitType::um))
                            .arg(UnitTypeTitle[UnitType::um])
                            .arg(computeData.diameter.percentThinHair), m_pWidget
                );
    pVertBoxLayout->addWidget(pLabel);
    pLabel = new QLabel(QString("Средних волос (%1 – %2 %3): %4%")
                            .arg(_p_ie_global_data->convertUnitedWithForamtF(_p_ie_global_data->getThreshold_thinHair(), UnitType::um))
                            .arg(_p_ie_global_data->convertUnitedWithForamtF(_p_ie_global_data->getThreshold_mediumHair(), UnitType::um))
                            .arg(UnitTypeTitle[UnitType::um])
                            .arg(computeData.diameter.percentMediumHair), m_pWidget
                );
    pVertBoxLayout->addWidget(pLabel);
    pLabel = new QLabel(QString("Анизотрихоз ???"), m_pWidget
                );
    pVertBoxLayout->addWidget(pLabel);

    m_pWidget->setMinimumWidth(500);
    m_pWidget->setMinimumHeight(500);
    m_pWidget->show();
}
