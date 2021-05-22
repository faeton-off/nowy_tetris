
#include "tetrixwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TetrixWindow window;//я ещё хочу сдесь изменить что-то
    window.show();
    return app.exec();
}
