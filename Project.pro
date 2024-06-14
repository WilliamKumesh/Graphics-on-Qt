QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Camera.cpp \
    camera_controller.cpp \
    main.cpp \
    scene_widget.cpp

HEADERS += \
    Camera.h \
    ChessDesk.h \
    Figure.h \
    Step.h \
    camera_controller.h \
    scene_widget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Bishop.obj \
    C:\\Qt\\Examples\\Qt-5.15.16\\PROJECT\\fragment.glsl \
    C:\\Qt\\Examples\\Qt-5.15.16\\PROJECT\\vertex.glsl \
    Chess.mtl \
    Chess.obj \
    ChessBoard.mtl \
    ChessBoard.obj \
    King.mtl \
    King.obj \
    Knight.obj \
    Pawn.obj \
    Pawn.png \
    Queen.obj \
    Rook.obj \
    fragment.glsl \
    textureBlack.jpg \
    textureBlack.jpg \
    textureWhite.jpg \
    vertex.glsl


DISTFILES += \
    Bishop.obj \
    C:\\Qt\\Examples\\Qt-5.15.16\\PROJECT\\fragment.glsl \
    C:\\Qt\\Examples\\Qt-5.15.16\\PROJECT\\vertex.glsl \
    Chess.mtl \
    Chess.obj \
    ChessBoard.mtl \
    ChessBoard.obj \
    King.mtl \
    King.obj \
    Knight.obj \
    Pawn.obj \
    Pawn.png \
    Queen.obj \
    Rook.obj \
    fragment.glsl \
    textureBlack.jpg \
    textureWhite.jpg \
    vertex.glsl
