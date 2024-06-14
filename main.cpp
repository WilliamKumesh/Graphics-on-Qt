#include <QApplication>
#include <QWidget>
#include "scene_widget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto w = new Scene_widget;
    w->show();

    return a.exec();
}
