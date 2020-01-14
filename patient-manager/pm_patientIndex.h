#ifndef PM_PATIENTINDEXCNT_H
#define PM_PATIENTINDEXCNT_H

#include <QObject>
#include <QJsonObject>
#include <QDir>
#include <QStackedWidget>
#include <QListWidget>
#include <QPushButton>

#include "pm_patient.h"

class PM_PatientIndex_SearchWidget;

class PM_PatientIndex
{
public:
    QString fullName, alias;
    uint id, uid;

    friend bool operator ==(const PM_PatientIndex& left, const PM_PatientIndex& right);

    PM_PatientIndex()
    {
        fullName = alias = "";
        id = uid = 0;
    }
    PM_PatientIndex(QString fullName, QString alias, uint id, uint uid)
    {
        this->fullName = fullName;
        this->alias = alias;
        this->id = id;
        this->uid = uid;
    }
    PM_PatientIndex(TSP_PatientData pd)
    {
        this->fullName = pd.patient_fullName;
        this->alias = pd.patient_nameAlias;
        this->id = pd.patient_ID;
        this->uid = pd.patient_UID;
    }
    int    read(const QJsonObject &json)
    {
        fullName = json["fullName"].toString();
        alias = json["alias"].toString();
        id = json["id"].toString().toUInt();
        uid = json["uid"].toString().toUInt();
        return 0;
    }
    int    write(QJsonObject &json)const
    {
        json["fullName"] = fullName;
        json["alias"] = alias;
        json["id"] = QString().number(id);
        json["uid"] = QString().number(uid);
        return 0;
    }
};

class PM_PatientIndexCnt : public QObject
{
    Q_OBJECT
public:
    enum ReturnCode{ Ok = 0, Error, Error_needToCheckDirs };


    explicit PM_PatientIndexCnt(QDir workDir = QDir("data/patients/"),
                                QObject *parent = nullptr);
    ~ PM_PatientIndexCnt();

    int init();
//    void addPatient();
//    void addPatientDialog();

//    void deletePatient();

    int    read(const QJsonObject &json);
    int    write(QJsonObject &json)const;

    QList<QMenu*> getAvailableActions();

    int save();
    int open();

    QWidget * getMainWidget();

signals:
    void patinetIndexWasChanged();
    void needToRunImageEditor(TSP_PatientData patientData, IE_ProfileType ie_type);

    void availableActionsWasChanged();

public slots:
    QList<PM_PatientIndex> search(QString str);
    int addPatient_Dialog();
    void checkDirs(); //! функция проверки доступных директорий и формирование и сохранение нового индекса пациентов
    void openPatientWidget(uint patientUID);
    void updateDocumentData(TSP_PatientData patientData);
    void makeSearch();

private:
    QList<PM_PatientIndex> m_patientIndexList;
    QDir m_workDir;
    PM_PatientIndex_SearchWidget * m_pSeacrhWidget;  //! виджет поиска

    QStackedWidget * m_pStackedWidget;
    PM_Patient     * m_pCurrentPatient;


    QWidget * initSearchWidget();
};


class PM_PatientIndex_SearchWidget: public QWidget
{
    Q_OBJECT
public:
    PM_PatientIndex_SearchWidget();
    friend class PM_PatientIndexCnt;
signals:
    void makeSearch();
    void choosenListElement(uint uid);
protected:
    bool event(QEvent *event) override;
private:
    QLineEdit * m_pSeacrhLineEdit;
    QListWidget        * m_pPatientListWidget; //! виджет со списком найденных пациентов
    QPushButton * m_pSearchButton;
    void printSearchResult(QList<PM_PatientIndex> &listResult);
    void sendActionToOpenPatientWidget();
};

#endif // PM_PATIENTINDEXCNT_H
