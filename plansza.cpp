#include "plansza.h"
#include <QKeyEvent>
#include <QLabel>
#include <QPainter>

Plansza::Plansza( QWidget *parent ) : QFrame( parent ) , czyTrwa( false ) , czyZapauzowana( false )
{
    /*!
     * \brief setFrameStyle( )
     * Ustawienie stylu planszy wg gotowych stylów Qt (Panel umożliwia użycie Sunken - wypukłość pojedynczych "kwadracików" klocka
     */
    setFrameStyle( QFrame::Panel | QFrame::Sunken );

    /*!
     * \brief setFocusPolicy( )
     * Zwracanie uwagi na działania użytkownika - StrongFocus oznacza, że program będzie odczytywał zarówno kliknięcia myszką, jak i odczyty z klawiatury.
     */
    setFocusPolicy( Qt::StrongFocus );

    /*!
     * \brief wyczyscPlansze( )
     * Czyszczenie planszy.
     */
    wyczyscPlansze();

    nastKlocek.losowyKsztalt();
}

void Plansza::ustNastepnyKlocek( QLabel *label )
{
    nastepnyKlocek = label;
}

/*!
 * \brief sizeHint( )
 * Gotowa funkcja Qt przechowująca minimalny rozmiar planszy.
 */
QSize Plansza::sizeHint() const
{
    return QSize( szerokoscPlanszy * 15 + frameWidth() * 2 , wysokoscPlanszy * 15 + frameWidth() * 2 );
}

/*!
 * \brief minimumSizeHint( )
 * Gotowa funkcja Qt przechowująca minimalny (zalecany) rozmiar planszy.
 */
QSize Plansza::minimumSizeHint() const

{
    return QSize( szerokoscPlanszy * 5 + frameWidth() * 2 , wysokoscPlanszy * 5 + frameWidth() * 2 );
}

void Plansza::startGry() // Umożliwia rozpoczęcie rozgrywki).
{
    if ( czyZapauzowana )
    {
        return;
    }

    czyTrwa = true; // sprawdzenie, czy rozgrywka jest w toku
    czyCzeka = false; // sprawdczenie, czy program oczekuje działania użytkownika
    liczbaUsunietych = 0;
    liczbaKlockow = 0;
    wynik = 0;
    poziom = 1;

    /*!
     * \brief wyczyscPlansze( )
     * Czyszczenie planszy.
     */
    wyczyscPlansze();

    emit zmianaUsunietychLinijek( liczbaUsunietych );
    emit zmianaWyniku( wynik );
    emit zmianaPoziomu( poziom );

    /*!
     * \brief nowyKlocek( )
     * Wywołanie następnego klocka.
     */
    nowyKlocek();

    timer.start( timeoutTime() , this );
}

void Plansza::pauzaGry() // Umożliwia zapauzowanie rozgrywki.
{
    if ( !czyTrwa )
    {
        return;
    }

    czyZapauzowana = !czyZapauzowana;

    if ( czyZapauzowana )
    {
        timer.stop();
    }
    else
    {
        timer.start( timeoutTime() , this );
    }

    /*!
     * \brief update( )
     * Przekazanie informacji o stanie rozgrywki.
     */
    update();
}

void Plansza::paintEvent( QPaintEvent *event ) // Umożliwia wizualną ciągłą zmianę i przesuwanie się planszy.
{
    QFrame::paintEvent( event );
    QPainter painter(this);
    QRect rect = contentsRect();

    if ( czyZapauzowana )
    {
        painter.drawText( rect , Qt::AlignCenter , tr( "PAUZA" ) );
        return;
    }

    int boardTop = rect.bottom() - wysokoscPlanszy * wysokosc2();

    for  ( int i = 0 ; i < wysokoscPlanszy ; ++i )
    {
        for ( int j = 0 ; j < szerokoscPlanszy ; ++j )
        {
            KsztaltKlocka ksztalt = ksztaltO( j , wysokoscPlanszy - i - 1 );

            if ( ksztalt != NoShape )
            {
                rysujKlocek( painter , rect.left() + j * szerokosc2() , boardTop + i * wysokosc2() , ksztalt );
            }
        }

    }

    if ( obecnyKlocek.ksztalt() != NoShape )
    {
        for (int i = 0; i < 4; ++i)
        {
            int x = obecnyX + obecnyKlocek.x( i );
            int y = obecnyY - obecnyKlocek.y( i );
            rysujKlocek( painter , rect.left() + x * szerokosc2() , boardTop + ( wysokoscPlanszy - y - 1 ) * wysokosc2() , obecnyKlocek.ksztalt() );
        }
    }
}

void Plansza::keyPressEvent( QKeyEvent *event ) // Umożliwia sterowanie klockiem na planszy oraz używanie przycisków.
{
    if ( !czyTrwa || czyZapauzowana || obecnyKlocek.ksztalt() == NoShape )
    {
        QFrame::keyPressEvent(event);
        return;
    }

    switch ( event->key() )
    {
        case Qt::Key_Left:
            /*!
             * \brief sprobujRuszacKlockiem( )
             * Sprawdzenie, czy można ruszyć klockiem w zadany przez użytkownika sposób.
             */
            sprobujRuszacKlockiem( obecnyKlocek , obecnyX - 1 , obecnyY) ;
            break;
        case Qt::Key_Right:
            sprobujRuszacKlockiem( obecnyKlocek , obecnyX + 1 , obecnyY );
            break;
        case Qt::Key_Down:
            sprobujRuszacKlockiem( obecnyKlocek.odwPrawo() , obecnyX , obecnyY );
            break;
        case Qt::Key_Up:
            sprobujRuszacKlockiem( obecnyKlocek.odwLewo() , obecnyX , obecnyY );
            break;
        case Qt::Key_Space:
             /*!
             * \brief upusc( )
             * Upuszczenie klocka po wciśnięciu spacji.
             */
            upusc();
            break;
        case Qt::Key_V:
             /*!
             * \brief linijkaDoDolu( )
             * Przesunięcie klocka o linijkę w dół po wciśnięciu 'V'.
             */
            linijkaDoDolu();
            break;
        default:
            QFrame::keyPressEvent( event );
    }
}

void Plansza::timerEvent( QTimerEvent *event ) // Rozpoznaje aktualny status rozgrywki (start/pauza), kontroluje prędkość rozgrywki.
{
    if ( event->timerId() == timer.timerId() )
    {
        if ( czyCzeka )
        {
            czyCzeka = false;
            nowyKlocek();
            timer.start( timeoutTime(), this );
        }
        else
        {
            linijkaDoDolu();
        }
    }
    else
    {
        QFrame::timerEvent( event );
    }
}

void Plansza::wyczyscPlansze() // Czyszczenie planszy.
{
    for ( int i = 0 ; i < wysokoscPlanszy * szerokoscPlanszy ; ++i )
    {
        tablica[ i ] = NoShape;
    }
}

void Plansza::upusc() // Upuszczenie klocka na spód planszy (na najniższe możliwe miejsce).
{
    int wysokosc = 0;
    int yY = obecnyY;

    while ( yY > 0 )
    {
        if ( !sprobujRuszacKlockiem( obecnyKlocek , obecnyX , yY - 1 ) )
        {
            break;
        }
        --yY;
        ++wysokosc;
    }

    upuszczonyKlocek( wysokosc );
}

void Plansza::linijkaDoDolu() // Przesunięcie klocka o linijkę w dół (przycisk 'V').
{
    if ( !sprobujRuszacKlockiem( obecnyKlocek , obecnyX , obecnyY - 1 ) )
    {
        upuszczonyKlocek(0);
    }
}

void Plansza::upuszczonyKlocek( int wysokosc ) // Naliczanie punktów po upuszczeniu klocka.
{
    for ( int i = 0 ; i < 4 ; ++i )
    {
        int x = obecnyX + obecnyKlocek.x( i );
        int y = obecnyY - obecnyKlocek.y( i );
        ksztaltO( x , y)  = obecnyKlocek.ksztalt();
    }

    ++liczbaKlockow;

    if ( liczbaKlockow % 25 == 0 )
    {
        ++poziom;
        timer.start( timeoutTime() , this );
        emit zmianaPoziomu( poziom );
    }

    wynik += wysokosc + 7;

    emit zmianaWyniku( wynik );
    usunPelneLinijki();

    if ( !czyCzeka )
    {
        nowyKlocek();
    }
}

void Plansza::usunPelneLinijki() // Skanowanie planszy w celu odnalezienia pełnej linijki.
{
    int liczbaPelnych = 0;

    for ( int i = wysokoscPlanszy - 1 ; i >= 0 ; --i )
    {
        bool czyPelna = true;

        for ( int j = 0 ; j < szerokoscPlanszy ; ++j)
        {
            if ( ksztaltO( j , i ) == NoShape )
            {
                czyPelna = false;
                break;
            }
        }

        if ( czyPelna )
        {
            ++liczbaPelnych;

            for ( int k = i ; k < wysokoscPlanszy - 1 ; ++k )
            {
                for ( int j = 0 ; j < szerokoscPlanszy ; ++j )
                {
                    ksztaltO( j, k ) = ksztaltO( j , k + 1 );
                }
            }

            for ( int j = 0 ; j < szerokoscPlanszy; ++j)
            {
                ksztaltO( j , wysokoscPlanszy - 1 ) = NoShape;
            }
        }
    }

    if ( liczbaPelnych > 0 )
    {
        liczbaUsunietych += liczbaPelnych;
        wynik += 10 * liczbaPelnych;

        emit zmianaUsunietychLinijek( liczbaUsunietych );
        emit zmianaWyniku( wynik );

        timer.start( 500 , this );

        czyCzeka = true;

        obecnyKlocek.ustawKsztalt( NoShape );

        update();
    }
}

void Plansza::nowyKlocek() // Wywołanie następnego klocka.
{
    obecnyKlocek = nastKlocek;
    nastKlocek.losowyKsztalt();
    pokazNastepny();

    obecnyX = szerokoscPlanszy / 2 + 1;
    obecnyY = wysokoscPlanszy - 1 + obecnyKlocek.Ymin();

    if ( !sprobujRuszacKlockiem( obecnyKlocek , obecnyX , obecnyY ) )
    {
        obecnyKlocek.ustawKsztalt( NoShape );
        timer.stop();
        czyTrwa = false;
    }
}

void Plansza::pokazNastepny() // Aktualizacja planszy pokazując następny klocek na niej.
{
    if ( !nastepnyKlocek )
    {
        return;
    }

    int dx = nastKlocek.Xmax() - nastKlocek.Xmin() + 1;
    int dy = nastKlocek.Ymax() - nastKlocek.Ymin() + 1;

    QPixmap pixmap( dx * szerokosc2() , dy * wysokosc2() );
    QPainter painter( &pixmap );

    painter.fillRect( pixmap.rect() , nastepnyKlocek->palette().window() );

    for  (int i = 0 ; i < 4 ; ++i )
    {
        int x = nastKlocek.x(i) - nastKlocek.Xmin();
        int y = nastKlocek.y(i) - nastKlocek.Ymin();

        rysujKlocek( painter , x * szerokosc2() , y * wysokosc2() , nastKlocek.ksztalt() );
    }

    nastepnyKlocek->setPixmap(pixmap);
}

bool Plansza::sprobujRuszacKlockiem( const Klocek &nowyKlocek , int xX , int yY ) // Sprawdzenie, czy można ruszyć klockiem w zadany przez użytkownika sposób.
{
    for ( int i = 0 ; i < 4 ; ++i )
    {
        int x = xX + nowyKlocek.x(i);
        int y = yY - nowyKlocek.y(i);

        if ( x < 0 || x >= szerokoscPlanszy || y < 0 || y >= wysokoscPlanszy )
        {
            return false;
        }

        if ( ksztaltO( x , y ) != NoShape )
        {
            return false;
        }
    }

    obecnyKlocek = nowyKlocek;
    obecnyX = xX;
    obecnyY = yY;

    update();

    return true;
}

void Plansza::rysujKlocek( QPainter &rysuj , int x , int y , KsztaltKlocka ksztalt ) // Kolorowanie klocka.
{
    static constexpr QRgb tabKolor[ 8 ] = { 0x212121 , 0x3F51B5 , 0x00BCD4 , 0x8BC34A , 0xFFC107 , 0xFF5722 , 0x673AB7 , 0x607D8B };

    QColor kolor = tabKolor[ int( ksztalt ) ];

    rysuj.fillRect( x + 1 , y + 1 , szerokosc2() - 2 , wysokosc2() - 2 , kolor );

    rysuj.setPen( kolor.lighter() );
    rysuj.drawLine( x , y + wysokosc2() - 1 , x , y );
    rysuj.drawLine( x , y , x + szerokosc2() - 1 , y );

    rysuj.setPen( kolor.darker() );
    rysuj.drawLine( x + 1 , y + wysokosc2() - 1 , x + szerokosc2() - 1 , y + wysokosc2() - 1 );
    rysuj.drawLine( x + szerokosc2() - 1 , y + wysokosc2() - 1 , x + szerokosc2() - 1 , y + 1 );
}
