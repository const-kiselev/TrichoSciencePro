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
        image-editor/ImageEditor.cpp \
        image-editor/_global_ie.cpp \
        image-editor/ieModel.cpp \
        image-editor/ieView.cpp \
        image-editor/ie_computeModule.cpp \
        image-editor/ie_layersTableModel.cpp \
        image-editor/ie_line_dd.cpp \
        image-editor/ie_modelLayer.cpp \
        image-editor/ie_tool.cpp \
        image-editor/ie_toolCalibration.cpp \
        image-editor/ie_toolItemDelegate.cpp \
        image-editor/ie_toolLine.cpp \
        image-editor/ie_toolMarker.cpp \
        image-editor/ie_toolRuler.cpp \
        image-editor/ie_tool_follicularUnit.cpp \
        image-editor/ie_tool_image.cpp \
        image-editor/toolController.cpp \
        main.cpp \
        patient-manager/PatientManager.cpp \
        patient-manager/pm_patientRecord.cpp \
        patient-manager/pm_test_patientList.cpp

HEADERS += \
        TrichoSciencePro.h \
        _header_tsp.h \
        image-editor/ImageEditor.h \
        image-editor/_global_ie.h \
        image-editor/ieModel.h \
        image-editor/ieView.h \
        image-editor/ie_computeModule.h \
        image-editor/ie_header.h \
        image-editor/ie_layersTableModel.h \
        image-editor/ie_line_dd.h \
        image-editor/ie_modelLayer.h \
        image-editor/ie_tool.h \
        image-editor/ie_toolCalibration.h \
        image-editor/ie_toolItemDelegate.h \
        image-editor/ie_toolLine.h \
        image-editor/ie_toolRuler.h \
        image-editor/ie_tool_follicularUnit.h \
        image-editor/ie_tool_image.h \
        image-editor/ie_toolmarker.h \
        image-editor/toolController.h \
        patient-manager/PatientManager.h \
        patient-manager/pm_patientRecord.h \
        patient-manager/pm_test_patientList.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image-editor/IE_res.qrc
