#include "pm_patientIndexCnt.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

PM_PatientIndexCnt::PM_PatientIndexCnt(QDir workDir, QObject *parent): QObject(parent), m_workDir(workDir)
{
    m_pStackedWidget = new QStackedWidget();

    m_pPatientListWidget = nullptr;
}

PM_PatientIndexCnt::~PM_PatientIndexCnt()
{
    delete m_pStackedWidget;
}

int PM_PatientIndexCnt::init()
{
    int answer = open();
    if(answer == ReturnCode::Error_needToCheckDirs)
    {
        checkDirs();
        answer = open();
    }
    if(answer)
        return answer;

    return 0;
}

void PM_PatientIndexCnt::checkDirs()
{
    QStringList folderList = m_workDir.entryList(QDir::Dirs);
    foreach(QString subdir, folderList)
    {
        if(subdir == "." || subdir == "..")
            continue;
        TSP_PatientData patientData = PM_Patient::getTSP_PatientData(subdir);
        if(patientData.modelPath != "Empty")
            m_patientIndexList.append( PM_PatientIndex(patientData) );
    }
    save();
}

int PM_PatientIndexCnt::read(const QJsonObject &json)
{
    QJsonArray patientArray = json["patientArray"].toArray();
    for (int patientIndex = 0; patientIndex < patientArray.size(); ++patientIndex) {
            QJsonObject patientObject = patientArray[patientIndex].toObject();
            PM_PatientIndex pi;
            pi.read(patientObject);
            m_patientIndexList.append( pi );
        }
    emit PatinetIndexWasChanged();
    return 0;
}

//! Не стоит беспокоиться о том, что база будет большой. Ее размеры не могут привышать более 5 тыс. пациентов. Так как это максимум, то и данные можно каждый раз перезаписывать.
int PM_PatientIndexCnt::write(QJsonObject &json) const
{
    QJsonArray patientArray;
    foreach(PM_PatientIndex patientIndex, m_patientIndexList)
    {
        QJsonObject patientObj;
        patientIndex.write(patientObj);
        patientArray.append(patientObj);
    }
    json["patientArray"] = patientArray;

    return 0;
}

QStringList PM_PatientIndexCnt::getPatinetTitleList()
{

}

int PM_PatientIndexCnt::save()
{
    QFile patientIndexFile;
    patientIndexFile.setFileName( m_workDir.filePath("patientIndex.json") );
    if(!patientIndexFile.open(QIODevice::WriteOnly))
    {
        qWarning("Couldn't open file.");
        return ReturnCode::Ok;
    }
    QJsonObject patientIndexObj;
    patientIndexObj["TSP_docType"] = "TSP_JSON_patientIndex";
    write(patientIndexObj);
    QJsonDocument saveDoc(patientIndexObj);
    patientIndexFile.write(saveDoc.toJson());
    patientIndexFile.close();
    return ReturnCode::Ok;
}

int PM_PatientIndexCnt::open()
{
    QFile patientIndexFile;
    patientIndexFile.setFileName( m_workDir.filePath("patientIndex.json") );

    if(!patientIndexFile.exists())
        return ReturnCode::Error_needToCheckDirs;
    if(!patientIndexFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(nullptr, "Application", QString("Ошибка. Не удалось открыть файл %1 с пациентами.")
                                                                .arg(patientIndexFile.fileName()));
        qWarning("Couldn't open file.");
        return ReturnCode::Error_needToCheckDirs;
    }

    QByteArray saveData = patientIndexFile.readAll();
    patientIndexFile.close();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));


    QJsonObject jsonDoc = loadDoc.object();

    if(jsonDoc["TSP_docType"] != "TSP_JSON_patientIndex")
    {
        qDebug() << patientIndexFile.fileName() << "isn't TSP_JSON_patientIndex.";
        QMessageBox::warning(nullptr, "Application", QString("Ошибка. Не удалось прочитать файл %1 с пациентами.")
                                                                .arg(patientIndexFile.fileName()));
        patientIndexFile.rename( m_workDir.filePath("!!!patientIndex_ERROR.json") );
        return ReturnCode::Error_needToCheckDirs;
    }

     return read(jsonDoc);
}

QWidget *PM_PatientIndexCnt::getMainWidget()
{
    if(!m_pSeacrhWidget)
    {
        initSearchWidget();
        m_pStackedWidget->addWidget(m_pSeacrhWidget);
    }
    return m_pStackedWidget;
}

QList<PM_PatientIndex> PM_PatientIndexCnt::search(QString str)
{
    //! желательно переделать и делать поиску по каждому слову поисковой фразы
    //! Алгоритм поиска:
    //! - проверяется, является ли строка числом. Если является, то происходит поиск среди "id" and "uid".
    //! - далее, проверется существование подстроки в строке "fullName" and "nameAlias". Если в фразе есть
    //! пробел, то берется первое слово.

    open();

    if(str.isEmpty())
        return m_patientIndexList;

    QList<PM_PatientIndex> patientList;
    QString currentStr = str;

    bool ok;
    uint id = str.toUInt(&ok);
    if(ok)
    {
        foreach(PM_PatientIndex patientIndexElem,m_patientIndexList)
            if(patientIndexElem.id == id || patientIndexElem.uid == id)
                patientList.append(patientIndexElem);
    }

    bool answer = true;
    while(answer)
    {
        foreach(PM_PatientIndex patientIndexElem,m_patientIndexList)
        {
            if( patientIndexElem.fullName.contains(str, Qt::CaseInsensitive) || patientIndexElem.alias.contains(str, Qt::CaseInsensitive) )
                patientList.append(patientIndexElem);
        }
        if( str.contains(" ") )
            str = str.split(" ").at(0);
        else answer = false;
    }
    return patientList;
}

QWidget *PM_PatientIndexCnt::initSearchWidget()
{
    QWidget * pWidget = new QWidget();
    QVBoxLayout * pvbLayout = new QVBoxLayout(pWidget);
    QLineEdit * pSearchLineEdit = new QLineEdit(pWidget);
    pvbLayout->addWidget(pSearchLineEdit);
    QPushButton * pButton = new QPushButton("Поиск", pWidget);


    connect(pButton, &QPushButton::clicked, [this, pSearchLineEdit, pvbLayout]()
    {
        QStringList patientListForWidget;
        QList<PM_PatientIndex> listResult = search(pSearchLineEdit->text());
        QString tmp;
        foreach(PM_PatientIndex elem, listResult)
            patientListForWidget.append( QString("%1\tid: %2\tuid: %3").arg(elem.alias).arg(elem.id).arg(elem.uid) );

        if(!m_pPatientListWidget)
        {
            m_pPatientListWidget = new QListWidget();
            pvbLayout->addWidget(m_pPatientListWidget);

            connect(m_pPatientListWidget, &QListWidget::currentTextChanged, [this](QString text)
            {
                int uidPos = text.indexOf("uid: ");
                uint uid = text.mid(uidPos+5).toUInt();
                emit openPatientWitUID(uid);
            });
        }
        m_pPatientListWidget->clear();
        m_pPatientListWidget->addItems(patientListForWidget);
    });
}
