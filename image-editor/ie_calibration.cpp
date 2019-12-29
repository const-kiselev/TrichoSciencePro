#include "ie_calibration.h"

// Задачи:
// реализовать только одну линию на поле!!!!! реализовать у
// линии установку вида и параметров отображения для этого необходимо в модель добавить управление слоями итд

IE_ToolCalibration::IE_ToolCalibration(QWidget *parent, qreal mIndex) :
    QWidget(parent)
{
    line = nullptr;
    measureIndex = mIndex;

    pModel = new IE_Model();
    pView = new IE_View(pModel);

    pToolController = pView->getToolsController();
    pToolController->setToolSetType(ToolSet::CallibrationToolSet);
    pToolController->setOrientation(Qt::Vertical);
    pModel->setPToolCnt(pToolController);
    connect(pToolController, &ToolsController::startUsingNewTool,
            pModel, &IE_Model::addLayerViaToolCnt);


    setWindowTitle("Калибровка / настройка масштаба");
    setWindowFlags(Qt::Tool);

    setLayout(new QBoxLayout(QBoxLayout::LeftToRight));
    static_cast<QBoxLayout*>(layout())->addWidget(pToolController);
    static_cast<QBoxLayout*>(layout())->addWidget(pView,3);

    QBoxLayout *pRightLayout = new QBoxLayout(QBoxLayout::TopToBottom);

    QPushButton *pButton = new QPushButton("Загрузить изображение");
    pRightLayout->addWidget(pButton);
    connect(pButton, &QPushButton::released, this, &IE_ToolCalibration::openImage);


    QLabel *pLabel = new QLabel("Количество единиц в миллимтерах:");
    QSpinBox *pSpinBox = new QSpinBox();
    //pSpinBox->setEnabled(false);
    pSpinBox->setRange(1,100);
    pSpinBox->setWrapping(true); // циклический режим
    pSpinBox->setButtonSymbols(QSpinBox::PlusMinus);
    pLabel->setBuddy(pSpinBox);
    pRightLayout->addWidget(pLabel);
    pRightLayout->addWidget(pSpinBox);

    connect(pSpinBox, SIGNAL(valueChanged(int)),
            this , SLOT(calculateMeasure(int)));

    connect(this, &IE_ToolCalibration::itemsInModelWasChanged,
            [pSpinBox,pLabel, this](){
        this->calculateMeasure(pSpinBox->value());
    });

    pLabel = new QLabel("В 1 мм ");
    QHBoxLayout *pHorBoxLayout = new QHBoxLayout(this);
    pHorBoxLayout->addWidget(pLabel);
    pMeasureLineEdit = new QLineEdit();
    pMeasureLineEdit->setText(QString().number(mIndex));
    pLabel->setBuddy(pMeasureLineEdit);
    pHorBoxLayout->addWidget(pMeasureLineEdit);
    pHorBoxLayout->addWidget(new QLabel(" пикселей."));
    pRightLayout->addItem(pHorBoxLayout);


    pButton = new QPushButton("Сохранить");
//    pButton->setEnabled(false);
    pRightLayout->addStretch(1);
    pRightLayout->addWidget(pButton);


    connect(pButton, &QPushButton::released, [this](){
        emit(this->saveChangedMeasureIndex());

    });

    static_cast<QBoxLayout*>(layout())->addWidget(pView,4);
    static_cast<QBoxLayout*>(layout())->addLayout(pRightLayout, 1);
    this->resize(800,500);

    connect(pModel, &IE_Model::changed,
            this, &IE_ToolCalibration::modelItemsControl);

}

qreal IE_ToolCalibration::getMeasureIndex() const
{
    return pMeasureLineEdit->text().toDouble();
}



// смысл какой: при изменении модели данная функция реагирует
// и проверяет весь список элементов на моделе.
// Если на ней больше одной линии, то остается самая новая
void IE_ToolCalibration::modelItemsControl()
{
    QList<IE_ModelLayer *> layersList = pModel->getLayersList();
    QList<IE_ModelLayer *>::iterator prevLine =layersList.end() , currItem = layersList.end();
    for(QList<IE_ModelLayer *>::iterator listIter = layersList.begin();listIter!=layersList.end();listIter++ )
    {
        currItem = listIter;
        if((*currItem)->getToolType() == ToolType::Ruler)
        {   if(prevLine!=layersList.end())
            {
                pModel->removeLayer(prevLine);
                layersList.erase(prevLine);
            }
            prevLine = currItem;

            line = static_cast<QGraphicsLineItem*>( (QGraphicsItem*)(prevLine.i->t()->parentItem()) );
        }
    }

    emit(itemsInModelWasChanged());
}

void IE_ToolCalibration::openImage()
{

    pModel->initAsNewModel(TSP_PatientData(), IEM_type::None, IE_ProfileType::MeasureIndex);
}

qreal IE_ToolCalibration::calculateMeasure(int measureLenght)
{
    if(!line)
        return 0;
    qreal answer = line->line().length()/measureLenght;
    pMeasureLineEdit->setText(QString().number(answer));
    return answer;

}


// ------- EVENTS



// ------- END EVENTS -------

