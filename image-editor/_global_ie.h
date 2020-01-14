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


class IE_Model;


//! \brief класс для хранения глобальных данных редактора изображений.
//!
//! В основном используется для хранения:
//! - масштаба изображения в экзмепляре модели изображения
//! - константы вычислений
//! - директории работы документа

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



public:

    explicit _global_ie(QObject *parent = nullptr);

  QString getModelDirPath() const;


  QString getModelResDirPath() const;


  QString getTmp_modelDirPath() const;


  QString getTmp_modelResDirPath() const;


  QString getLastSelectedDirByUser() const;
  void setLastSelectedDirByUser(const QString &value);

private:
  friend class IE_Model;

  qreal threshold_terminal_wellus,threshold_thinHair_top, threshold_mediumHair_top;

  qreal measureIndex;
  UnitType unitType, measureIndexUnitType;
  QString modelDirPath, modelResDirPath, tmp_modelDirPath, tmp_modelResDirPath, lastSelectedDirByUser;
private slots:
  void setMeasureIndex(qreal mIndex);
  void setUnitType(UnitType uType);
  void setIndexUnitType(int uType);
  void setThreshold_TW(qreal data);
  void setThreshold_thinHair(qreal data);
  void setThreshold_mediumHair(qreal data);
  void setModelDirPath(const QString &value);
  void setModelResDirPath(const QString &value);
  void setTmp_modelDirPath(const QString &value);
  void setTmp_modelResDirPath(const QString &value);
};


#endif // _GLOBAL_IE_H
