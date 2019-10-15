#include "ie_report.h"

IE_Report::IE_Report(_global_ie *pieg) :  _p_ie_global_data(pieg)
{

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

    QWidget* pTmp = new QWidget();
    pTmp->setWindowFlag(Qt::Tool);
    pTmp->setWindowTitle("[Отчет] Плотность волос");
    QVBoxLayout *pVertBoxLayout = new QVBoxLayout(pTmp);
    pVertBoxLayout->addWidget(chartView);


//    length->setNum(0);
//    QHBoxLayout *pHorBoxLayout = new QHBoxLayout(this)






    QLabel *pLabel;
//    pLabel->setNum(i);
    pLabel = new QLabel(QString("Всего измеренно волос: %1. Это %2 на см.кв.")  .arg(computeData.density.hairQuantity)
                                                                                .arg(computeData.density.totalHairQuantityInCm2), pTmp);
    pVertBoxLayout->addWidget(pLabel);
    pLabel = new QLabel(QString("Из них терминальных волос: %1. Это %2 на см.кв. или %3 %").arg(computeData.density.terminalQuantity).
                                                                                            arg(computeData.density.terminHairInCm2).
                                                                                            arg(computeData.density.percentTermin), pTmp);
    pVertBoxLayout->addWidget(pLabel);
    pLabel = new QLabel(QString("Веллус: %1. Это %2 на см.кв. или %3 %"). arg(computeData.density.vellusQuantity).arg(computeData.density.vellusHairInCm2).
                                                                                            arg(computeData.density.percentVellus), pTmp);
    pVertBoxLayout->addWidget(pLabel);
    pLabel = new QLabel(QString("Площадь изображения: %1 %2.кв.").arg(computeData.density.squareInCm2).arg(UnitTypeTitle[UnitType::cm]), pTmp);
    pVertBoxLayout->addWidget(pLabel);

    pTmp->setMinimumWidth(500);
    pTmp->setMinimumHeight(500);
    pTmp->show();
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

    QWidget* pTmp = new QWidget();
    pTmp->setWindowFlag(Qt::Tool);
    pTmp->setWindowTitle("[Отчет] Диаметр волос");
    QVBoxLayout *pVertBoxLayout = new QVBoxLayout(pTmp);
    pVertBoxLayout->addWidget(chartView);


//    length->setNum(0);
//    QHBoxLayout *pHorBoxLayout = new QHBoxLayout(this)


    QLabel *pLabel;
    pLabel = new QLabel(QString("Средний диаметр всех терминальных волос: %1.").arg(_p_ie_global_data->convertUnitedWithForamtF(computeData.diameter.midDiameterTermin, UnitType::um)), pTmp);
    pVertBoxLayout->addWidget(pLabel);
    pLabel = new QLabel(QString("Всего измерено %1 волос.").arg(computeData.diameter.hairQuantity), pTmp);
    pVertBoxLayout->addWidget(pLabel);
    pLabel = new QLabel(QString("Средний диаметр всех волос = %1.").arg(_p_ie_global_data->convertUnitedWithForamtF(computeData.diameter.midDiameterAll, UnitType::um)), pTmp);
    pVertBoxLayout->addWidget(pLabel);
    pLabel = new QLabel(QString("Измерено терминальных волос: %1% (%2 на %3 кв.%4.)")
                        .arg(computeData.diameter.percentTermin)
                        .arg(computeData.diameter.terminalQuantity)
                        .arg(computeData.diameter.squareInUnitedUnits)
                        .arg(UnitTypeTitle[_p_ie_global_data->getUnitType()]), pTmp
            );
    pVertBoxLayout->addWidget(pLabel);
    pLabel = new QLabel(QString("Измерено веллусных волос: %1% (%2 на %3 кв.%4.)")
                        .arg(computeData.diameter.percentVellus)
                        .arg(computeData.diameter.vellusQuantity)
                        .arg(computeData.diameter.squareInUnitedUnits)
                        .arg(UnitTypeTitle[_p_ie_global_data->getUnitType()]), pTmp
            );
    pVertBoxLayout->addWidget(pLabel);
    pLabel = new QLabel(QString("\nСреди терминальных:\nТонких волос (%1 – %2 %3): %4%")
                            .arg(_p_ie_global_data->convertUnitedWithForamtF(_p_ie_global_data->getThreshold_TW(), UnitType::um))
                            .arg(_p_ie_global_data->convertUnitedWithForamtF(_p_ie_global_data->getThreshold_thinHair(), UnitType::um))
                            .arg(UnitTypeTitle[UnitType::um])
                            .arg(computeData.diameter.percentThinHair), pTmp
                );
    pVertBoxLayout->addWidget(pLabel);
    pLabel = new QLabel(QString("Средних волос (%1 – %2 %3): %4%")
                            .arg(_p_ie_global_data->convertUnitedWithForamtF(_p_ie_global_data->getThreshold_thinHair(), UnitType::um))
                            .arg(_p_ie_global_data->convertUnitedWithForamtF(_p_ie_global_data->getThreshold_mediumHair(), UnitType::um))
                            .arg(UnitTypeTitle[UnitType::um])
                            .arg(computeData.diameter.percentMediumHair), pTmp
                );
    pVertBoxLayout->addWidget(pLabel);
    pLabel = new QLabel(QString("Анизотрихоз ???"), pTmp
                );
    pVertBoxLayout->addWidget(pLabel);

    pTmp->setMinimumWidth(500);
    pTmp->setMinimumHeight(500);
    pTmp->show();
}
