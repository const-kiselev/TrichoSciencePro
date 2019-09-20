#ifndef _GLOBAL_IE_H
#define _GLOBAL_IE_H

#include <QObject>
enum UnitType {m = 0,cm,mm,um,px};

static const char* UnitTypeTitle[] = {"м", "см", "мм", "мкм", "px"};

template <class T>
class Singleton {
public:
  static T& instance() {
    static T instance;
    return instance;
  }

private:
  Singleton();
  ~Singleton();
  Singleton(const Singleton &);
  Singleton& operator=(const Singleton &);
};


class _global_ie:public QObject
{
    Q_OBJECT


private:
  explicit _global_ie(QObject *parent = 0);
signals:
  void measureIndexChanged(qreal mIndex);
  void unitTypeChanged(UnitType uType);
  void changed();

public slots:
  qreal getMeasureIndex() const;
  UnitType getUnitType() const;
  qreal getThreshold_TW() const;
  qreal convertF(qreal value) const;
  qreal convertWithForamtF(qreal value, UnitType uType) const;


  void setMeasureIndex(qreal mIndex, int key);
  void setUnitType(UnitType uType, int key);
  void setIndexUnitType(int uType, int key);
  void setThreshold_TW(qreal data, int key);
public:
  void setOwner(int randomKey);

  int getIndexThreshold_terminal_wellus_unitType() const;
  void setThreshold_terminal_wellus_unitType(int value, int key);

private:
  friend class Singleton<_global_ie>;
  qreal threshold_terminal_wellus;
  bool ownerSet;
  int randomKey;
  qreal measureIndex;
  UnitType unitType, threshold_terminal_wellus_unitType, measureIndexUnitType;

};

#define IE_GLOBAL_DATA Singleton<_global_ie>::instance()

#endif // _GLOBAL_IE_H
