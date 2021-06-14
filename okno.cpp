#include "plansza.h"
#include "okno.h"
#include <QLabel>
#include <QLCDNumber>
#include <QPushButton>
#include <QCoreApplication>
#include <QGridLayout>

Okno::Okno( QWidget *parent ) : QWidget( parent ) , plansza( new Plansza )
{
    kolejnyKlocek = new QLabel;
    kolejnyKlocek->setFrameStyle( QFrame::Box | QFrame::Raised );
    kolejnyKlocek->setAlignment( Qt::AlignCenter );
    plansza->ustNastepnyKlocek( kolejnyKlocek );

    wyniklcd = new QLCDNumber( 5 ); // pokazuje aktualny wynik
    wyniklcd->setSegmentStyle( QLCDNumber::Filled );

    poziomlcd = new QLCDNumber( 2 ); // pokazuje aktualny poziom
    poziomlcd->setSegmentStyle( QLCDNumber::Filled );

    linijkilcd = new QLCDNumber( 5 ); // pokazuje wypełnione linijki
    linijkilcd->setSegmentStyle( QLCDNumber::Filled );

    przyciskStart = new QPushButton( tr( "&Start" ) ); // pokazuje przycisk "Start"
    przyciskStart->setFocusPolicy( Qt::NoFocus );

    przyciskWyjdz = new QPushButton( tr( "&Wyjdź" ) ); // pokazuje przycisk "Wyjdź"
    przyciskWyjdz->setFocusPolicy( Qt::NoFocus );

    przyciskPauza = new QPushButton( tr( "&Pauza/Wznów" ) ); // pokazuje przycisk "Pauza/Wyjdź"
    przyciskPauza->setFocusPolicy( Qt::NoFocus );

    connect( przyciskStart , &QPushButton::clicked , plansza, &Plansza::startGry );
    connect( przyciskWyjdz , &QPushButton::clicked , qApp, &QCoreApplication::quit );
    connect( przyciskPauza , &QPushButton::clicked , plansza, &Plansza::pauzaGry );

    connect( plansza , &Plansza::zmianaWyniku , wyniklcd , QOverload<int>::of( &QLCDNumber::display ) );
    connect( plansza , &Plansza::zmianaPoziomu , poziomlcd , QOverload<int>::of( &QLCDNumber::display ) );
    connect( plansza , &Plansza::zmianaUsunietychLinijek , linijkilcd , QOverload<int>::of( &QLCDNumber::display ) );

    QGridLayout *layout = new QGridLayout;
        layout->addWidget( stworzOkno( tr( "NASTĘPNY KLOCEK" ) ) , 0 , 0 );
        layout->addWidget( kolejnyKlocek , 1 , 0 );
        layout->addWidget( stworzOkno( tr( "POZIOM" ) ) , 2 , 0 );
        layout->addWidget( poziomlcd, 3 , 0 );
        layout->addWidget( przyciskStart , 4 , 0 );
        layout->addWidget( plansza, 0 , 1 , 6 , 1 );
        layout->addWidget( stworzOkno( tr( "WYNIK" ) ) , 0 , 2 );
        layout->addWidget( wyniklcd , 1 , 2 );
        layout->addWidget( stworzOkno( tr( "USUNIĘTE LINIJKI" ) ), 2 , 2 );
        layout->addWidget( linijkilcd , 3 , 2 );
        layout->addWidget( przyciskWyjdz , 4 , 2 );
        layout->addWidget( przyciskPauza , 5 , 2 );
        setLayout( layout );

        setWindowTitle( tr( "@Tetris #Dmitry #Jakub" ) );
        resize( 550 , 370 );
}

QLabel *Okno::stworzOkno( const QString &text )
{
    QLabel *label = new QLabel( text );
    label->setAlignment( Qt::AlignHCenter | Qt::AlignBottom) ;
    return label;
}
