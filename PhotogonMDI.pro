#-------------------------------------------------
#
# Project created by QtCreator 2018-12-30T12:24:44
#
#-------------------------------------------------

QT       += core gui

QT += network

QT += svg



mac {
    CONFIG += x68
    CONFIG += link_pkgconfig
    CONFIG += app_bundle
    PKGCONFIG += opencv
    PKGCONFIG += zbar
    QT_CONFIG -= no-pkg-config
    LIBS += -L/usr/local/Cellar/opencv/3.4.3/lib/
    INCLUDEPATH += /usr/local/include/eigen3/

    plist.path = "$$DESTDIR/$$join(TARGET,,,.app)/Contents"
    plist.files = Info.plist
    INSTALLS += plist
}


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PhotogonMDI
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS


CONFIG += c++1z

3QMAKE_CXXFLAGS += -std=c++17

CONFIG += sdk_no_version_check



SOURCES += \
        main.cpp \
    mainwindow.cpp \
    newproject.cpp \
    bitbuffer.cpp \
    qrsegment.cpp \
    qrcode.cpp \
    qrgenerate.cpp \
    listgenerate.cpp \
    treatmentdialog.cpp \
    treatmentnumdialog.cpp \
    treatmentlevelsdialog.cpp \
    ../Documents/programming/photogon/photogon/Photogon/configdialog.cpp \
    cameradiscovery.cpp \
    myudp.cpp \
    camerasetup.cpp \
    captureplant.cpp \
    worker.cpp \
    displayfront.cpp \
    displayside.cpp \
    displaytop.cpp \
    editmask.cpp \
    computervision.cpp \
    displayshape.cpp \
    workershapes.cpp \
    listcaptureplant.cpp \
    displayhistogram.cpp \
    displaymask.cpp \
    welcomescreen.cpp \
    editroi.cpp \
    selectcolorchips.cpp \
    detectcolorchips.cpp \
    createtargetcolor.cpp \
    colorchipsmask.cpp \
    connectcameras.cpp \
    workerdisplayfront.cpp \
    manualimport.cpp \
    manualimportselected.cpp \
    manualimportconfig.cpp \
    manualimportmasksubtraction.cpp \
    manualroi.cpp \
    manualimportmask.cpp \
    manualcomputervisionsubtraction.cpp \
    manualcomputervision.cpp \
    mytcp.cpp \
    mytcpside.cpp \
    mytcptop.cpp

HEADERS += \
    mainwindow.h \
    newproject.h \
    bitbuffer.hpp \
    qrsegment.hpp \
    qrcode.hpp \
    qrgenerate.h \
    listgenerate.h \
    treatmentdialog.h \
    treatmentnumdialog.h \
    treatmentlevelsdialog.h \
    ../Documents/programming/photogon/photogon/Photogon/configdialog.h \
    cameradiscovery.h \
    myudp.h \
    camerasetup.h \
    captureplant.h \
    worker.h \
    displayfront.h \
    displayside.h \
    displaytop.h \
    editmask.h \
    computervision.h \
    displayshape.h \
    workershapes.h \
    listcaptureplant.h \
    displayhistogram.h \
    displaymask.h \
    welcomescreen.h \
    editroi.h \
    selectcolorchips.h \
    detectcolorchips.h \
    createtargetcolor.h \
    colorchipsmask.h \
    connectcameras.h \
    workerdisplayfront.h \
    manualimport.h \
    manualimportselected.h \
    manualimportconfig.h \
    manualimportmasksubtraction.h \
    manualroi.h \
    manualimportmask.h \
    manualcomputervisionsubtraction.h \
    manualcomputervision.h \
    mytcp.h \
    mytcpside.h \
    mytcptop.h

FORMS += \
    mainwindow.ui \
    newproject.ui \
    qrgenerate.ui \
    listgenerate.ui \
    ../Documents/programming/photogon/photogon/Photogon/configdialog.ui \
    cameradiscovery.ui \
    camerasetup.ui \
    captureplant.ui \
    displayfront.ui \
    displayside.ui \
    displaytop.ui \
    editmask.ui \
    displayshape.ui \
    listcaptureplant.ui \
    displayhistogram.ui \
    displaymask.ui \
    welcomescreen.ui \
    editroi.ui \
    selectcolorchips.ui \
    connectcameras.ui \
    manualimport.ui \
    manualimportselected.ui \
    manualimportconfig.ui \
    manualimportmasksubtraction.ui \
    manualroi.ui \
    manualimportmask.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc
