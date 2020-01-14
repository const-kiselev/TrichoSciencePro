#-------------------------------------------------
#
# Project created by QtCreator 2019-09-20T02:10:43
#
#-------------------------------------------------

QT       += core gui
QT += widgets
QT += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TrichosciencePro
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        TrichoSciencePro.cpp \
    image-editor/ie_modelLayer_controller.cpp \
        main.cpp \
    patient-manager/PatientManager.cpp \
    patient-manager/pm_documentIndex.cpp \
    patient-manager/pm_medicalRecord.cpp \
    patient-manager/pm_patient.cpp \
    patient-manager/pm_patientIndex.cpp


HEADERS += \
        TrichoSciencePro.h \
        _header_tsp.h \
    image-editor/ie_modelLayer_controller.h \
    patient-manager/PatientManager.h \
    patient-manager/pm_documentIndex.h \
    patient-manager/pm_medicalRecord.h \
    patient-manager/pm_patient.h \
    patient-manager/pm_patientIndex.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image-editor/IE_res.qrc

ICON = image-editor/appLogo.icns

macx {
 LIBS +=	 -framework AppKit
}

IMAGE_EDITOR_FOLDER = image-editor/

include($$IMAGE_EDITOR_FOLDER/image-editor.pri)
