#ifndef PM_DOCUMENTINDEX_H
#define PM_DOCUMENTINDEX_H

#include <QDate>
#include <QDir>
#include <QObject>
#include <QStandardItemModel>
#include <QTableWidget>
#include "../_header_tsp.h"

class PM_DocumentIndexElement
{
public:
    explicit PM_DocumentIndexElement();
    enum Type {ie_model, medicalExamRecord};
    static QString convertType(Type type);
    static Type convertType(QString stringType);
    struct IE_ModelInfoStruct
    {
        IE_ProfileType ie_type;
    };
    IE_ModelInfoStruct getIE_ModelInfo();
    void setIE_ModelInfo(IE_ModelInfoStruct & iemis);

    int     read(const QJsonObject &json);
    int     write(QJsonObject &json)const;
    int     openIE_Model(QDir dir);
    friend bool operator ==(const PM_DocumentIndexElement& left,
                            const PM_DocumentIndexElement& right
                            );
    Type getType() const;
    void setType(const Type &type);

    QString getAlias() const;
    void setAlias(const QString &alias);

    QString getPath() const;
    void setPath(const QString &path);

    QDateTime getCreateDateTime() const;
    void setCreateDateTime(const QDateTime &createDateTime);

    QDateTime getSaveDateTime() const;
    void setSaveDateTime(const QDateTime &saveDateTime);

    IE_ModelInfoStruct getIeModelInfo() const;
    void setIeModelInfo(const IE_ModelInfoStruct &ieModelInfo);

private:
    Type m_type;
    QString m_alias;
    QString m_path;
    QDateTime m_createDateTime, m_saveDateTime;
    IE_ModelInfoStruct m_ieModelInfo;
};


//! \brief Класс, отвечающий за работу с индексом документом. Именно он отправляет запрос на открытие документа.
class PM_DocumentIndexCnt : public QObject
{
    Q_OBJECT
public:
    explicit PM_DocumentIndexCnt(QDir workDir, QObject *parent = nullptr);

    enum FilterType {ie_model_Trichogram, ie_model_Trichoscopy, ie_model_Phototrichogram, medicalExamRecord};

    int init();
    void findAllDocuments();

    int save();
    int open();
    void addDocument(PM_DocumentIndexElement & iemis);
    void deleteDocument(PM_DocumentIndexElement & iemis);

    int     read(const QJsonObject &json);
    int     write(QJsonObject &json)const;


    QWidget *getWidget();

signals:
    void documentIndexWasChanged();
    void openImageEditor(QString path);
public slots:
    void updateTable();
    void changeFilter( FilterType ft );
private:
    QStandardItemModel* m_documentIndexModel;
    QList<PM_DocumentIndexElement> m_documentList;
    QDir m_workDir;
    QTableView * m_tableView;
    QWidget * m_widget;

    void initWidget();

};


#endif // PM_DOCUMENTINDEX_H
