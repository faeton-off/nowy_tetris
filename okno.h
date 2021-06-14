#ifndef OKNO_H

#define OKNO_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLCDNumber;
class QLabel;
class QPushButton;
QT_END_NAMESPACE

class Plansza;

class Okno : public QWidget
{
    Q_OBJECT

    public:
        /*!
         * \brief
         * Tworzy interfejs programu.
         */
        Okno(QWidget *parent = nullptr);

    private:
        QLabel *stworzOkno(const QString &text); // tworzy nowe okno
        Plansza *plansza; // wyświetla planszę
        QLabel *kolejnyKlocek; // kolejny klocek
        QLCDNumber *wyniklcd; // aktualny wynik
        QLCDNumber *poziomlcd; // aktualny poziom
        QLCDNumber *linijkilcd; // wypełnione linijki
        QPushButton *przyciskStart; // przycisk "Start"
        QPushButton *przyciskWyjdz; // przycisk "Wyjdź"
        QPushButton *przyciskPauza; // przycisk "Pauza/Wznów"
};

#endif
