#include "ie_fieldOfView_controller.h"
#include "ie_fieldOfView_infoWidget.h"

IE_FieldOfView_Controller::IE_FieldOfView_Controller(QList<IE_ModelLayer*>*ll,
                                                     _global_ie * pieg,
                                                     QObject *parent
                                                     ) :    QObject(parent),
                                                            m_quantityOfFields(0),
                                                            m_inited(false),
                                                            m_activeFVIndex(-1),
                                                            layersList(ll),
                                                            m_p_ie_global_data(pieg)
{
    m_pImageBaseCnt = nullptr;
    m_pInfoWidget = new IE_FieldOfView_ControllerInfoWidget();
    connect(m_pInfoWidget, &IE_FieldOfView_ControllerInfoWidget::quantityWasChanged, [this](int q)
    {
        changeQuantity((Quantity)q);
    });
    m_activeFVIndex = -1;
    connect(m_pInfoWidget, &IE_FieldOfView_ControllerInfoWidget::activeFVWasChanged, [this](int index)
    {
        changeActiveFieldOfView(index);
    });
    connect(m_pInfoWidget, &IE_FieldOfView_ControllerInfoWidget::hideAllElementsOnActiveFV, [this]()
    {
        m_fieldOfViewList.at(m_activeFVIndex)->hideIntersectedLayersWithFv();
    });
    connect(m_pInfoWidget, &IE_FieldOfView_ControllerInfoWidget::showAllElementsOnActiveFV, [this]()
    {
        m_fieldOfViewList.at(m_activeFVIndex)->showIntersectedLayersWithFv();
    });
    connect(m_pInfoWidget, &IE_FieldOfView_ControllerInfoWidget::removeAllElementsOnActiveFV, [this]()
    {
        m_fieldOfViewList.at(m_activeFVIndex)->removeIntersectedLayersWithFv();
    });
    connect(this, &IE_FieldOfView_Controller::activeFVLayerListWasUpdated, m_pInfoWidget, &IE_FieldOfView_ControllerInfoWidget::updateActiveFVLayerList);
    connect(m_pInfoWidget, &IE_FieldOfView_ControllerInfoWidget::makeInit,[this]()
    {
       makeInitDialog();
    });

}

IE_FieldOfView_Controller::~IE_FieldOfView_Controller()
{
    while(!m_fieldOfViewList.isEmpty())
    {
        delete m_fieldOfViewList.at(m_fieldOfViewList.size()-1);
        m_fieldOfViewList.removeAt(m_fieldOfViewList.size()-1);
    }
}

int IE_FieldOfView_Controller::read(const QJsonObject &json)
{
    QJsonArray fvArray = json["fieldOfViewArray"].toArray();
    if(fvArray.size() < 1 || fvArray.size() == 5 || fvArray.size() > 6)
    {
        qWarning() << "Invalid number of field of view = " << fvArray.size();
        return 1;
    }
//    removeAllFieldOfView();
    init((Quantity)(fvArray.size()));
    int answer = 0;
    for (int fvIndex = 0; fvIndex < fvArray.size(); ++fvIndex)
    {
        QJsonObject fvObject = fvArray[fvIndex].toObject();
        answer = m_fieldOfViewList.at(fvIndex)->read(fvObject);
        if(answer)
            return answer;

        if(IE_ImageBaseCnt::containsImageBaseUserChoice(fvObject))
        {
            initImageBase();
            m_pImageBaseCnt->readUserChoice(fvObject, fvIndex);
        }
    }
    m_activeFVIndex = 0;
    if(m_pImageBaseCnt)
        m_pImageBaseCnt->setUserChoiceListSize(m_quantityOfFields);
    changeActiveFieldOfView(m_activeFVIndex);
    return 0;
}

int IE_FieldOfView_Controller::write(QJsonObject &json) const
{
    QJsonArray fvArray;
    int i = 0;
    foreach (IE_FieldOfView * pfv, m_fieldOfViewList)
    {
        QJsonObject fvObject;
        pfv->write(fvObject);
        if(m_pImageBaseCnt)
            m_pImageBaseCnt->writeUserChoice(fvObject, i++);
        fvArray.append(fvObject);
    }
    json["fieldOfViewArray"] = fvArray;
    return 0;
}

void IE_FieldOfView_Controller::init(IE_FieldOfView_Controller::Quantity quantity)
{
    if(m_quantityOfFields)
    {
        qWarning() << "Init was done earlier. Redirect to changeQuantity(...).";
        changeQuantity(quantity);
        return;
    }

    m_pInfoWidget->init(quantity);
    m_activeFVIndex = 0;
    for(int i = 0; i < (int)quantity; i++)
    {
        addFieldOfView(i);
    }
    changeActiveFieldOfView(m_activeFVIndex);
    m_inited = true;
    if(m_pImageBaseCnt)
        m_pImageBaseCnt->setUserChoiceListSize(m_quantityOfFields);
}

int IE_FieldOfView_Controller::initImageBase()
{
    if(m_pImageBaseCnt)
        return 0;
    m_pImageBaseCnt = new IE_ImageBaseCnt();
    m_pImageBaseCnt->setUserChoiceListSize(m_quantityOfFields);
    connect(m_pImageBaseCnt, &IE_ImageBaseCnt::needLayerTitlesList, [=]()
    {
        if(m_activeFVIndex < 0)
            return;
        m_pImageBaseCnt->makeCorellation(m_fieldOfViewList[m_activeFVIndex]->getLayerTitleList());
    });
    return 0;
}

QDockWidget *IE_FieldOfView_Controller::getImageBaseDockWidget()
{
    if(!m_pImageBaseCnt)
        return nullptr;
    return m_pImageBaseCnt->getDockWidgetWithAllImages();
}

QDialog::DialogCode IE_FieldOfView_Controller::makeInitDialog()
{
    if(m_inited)
        return QDialog::Rejected;


    QDialog * locPDialog = new QDialog();
    QVBoxLayout * pMainLayout = new QVBoxLayout(locPDialog);
    QLabel *pLabel = new QLabel("Количество полей зрения:", locPDialog);

    QComboBox    *pcboQuiantityFV = new QComboBox(locPDialog);
    pLabel->setBuddy(pcboQuiantityFV);
    QStringList  lst;

    lst << QString().number(IE_FieldOfView_Controller::Quantity::One)
        << QString().number(IE_FieldOfView_Controller::Quantity::Two)
        << QString().number(IE_FieldOfView_Controller::Quantity::Three)
        << QString().number(IE_FieldOfView_Controller::Quantity::Four)
        << QString().number(IE_FieldOfView_Controller::Quantity::Six);
    pcboQuiantityFV->addItems(lst);
    pcboQuiantityFV->setCurrentIndex(0);

    pMainLayout->addWidget(pLabel);
    pMainLayout->addWidget(pcboQuiantityFV);

    QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                         | QDialogButtonBox::Cancel,locPDialog);
    pMainLayout->addWidget(buttonBox);
    connect(buttonBox, &QDialogButtonBox::accepted, locPDialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, locPDialog, &QDialog::reject);

    locPDialog->setLayout(pMainLayout);
    locPDialog->setModal(true);

    locPDialog->show();
    int answer =  locPDialog->exec();

    if(answer == QDialog::Accepted)
    {
        init((IE_FieldOfView_Controller::Quantity)pcboQuiantityFV->currentText().toInt());
        makeDialogForSetupAsNew();
    }
    delete locPDialog;
    return (QDialog::DialogCode)answer;
}

void IE_FieldOfView_Controller::changeQuantity(IE_FieldOfView_Controller::Quantity q)
{
    if( (int)q == m_quantityOfFields )
        return;
    //! \todo не забыть разъединить сигнал!?
    if( (int)q < m_quantityOfFields )
    {
        // FV сам удаляет все слои, которые находятся на нем
        for(int i = m_quantityOfFields-1; i >= (int)q; i--)
            removeFieldOfView(i);
    }
    else
    {
        for(int i = m_quantityOfFields; i < (int)q; i++)
            addFieldOfView(i);
        makeDialogForSetupAsNew();
    }

    relocateAllFieldOfView();
    m_pInfoWidget->changeQuantity((Quantity) m_quantityOfFields);
    if(m_pImageBaseCnt)
        m_pImageBaseCnt->setUserChoiceListSize(m_quantityOfFields);
}

uint IE_FieldOfView_Controller::getQuantity()const
{
    return m_quantityOfFields;
}

QWidget *IE_FieldOfView_Controller::getInfoWidget()const
{
    return m_pInfoWidget;
}

void IE_FieldOfView_Controller::relocateAllFieldOfView()
{
    QPointF followPointF(0,0);
    qreal highest = 0;
    switch ((Quantity)m_quantityOfFields)
    {
    case Quantity::One:
    case Quantity::Two:
    case Quantity::Three:
    {
        for(int i = 0; i < m_quantityOfFields; i++)
        {
            IE_FieldOfView * pfv = m_fieldOfViewList.at(i);
            pfv->setPos(followPointF);
            QRectF layerRect = pfv->getRect();
            followPointF.setX( followPointF.x() + layerRect.width() );
        }
        break;
    }
    case Quantity::Four:
    case Quantity::Six:
    {
        for(int i = 0; i < m_quantityOfFields; i++)
        {
            IE_FieldOfView * pfv = m_fieldOfViewList.at(i);
            pfv->setPos(followPointF);
            QRectF layerRect = pfv->getRect();
            followPointF.setX( followPointF.x() + layerRect.width() );
            if((Quantity)m_quantityOfFields == Quantity::Four)
            {
                if(i < 2)
                    if(layerRect.height() > highest)
                        highest = layerRect.height();
                if(i == 1)
                {
                    followPointF.setY( followPointF.y() + highest );
                    followPointF.setX(0);
                }

            }
            else if((Quantity)m_quantityOfFields == Quantity::Six)
            {
                if(i < 3)
                    if(layerRect.height() > highest)
                        highest = layerRect.height();
                if(i == 2)
                {
                    followPointF.setY( followPointF.y() + highest );
                    followPointF.setX(0);
                }
            }
        }
        break;
    }
    }
    emit boundingRectWasChanged(getBoundingRectOfAllFieldOfView());
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
        QString fileName = m_fieldOfViewList.at(i)->getMainImageFileInfo().fileName();
        if(fileName.isEmpty())
            fileName = "Выберите изображение";

        pMainLayout->addWidget(new QLabel(QString("Поле зрения %1:").arg(i+1),locPDialog));
        QLabel * pLableFileName = new QLabel(fileName,locPDialog);
        pMainLayout->addWidget(pLableFileName);
        QPushButton * pLoadButton = new QPushButton("Загрузить изображение", locPDialog);
        pMainLayout->addWidget(pLoadButton);
        connect(pLoadButton, &QPushButton::clicked, [this, i, pLableFileName]()
        {
            if(m_fieldOfViewList.at(i)->setMainImage())
            {
                QMessageBox::warning(nullptr, "Ошибка", "Не удалось загрузить изображение. Попробуйте еще раз.");
                return;
            }
            pLableFileName->setText(m_fieldOfViewList.at(i)
                                        ->getMainImageFileInfo().fileName());
        });
    }

    QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                         | QDialogButtonBox::Cancel,locPDialog);
    pMainLayout->addWidget(buttonBox);
    connect(buttonBox, &QDialogButtonBox::accepted, locPDialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, locPDialog, &QDialog::reject);


    locPDialog->setLayout(pMainLayout);
    locPDialog->setModal(true);

    locPDialog->show();
    int answer =  locPDialog->exec();

    if(answer == QDialog::Accepted)
    {
        relocateAllFieldOfView();
    }
    delete locPDialog;
    return (QDialog::DialogCode)answer;
}

QRectF IE_FieldOfView_Controller::getBoundingRectOfAllFieldOfView()
{
    QRectF answer;
    answer.setTopLeft(QPointF(0,0));
    answer.setBottomRight(QPointF(0,0));
    //      находим САМУЮ нижнюю правую точку
    for(int i = 0; i < m_quantityOfFields; i++)
    {
        IE_FieldOfView * pfv = m_fieldOfViewList.at(i);
        qDebug() << "!!!!!!! " << pfv->getRect();
        if(pfv->getRect().bottomRight().x() > answer.bottomRight().x())
            answer.setWidth(pfv->getRect().bottomRight().x());
        if(pfv->getRect().bottomRight().y() > answer.bottomRight().y())
            answer.setHeight(pfv->getRect().bottomRight().y());
    }
    qDebug() << answer;
    return answer;
}

void IE_FieldOfView_Controller::changeActiveFieldOfView(int index)
{
    if(index > ((int)m_quantityOfFields)-1)
        return;
    m_activeFVIndex = index;
    emit boundingRectWasChanged( m_fieldOfViewList.at(index)->getRect() );
    m_pInfoWidget->changeActiveFV(index, m_fieldOfViewList.at(index)->getNote());
    checkLayerList();
    if(m_pImageBaseCnt)
        m_pImageBaseCnt->setCurrentUserChoiceList(index);
}

QList<IE_ModelLayer *> IE_FieldOfView_Controller::getActiveFieldOfViewLayerList()
{
    if(!m_quantityOfFields)
        return QList<IE_ModelLayer *>();
    return m_fieldOfViewList.at(m_activeFVIndex)->getLayers();
}

void IE_FieldOfView_Controller::checkLayerList()
{
    emit activeFVLayerListWasUpdated(getActiveFieldOfViewLayerList());
}

int IE_FieldOfView_Controller::getActiveFVIndex() const
{
    return m_activeFVIndex;
}

void IE_FieldOfView_Controller::addFieldOfView(int index)
{
    IE_FieldOfView * tmpFV = new IE_FieldOfView(index+1, layersList, m_p_ie_global_data);
    m_fieldOfViewList.append(tmpFV);
    connect(tmpFV, &IE_FieldOfView::addNewLayer, [this](IE_ModelLayer* pLayer)
    {
        emit addNewLayer(pLayer);
    });
    connect(tmpFV, &IE_FieldOfView::boundingRectWasCganged, [this]()
    {
        emit boundingRectWasChanged(getBoundingRectOfAllFieldOfView());
    });
    connect(tmpFV, &IE_FieldOfView::layerAction, [this](IE_ModelLayer::Action action, QList<IE_ModelLayer*>::iterator iter)
    {
        emit layerAction(action, iter);
    });

    m_quantityOfFields++;
}

void IE_FieldOfView_Controller::removeFieldOfView(int index)
{
    if(index < 0 || index >= m_quantityOfFields || index >= m_fieldOfViewList.size())
        return;
    m_fieldOfViewList.at(index)->removeLayersAndMainImage();
    delete m_fieldOfViewList.at(index);
    m_fieldOfViewList.removeAt(index);
    m_quantityOfFields--;

    if(m_quantityOfFields)
    {
        if(m_activeFVIndex == index)
            m_activeFVIndex = index-1;
        changeActiveFieldOfView(m_activeFVIndex);
        m_pInfoWidget->changeQuantity((Quantity)m_quantityOfFields);
    }
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


IE_FieldOfView_ControllerInfoWidget::IE_FieldOfView_ControllerInfoWidget(QWidget *parent):QWidget(parent)
{
    //pcboQuiantityFV = new QComboBox(this);
    pcboActiveFV = new QComboBox(this);
    pcboActiveFV->hide();
    m_pDockLayersListWidget = new QListWidget(this);
    m_pDockLayersListWidget->hide();
    m_pDockLayersListWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    QHBoxLayout * pHorBoxLayout = new QHBoxLayout();
    pInitBut = new QPushButton("Начать работу");
    connect(pInitBut, &QPushButton::clicked, this, &IE_FieldOfView_ControllerInfoWidget::makeInit);
    pHorBoxLayout->addWidget(pInitBut);
    setLayout(pHorBoxLayout);
}

IE_FieldOfView_ControllerInfoWidget::~IE_FieldOfView_ControllerInfoWidget()
{
    //delete pcboQuiantityFV;
    delete pcboActiveFV;
}

void IE_FieldOfView_ControllerInfoWidget::init(int currentFVquantity)
{
    delete pInitBut;
    pInitBut = nullptr;
    delete layout();
    QVBoxLayout * pVertBoxLayout = new QVBoxLayout(this);

    QHBoxLayout * pHorBoxLayout;
    QLabel *pLabel;
    /*
    {
        pHorBoxLayout = new QHBoxLayout(this);
        pLabel = new QLabel("Количество полей зрения:", this);


        pLabel->setBuddy(pcboQuiantityFV);
        QStringList  lst;

        lst << QString().number(IE_FieldOfView_Controller::Quantity::One)
            << QString().number(IE_FieldOfView_Controller::Quantity::Two)
            << QString().number(IE_FieldOfView_Controller::Quantity::Three)
            << QString().number(IE_FieldOfView_Controller::Quantity::Four)
            << QString().number(IE_FieldOfView_Controller::Quantity::Six);
        pcboQuiantityFV->addItems(lst);
        pcboQuiantityFV->setCurrentText(QString().number(currentFVquantity));
        oldQuantity = pcboQuiantityFV->currentText().toInt();

        pHorBoxLayout->addWidget(pLabel);
        pHorBoxLayout->addWidget(pcboQuiantityFV);
        pVertBoxLayout->addItem(pHorBoxLayout);

        connect(pcboQuiantityFV, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index)
        {
            if(index == -1)
                return;
//            if(oldQuantity<index+1)

            int n = QMessageBox::warning(nullptr,
                                         "Предупреждение",
                                         "Вы точно хотите изменить количество полей зрения?"
                                         "\nЕсли вы выбрали меньшее количество, то это повлечет "
                                         "удаление полей зрения (и данных/элементов на них), которые меньше выбранного значения.",
                                         QMessageBox::Yes | QMessageBox::No,
                                         QMessageBox::No
                                         );
            if(n == QMessageBox::Yes)
            {
                emit quantityWasChanged((IE_FieldOfView_Controller::Quantity)pcboQuiantityFV->currentText().toInt());
                oldQuantity = pcboQuiantityFV->currentText().toInt();
            }
            else
                pcboQuiantityFV->setCurrentText(QString().number(oldQuantity));

        });
    }*/


    {
        pHorBoxLayout = new QHBoxLayout(this);

        pLabel = new QLabel("Активное поле зрения:", this);
        pcboActiveFV->show();
        pLabel->setBuddy(pcboActiveFV);
        QStringList  lst;
        for(int i=0; i < currentFVquantity; i++)
            lst << QString().number(i+1);
        pcboActiveFV->addItems(lst);
        pcboActiveFV->setCurrentIndex(0);

        pHorBoxLayout->addWidget(pLabel);
        pHorBoxLayout->addWidget(pcboActiveFV);
        pVertBoxLayout->addItem(pHorBoxLayout);

        connect(pcboActiveFV, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index)
        {
            if(index == -1)
                return;
            emit activeFVWasChanged(pcboActiveFV->currentIndex());

        });

        QPushButton * ppbut;
        {

            ppbut = new QPushButton("Скрыть все элементы в поле зрения", this);
            pVertBoxLayout->addWidget(ppbut);
            connect(ppbut, &QPushButton::clicked, [this]()
            {
                emit hideAllElementsOnActiveFV();
            });
            ppbut = new QPushButton("Отобразить все элементы в поле зрения", this);
            pVertBoxLayout->addWidget(ppbut);
            connect(ppbut, &QPushButton::clicked, [this]()
            {
                emit showAllElementsOnActiveFV();
            });
        }
        {
            ppbut = new QPushButton("Удалить все элементы в поле зрения", this);
            pVertBoxLayout->addWidget(ppbut);
            connect(ppbut, &QPushButton::clicked, [this]()
            {
                int n = QMessageBox::warning(nullptr,
                                             "Предупреждение",
                                             "Вы точно хотите удалить все элементы на данном поле зрения?",
                                             QMessageBox::Yes | QMessageBox::No,
                                             QMessageBox::No
                                             );
                if(n == QMessageBox::Yes)
                    emit removeAllElementsOnActiveFV();
            });
        }
        {
            ppbut = new QPushButton("Примечания", this);
            pVertBoxLayout->addWidget(ppbut);
            connect(ppbut, &QPushButton::clicked, [this]()
            {
                makeChangeActiveFVNoteDialog();
            });
        }
        pVertBoxLayout->addStretch(1);
        {
            ppbut = new QPushButton("Изменить изображение", this);
            pVertBoxLayout->addWidget(ppbut);
            connect(ppbut, &QPushButton::clicked, [this]()
            {
                int n = QMessageBox::warning(nullptr,
                                             "Предупреждение",
                                             "Вы уверены, что хотите изменить изобаржение данного поля зрения?"
                                             "\nЭто повлечет удаление всех инструментов на данном поле зрения.",
                                             QMessageBox::Yes | QMessageBox::No,
                                             QMessageBox::No
                                             );
                if(n == QMessageBox::Yes)
                    emit changeActiveFVMainImage();

            });
        }
    }
    pVertBoxLayout->addStretch(1);

    pVertBoxLayout->addWidget(m_pDockLayersListWidget);
    m_pDockLayersListWidget->show();

    this->setLayout(pVertBoxLayout);
}

void IE_FieldOfView_ControllerInfoWidget::changeQuantity(int q)
{
    pcboQuiantityFV->setCurrentText(QString().number(q));
    QStringList  lst;
    pcboActiveFV->clear();
    for(int i=0; i < q; i++)
        lst << QString().number(i+1);
    pcboActiveFV->addItems(lst);
    pcboActiveFV->setCurrentIndex(0);
    emit activeFVWasChanged(0);
}

void IE_FieldOfView_ControllerInfoWidget::changeActiveFV(int index, QString fvNote)
{
    pcboActiveFV->setCurrentIndex(index);
    m_activeFVNote = fvNote;
}

void IE_FieldOfView_ControllerInfoWidget::updateActiveFVLayerList(QList<IE_ModelLayer *> layerList)
{
    m_pDockLayersListWidget->clear();

    for (QList<IE_ModelLayer*>::iterator iter = layerList.begin();
         iter!=layerList.end();iter++
         )
        m_pDockLayersListWidget->addItem( IE_ModelLayer::toStr( iter.i->t()->getToolType() ) );

}

void IE_FieldOfView_ControllerInfoWidget::makeChangeActiveFVNoteDialog()
{
    QDialog * locPDialog = new QDialog();
    QVBoxLayout * pMainLayout = new QVBoxLayout(locPDialog);

    QLabel *pLabel = new QLabel("Примечания:",locPDialog);
    QPlainTextEdit * pPlaintTextEdit = new QPlainTextEdit(m_activeFVNote, locPDialog);
    pLabel->setBuddy(pPlaintTextEdit);

    pMainLayout->addWidget(pLabel);
    pMainLayout->addWidget(pPlaintTextEdit);


    QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Save
                                         | QDialogButtonBox::Cancel,locPDialog);
    pMainLayout->addWidget(buttonBox);
    connect(buttonBox, &QDialogButtonBox::accepted, locPDialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, locPDialog, &QDialog::reject);

    locPDialog->setLayout(pMainLayout);
    locPDialog->setModal(true);

    locPDialog->show();
    int answer =  locPDialog->exec();
    if(answer == QDialog::Accepted)
    {
        m_activeFVNote = pPlaintTextEdit->toPlainText();
        emit activeFVNoteWasChanged(m_activeFVNote);
    }
    delete locPDialog;

}
