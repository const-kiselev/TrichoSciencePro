#ifndef IE_COMPUTE_H
#define IE_COMPUTE_H

#include <QtWidgets>

#include "ie_fieldOfView_controller.h"
#include "ie_header.h"

/*!
 *      \brief Класс вычислений.
 *
 *      Данный класс отвечает за вычисления.
 *
 *      Доступны диалоговые окна с отчетами: плотность, диаметр, длина волос.
 *      Дотсупно построение графиков и выгрузка отчетов.
 *
 *      Логика: класс необходим как для вычислений в самом IE, и для получения данных для составления отчета. Так как отчеты предусмотрены и для обследований, в которых несколько полей зрения, то необходимо получить данные со всех полей зрений.
 *
 *      Проблематика заключается в том, что если несколько полей зрения, площади вычисляются с помощью
 *
 *      Политика работы с классом:
 *          Класс со статичными методами для вычислений.
 *          Нет необходимости в сущестоввании эксземпляра класса все время.
 * Так что при необходимости лучше создавать экземпляр класса, выполнить все вычисления и удалить его.
 */

class IE_Compute : public QObject
{
    Q_OBJECT
public:
    explicit IE_Compute(_global_ie * pieg,
                        IE_FVConstList fieldOfViewListConstPtr,
                        QObject *parent = nullptr
                        );
    enum ComputeType
    {
        HairDensity = 1,
        HairDiameter = (1 << 1)
    };



    //! Структура входных данных
    struct InputData
    {
        // вычисление площади происходит каждый раз при выполнении методов compute
        IE_ReportType reportType;
        ComputeType computeType;
        qreal   squareInPixels;
        IE_FVConstList fieldOfViewListConstIter;
        InputData(IE_FVConstList fieldOfViewListConstPtr,
                  IE_ReportType rt = IE_ReportType::Full
                )
        {
            reportType = rt;
            switch (reportType)
            {
                case IE_ReportType::Full:
                computeType = (ComputeType) (ComputeType::HairDensity |
                                                ComputeType::HairDiameter);
                break;
                case IE_ReportType::Trichoscopy:
                computeType = (ComputeType) (ComputeType::HairDensity |
                                                ComputeType::HairDiameter);
                break;
            }
            squareInPixels = -1;
            this->fieldOfViewListConstIter = fieldOfViewListConstIter;
        }
    };



    //! Структура данных для отчета `ПЛОТНОСТЬ ВОЛОС`
    struct HairDensityReportData
    {
        ComputeType computeType;
        qreal   areaValue;
        uint    hairQuantity,       /**< кол-во волос */
                terminalQuantity,   /**< кол-во терминальных волос */
                vellusQuantity,     /**< кол-во веллусных волос */
                percentTermin,
                percentVellus;
        qreal   squareInCm2,        /**< площадь области вычислений */
                totalHairQuantityInCm2, /**< кол-во волос на см^2 */
                terminHairInCm2,    /**< кол-во терминальных волос на см^2 */
                vellusHairInCm2;    /**< кол-во веллусных волос на см^2 */

        HairDensityReportData():  computeType(ComputeType::HairDensity)
        {
            hairQuantity = terminalQuantity = vellusQuantity
                    = percentTermin = percentVellus = 0;
            squareInCm2 = totalHairQuantityInCm2 =
                    terminHairInCm2 = vellusHairInCm2 = areaValue = qreal(0);
        }
    };
    //! Структура данных для отчета `ДИАМЕТР ВОЛОС`
    struct HairDiameterReportData
    {
        ComputeType computeType;
        qreal   areaValue;
        uint    hairQuantity,       /**< кол-во волос */
                terminalQuantity,   /**< кол-во терминальных волос */
                vellusQuantity,     /**< кол-во веллусных волос */
                thinHairQ,
                mediumHairQ,
                percentTermin,
                percentVellus,
                percentThinHair,
                percentMediumHair;
        qreal   squareInUnitedUnits,
                sumOfDiametersOfAllHair,
                sumOfDiametersOfTerminHair,
                midDiameterAll,
                midDiameterTermin;
        QList<qreal>
                diametersListInUm;
        HairDiameterReportData(): computeType(ComputeType::HairDiameter)
        {
            hairQuantity = terminalQuantity = vellusQuantity
                    = thinHairQ = mediumHairQ = percentTermin
                    = percentVellus
                    = 0;
            squareInUnitedUnits = sumOfDiametersOfAllHair
                    = sumOfDiametersOfTerminHair = areaValue = qreal(0);
        }
    };


    //! Структура выходных данных
    struct OutputData
    {
        ComputeType computeType;
        HairDensityReportData density;
        HairDiameterReportData diameter;
    };


    qreal   computeSquareInPixels() const;
    HairDensityReportData
            computeHairDensity();
    HairDiameterReportData
            computeHairDiameter();


    //! Стоит обратить внимание, что вычисление данных со всех `точек зрения` (в них могут быть определены другие меры в качествве стандартных) происходит в первом документе, то есть он определяет меру, следовательно все поступаемые данные долдны быть в пикселях.
    //!
    //!
    static OutputData makeCompute(_global_ie * pieg,
                                  InputData id,
                                  QObject *parent = nullptr
                                );
//    OutputData compute(ComputeType compType);
//    static OutputData compute(_global_ie * pieg, ComputeType compType);
//    InputData getInputData() const;

signals:


public slots:


private:
    _global_ie *_p_ie_global_data;
    IE_FVConstList m_fieldOfViewListConstPtr;
    OutputData compute(IE_Compute::ComputeType compType);

};

#endif // IE_COMPUTE_H
