#ifndef IE_COMPUTE_H
#define IE_COMPUTE_H

#include <QtWidgets>

#include "ie_toolController.h"


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
 *          Нет необходимости в сущестоввании эксземпляра класса все время.
 * Так что при необходимости лучше создавать экземпляр класса, выполнить все вычисления и удалить его.
 */

class IE_Compute : public QObject
{
    Q_OBJECT
public:
    explicit IE_Compute(_global_ie * pieg,
                        QList<IE_ModelLayer*> * pll,
                        QObject *parent = nullptr
                        );
    enum ComputeType
    {
        HairDensity = 1,
        HairDiameter = (1 << 1)
    };
    //! Структура, которая отвечает за информацию о слое, которой будет достаточно для вычислений.
    struct LayerComputeData
    {
        qreal   width,
                length;
        ToolType toolType;
        LayerComputeData(qreal w, qreal l, ToolType tt):width(w),
                                                        length(l),
                                                        toolType(tt){}
    };

    //! Структура входных данных
    struct InputData
    {
        ComputeType computeType;
        qreal   squareInPixels;
        QList<LayerComputeData> layerList;
        InputData()
        {
            squareInPixels = qreal(0);
        }
    };



    //! Структура данных для отчета `ПЛОТНОСТЬ ВОЛОС`
    struct HairDensityReportData
    {
        ComputeType computeType;
        InputData inputData;
        uint    hairQuantity,       /**< кол-во волос */
                terminalQuantity,   /**< кол-во терминальных волос */
                vellusQuantity,     /**< кол-во веллусных волос */
                percentTermin,
                percentVellus;
        qreal   squareInCm2,        /**< площадь области вычислений */
                totalHairQuantityInCm2, /**< кол-во волос на см^2 */
                terminHairInCm2,    /**< кол-во терминальных волос на см^2 */
                vellusHairInCm2;    /**< кол-во веллусных волос на см^2 */

        HairDensityReportData(InputData id = InputData()):  computeType(ComputeType::HairDensity),
                                                            inputData(id)
        {
            hairQuantity = terminalQuantity = vellusQuantity
                    = percentTermin = percentVellus = 0;
            squareInCm2 = totalHairQuantityInCm2 =
                    terminHairInCm2 = vellusHairInCm2 = qreal(0);
        }
    };
    //! Структура данных для отчета `ДИАМЕТР ВОЛОС`
    struct HairDiameterReportData
    {
        ComputeType computeType;
        InputData inputData;
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
        HairDiameterReportData(InputData id = InputData()): computeType(ComputeType::HairDiameter),
                                                            inputData(id)
        {
            hairQuantity = terminalQuantity = vellusQuantity
                    = thinHairQ = mediumHairQ = percentTermin
                    = percentVellus
                    = 0;
            squareInUnitedUnits = sumOfDiametersOfAllHair
                    = sumOfDiametersOfTerminHair = qreal(0);
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
            computeHairDensity(InputData id);
    HairDiameterReportData
            computeHairDiameter(InputData id);


    //! Стоит обратить внимание, что вычисление данных со всех `точек зрения` (в них могут быть определены другие меры в качествве стандартных) происходит в первом документе, то есть он определяет меру, следовательно все поступаемые данные долдны быть в пикселях.
    //!
    //!
    OutputData compute(InputData id);
    OutputData compute(ComputeType compType);
    InputData getInputData() const;

signals:


public slots:


private:
    _global_ie *_p_ie_global_data;
    QList<IE_ModelLayer*>   *layersList;

};

#endif // IE_COMPUTE_H
