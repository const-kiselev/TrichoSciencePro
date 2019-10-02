QT       += core gui
QT += widgets
QT += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = image-editor
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

ICON = resources/TSP.icns


SOURCES += \
    ImageEditor.cpp \
    _global_ie.cpp \
    ieModel.cpp \
    ieView.cpp \
    ie_computeModule.cpp \
    ie_layersTableModel.cpp \
    ie_line_dd.cpp \
    ie_modelLayer.cpp \
    ie_tool.cpp \
    ie_toolCalibration.cpp \
    ie_toolItemDelegate.cpp \
    ie_toolLine.cpp \
    ie_toolMarker.cpp \
    ie_toolRuler.cpp \
    ie_tool_follicularUnit.cpp \
    ie_tool_image.cpp \
    main.cpp \
    toolController.cpp

HEADERS += \
    ../_header_tsp.h \
    ImageEditor.h \
    _global_ie.h \
    ieModel.h \
    ieView.h \
    ie_computeModule.h \
    ie_header.h \
    ie_layersTableModel.h \
    ie_line_dd.h \
    ie_modelLayer.h \
    ie_tool.h \
    ie_toolCalibration.h \
    ie_toolItemDelegate.h \
    ie_toolLine.h \
    ie_toolRuler.h \
    ie_tool_follicularUnit.h \
    ie_tool_image.h \
    ie_toolmarker.h \
    toolController.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    IE_res.qrc

ICON = TSP.icns
