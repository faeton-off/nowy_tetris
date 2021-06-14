#include "okno.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv); // uruchomienie aplikacji

    Okno okno; // wywołanie okna
    okno.show(); // wyświetlenie okna

    return app.exec();
}
