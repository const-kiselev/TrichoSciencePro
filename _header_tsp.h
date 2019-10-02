#ifndef HEADER_TSP_H
#define HEADER_TSP_H

#include <QString>

struct _Model_patientData
{
    QString patient_fullName, doctor_fullName, modelDir, modelPath;
    uint patient_ID, patient_UID, doctor_ID, model_ID;
//    int editorVersion;
    _Model_patientData()
    {
        patient_fullName = doctor_fullName = modelPath = "Empty";
        patient_ID = patient_UID = doctor_ID = model_ID = 0;
        modelDir="";
    }
};

#endif // _HEADER_TSP_H
