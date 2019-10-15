#include "ie_fieldOfView_controller.h"

IE_FieldOfView_Controller::IE_FieldOfView_Controller(QObject *parent) : QObject(parent), m_quantityOfFields(0)
{
    //
}

int IE_FieldOfView_Controller::read(const QJsonObject &json)
{
    QJsonArray fvArray = json["fieldOfViewArray"].toArray();
    changeQuantity( (Quantity)(fvArray.size()+1));
    for (int fvIndex = 0; fvIndex < fvArray.size(); ++fvIndex)
    {
        QJsonObject fvObject = fvArray[fvIndex].toObject();
        IE_FieldOfView * tmp_pfv = new IE_FieldOfView();
        tmp_pfv->read(fvObject);
        m_fieldOfViewList.append(tmp_pfv);
    }
    return 0;
}

int IE_FieldOfView_Controller::write(QJsonObject &json) const
{
    QJsonArray fvArray;
    foreach (IE_FieldOfView * pfv, m_fieldOfViewList)
    {
        QJsonObject fvObject;
        pfv->write(fvObject);
        fvArray.append(fvObject);
    }
    json["fieldOfViewArray"] = fvArray;
    return 0;
}

void IE_FieldOfView_Controller::init(IE_FieldOfView_Controller::Quantity quantity)
{
    m_quantityOfFields = quantity;
    for(int i = 0; i < m_quantityOfFields; i++)
    {
        IE_FieldOfView * tmpFV = new IE_FieldOfView();
        m_fieldOfViewList.append(tmpFV);
    }
}

uint IE_FieldOfView_Controller::getQuantity()const
{
    return m_quantityOfFields;
}

void IE_FieldOfView_Controller::initDockWidget()
{

}

QDockWidget *IE_FieldOfView_Controller::getDockWidget()const
{

}

QDialog::DialogCode IE_FieldOfView_Controller::makeDialogForSetupAsNew()
{
    QDialog * locPDialog = new QDialog();
    QVBoxLayout * pMainLayout = new QVBoxLayout(locPDialog);
    pMainLayout->addWidget(new QLabel("Выберите обследуемые изображения для каждого поля зрения:",
                                      locPDialog));

//    QString paths[quantityMax];

    for(int i = 0; i < m_quantityOfFields; i++)
    {
        //! \bug ОШИБКА БУДЕТ СТО ПРО!!!!!!
        QString fileName = m_fieldOfViewList.at(i)->getMainImageFileInfo().fileName();
        if(fileName.isEmpty())
            fileName = "Выберите изображение";

        pMainLayout->addWidget(new QLabel(QString("Поле зрения %1:").arg(i+1),locPDialog));
        QLabel * pLableFileName = new QLabel(fileName,locPDialog);
        pMainLayout->addWidget(pLableFileName);
        QPushButton * pLoadButton = new QPushButton("Загрузить изображение", locPDialog);
        connect(pLoadButton, &QPushButton::clicked, [this, i, pLableFileName]()
        {
            QString filePath =
                    QFileDialog::getOpenFileName(nullptr,
                                                 "Выбор изображения",
                                                 QStandardPaths::writableLocation(
                                                     QStandardPaths::PicturesLocation),
                                                 "*.png *.jpg"
                                                );

            pLableFileName->setText(filePath);
            m_fieldOfViewList.at(i)->setMainImage(filePath);
        });
    }


    locPDialog->setLayout(pMainLayout);
    locPDialog->setModal(true);

    locPDialog->show();
    int answer =  locPDialog->exec();

    if(answer == QDialog::Accepted)
    {

    }
    delete locPDialog;
    return (QDialog::DialogCode)answer;
}

IE_FieldOfView_Controller::Quantity IE_FieldOfView_Controller::getStandartQuantity(IE_ProfileType profileType)
{
    switch (profileType)
    {
    case IE_ProfileType::Trichogram: return Quantity::Two;
    case IE_ProfileType::Trichoscopy: return Quantity::Two;
    case IE_ProfileType::Phototrichogram: return Quantity::Two;
    default: return Quantity::One;
    }
}


