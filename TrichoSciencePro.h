#ifndef TRICHOSCIENCEPRO_H
#define TRICHOSCIENCEPRO_H

#include <QObject>
#include "_header_tsp.h"
#include "patient-manager/PatientManager.h"
#include "image-editor/ImageEditor.h"


class TrichoSciencePro : public QObject
{
    Q_OBJECT
public:
    explicit TrichoSciencePro(QObject *parent = nullptr);
    void start();
private:
    PatientManager *pPatientManager;
signals:

private slots:
    void runNewImageEditor(_Model_patientData patientData);
    void runSavedImageEditor(_Model_patientData patientData);
};

#endif // TRICHOSCIENCEPRO_H
