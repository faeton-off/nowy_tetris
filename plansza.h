#ifndef PLANSZA_H
#define PLANSZA_H

#include <QBasicTimer>
#include <QFrame>
#include <QPointer>
#include "klocek.h"

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE//djddsfsd

class Plansza: public QFrame
{
    Q_OBJECT

    public:
        /*!
         * \brief
         * Tworzy planszę do gry.
         */
        Plansza( QWidget *parent = nullptr );

        /*!
         * \brief
         * Nakazuje utworzenie kolejnego klocka.
         */
        void ustNastepnyKlocek( QLabel *label );

        /*!
         * \brief
         * Przechowuje minimalny rozmiar planszy.
         */
        QSize sizeHint() const override;

        /*!
         * \brief
         * Przechowuje minimalny (zalecany) rozmiar planszy.
         */
        QSize minimumSizeHint() const override;

        // funkcje sizeHint() i minimumSizeHint() to gotowe funkcje Qt

    public slots:
        /*!
         * \brief
         * Umożliwia rozpoczęcie rozgrywki.
         */
        void startGry();

        /*!
         * \brief
         * Umożliwia zapauzowanie rozgrywki.
         */
        void pauzaGry();


    signals:
        /*!
         * \brief
         * Przesyła informację o zmianie wyniku.
         */
        void zmianaWyniku( int wynik );

        /*!
         * \brief
         * Przesyła informację o zmianie poziomu.
         */
        void zmianaPoziomu( int poziom );

        /*!
         * \brief
         * Przesyła informację o zmianie usuniętych linijek.
         */
        void zmianaUsunietychLinijek( int liczbaLinijek );


    protected:
        /*!
         * \brief
         * Umożliwia wizualną ciągłą zmianę i przesuwanie się planszy.
         */
        void paintEvent( QPaintEvent *event ) override;

        /*!
         * \brief
         * Umożliwia sterowanie klockiem na planszy oraz używanie przycisków.
         */
        void keyPressEvent( QKeyEvent *event ) override;

        /*!
         * \brief
         * Rozpoznaje aktualny status rozgrywki (start/pauza), kontroluje prędkość rozgrywki.
         */
        void timerEvent( QTimerEvent *event ) override;


    private:
        enum
        {
            szerokoscPlanszy = 10 , wysokoscPlanszy = 22
        };

        KsztaltKlocka &ksztaltO( int x , int y )
        {
            return tablica[(y * szerokoscPlanszy) + x];
        }

        /*!
         * \brief
         * czas rozgrywki (do wyniku)
         */
        int timeoutTime()
        {
            return 1000 / (1 + poziom);
        }

        /*!
         * \brief
         * szerokość do utworzenia kolejnego klocka
         */
        int szerokosc2()
        {
            return contentsRect().width() / szerokoscPlanszy;
        }

        /*!
         * \brief
         * wysokość do utworzenia kolejnego klocka
         */
        int wysokosc2()
        {
            return contentsRect().height() / wysokoscPlanszy;
        }

        void wyczyscPlansze(); // czyszczenie planszy
        void upusc(); // przesuwanie klocka w dół
        void linijkaDoDolu(); // przesunięcie klocka o linijkę w dół
        void upuszczonyKlocek( int wysokosc ); // naliczanie punktów
        void usunPelneLinijki(); // skanowanie planszy w celu odnalezienia pełnej linijki
        void nowyKlocek(); // wywołanie następnego klocka
        void pokazNastepny(); // aktualizacja planszy pokazując następny klocek na niej
        bool sprobujRuszacKlockiem( const Klocek &nowyKlocek , int xX , int yY ); // sprawdzenie, czy można ruszyć klockiem w zadany przez użytkownika sposób
        void rysujKlocek( QPainter &painter , int x , int y , KsztaltKlocka ksztalt ); // wizualne utworzenie i pokolorowanie klocka

        QBasicTimer timer; // gotowa funkcja Qt do analizowania statusu rozgrywki
        QPointer<QLabel> nastepnyKlocek; // ustawienie następnego klocka

        // zmienne do oceny aktualnego statusu rozgrywki
        bool czyTrwa;
        bool czyZapauzowana;
        bool czyCzeka;

        Klocek obecnyKlocek; // zapis aktualnego klocka
        Klocek nastKlocek; // zapis następnego klocka

        int obecnyX; // zmienna przechowująca położenie klocka w poziomie
        int obecnyY; // zmienna przechowują położenie klocka w pionie
        int liczbaUsunietych; // liczba usuniętych linijek (do wyniku)
        int liczbaKlockow; // liczba użytych klocków (do wyniku)
        int wynik; // zmienna przechowująca aktualny wynik
        int poziom; // zmienna przechowująca aktualny poziom

        KsztaltKlocka tablica[ szerokoscPlanszy * wysokoscPlanszy ]; // tablica przechowująca "zapis" planszy przed wyczyszczeniem linijki
};

#endif
