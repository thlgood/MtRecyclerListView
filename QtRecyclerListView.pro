QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    listitem.cpp \
    main.cpp \
    dialog.cpp \
    src/mtrecyclerlistview.cpp \
    src/mtrecyclermulticolumnlistview.cpp \
    src/mtrecyclermulticolumnlistviewadapter.cpp \
    src/multicolumnlistheader.cpp

HEADERS += \
    dialog.h \
    listitem.h \
    src/mtrecyclerlistview.h \
    src/mtrecyclerlistviewadapter.h \
    src/mtrecyclermulticolumnlistview.h \
    src/mtrecyclermulticolumnlistviewadapter.h \
    src/multicolumnlistheader.h

FORMS += \
    dialog.ui \
    listitem.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource/resource.qrc
