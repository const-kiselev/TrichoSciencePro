#ifndef _GLOBAL_IE_H
#define _GLOBAL_IE_H

#include <QObject>
enum UnitType {m = 0,cm,mm,um,px};

static const char* UnitTypeTitle[] = {"м", "см", "мм", "мкм", "px"};

static const char   * IE_MODEL_DIR = "./IE_models",
                    * IE_MODEL_DIR_NAME = "IE_models",
                    * IE_MODEL_RES_DIR = "./res",
                    * IE_MODEL_RES_DIR_NAME = "res",
                    * IE_MODEL_FILE_NAME_POSTFIX = "_IE_model",
                    * IE_MODEL_FILE_NAME_EXTENSION = "json",
                    * IE_TMP_DIR = "./tmp", * IE_TMP_DIR_NAME = "tmp";



class _global_ie:public QObject
{
    Q_OBJECT
private:

signals:
  void measureIndexChanged(qreal mIndex);
  void unitTypeChanged(UnitType uType);
  void changed();

public slots:
  qreal getMeasureIndex() const;
  UnitType getUnitType() const;
  qreal getThreshold_TW() const;
  qreal getThreshold_thinHair() const;
  qreal getThreshold_mediumHair() const;
  qreal convertF(qreal value) const;
  qreal convertPixelWithForamtF(qreal value, UnitType uType) const;
  qreal convertUnitedWithForamtF(qreal value, UnitType uType) const;


  void setMeasureIndex(qreal mIndex, int key);
  void setUnitType(UnitType uType, int key);
  void setIndexUnitType(int uType, int key);
  void setThreshold_TW(qreal data, int key);
  void setThreshold_thinHair(qreal data, int key);
  void setThreshold_mediumHair(qreal data, int key);
public:
//  void setOwner(int randomKey);

    explicit _global_ie(int randomKey, QObject *parent = nullptr);

  QString getModelDirPath() const;
  void setModelDirPath(const QString &value, int key);

  QString getModelResDirPath() const;
  void setModelResDirPath(const QString &value, int key);

  QString getTmp_modelDirPath() const;
  void setTmp_modelDirPath(const QString &value, int key);

  QString getTmp_modelResDirPath() const;
  void setTmp_modelResDirPath(const QString &value, int key);

  QString getLastSelectedDirByUser() const;
  void setLastSelectedDirByUser(const QString &value);

private:
  qreal threshold_terminal_wellus,threshold_thinHair_top, threshold_mediumHair_top;
  bool ownerSet;
  int randomKey;
  qreal measureIndex;
  UnitType unitType, measureIndexUnitType;
  QString modelDirPath, modelResDirPath, tmp_modelDirPath, tmp_modelResDirPath, lastSelectedDirByUser;
};


#endif // _GLOBAL_IE_H
