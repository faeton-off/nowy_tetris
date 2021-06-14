#include "klocek.h"
#include <QtCore>

void Klocek::losowyKsztalt()
{
    /*!
     * \brief ustawKsztalt( )
     * Generowanie losowego kształtu klocka.
     */
    ustawKsztalt( KsztaltKlocka( QRandomGenerator::global()->bounded( 7 ) + 1 ) );
}

void Klocek::ustawKsztalt( KsztaltKlocka ksztalt )
{
    static constexpr int wspTablica[8][4][2] = {
        { { 0 , 0 },   { 0 , 0 },   { 0 , 0 },   { 0 , 0 } },
        { { 0 , -1 },  { 0 , 0 },   { -1 , 0 },  { -1 , 1 } },
        { { 0 , -1 },  { 0 , 0 },   { 1 , 0 },   { 1 , 1 } },
        { { 0 , -1 },  { 0 , 0 },   { 0 , 1 },   { 0 , 2 } },
        { { -1 , 0 },  { 0 , 0 },   { 1 , 0 },   { 0 , 1 } },
        { { 0 , 0 },   { 1 , 0 },   { 0 , 1 },   { 1 , 1 } },
        { { -1 , -1 }, { 0 , -1 },  { 0 , 0 },   { 0 , 1 } },
        { { 1 , -1 },  { 0 , -1 },  { 0 , 0 },   { 0 , 1 } }
    }; // tablica zawierająca możliwe miejsce dla ustawionego klocka

    for ( int i = 0 ; i < 4 ; i++ )
    {
        for ( int j = 0 ; j < 2 ; ++j )
        {     
            wsp[ i ][ j ] = wspTablica[ ksztalt ][ i ][ j ]; // obrót klocka
        }
    }

    klocek_ksztalt = ksztalt;
}

int Klocek::Xmin() const
{
    int min = wsp[ 0 ][ 0 ];

    for ( int i = 1 ; i < 4 ; ++i )
    {
        min = qMin( min , wsp[ i ][ 0 ] );
    }

    return min;
}

int Klocek::Xmax() const
{
    int max = wsp[ 0 ][ 0 ];

    for ( int i = 1 ; i < 4; ++i )
    {
        max = qMax( max , wsp[ i ][ 0 ] );
    }

    return max;
}

int Klocek::Ymin() const
{
    int min = wsp[ 0 ][ 1 ];

    for ( int i = 1 ; i < 4 ; ++i )
    {
        min = qMin( min , wsp[ i ][ 1 ] );
    }

    return min;
}

int Klocek::Ymax() const
{
    int max = wsp[ 0 ][ 1 ];

    for ( int i = 1 ; i < 4 ; ++i )
    {
        max = qMax( max , wsp[ i ][ 1 ] );
    }

    return max;
}

Klocek Klocek::odwLewo() const
{
    if (klocek_ksztalt == SquareShape)
    {
        return *this;
    }

    /*!
     * \brief result( )
     * Pokazuje klocek po obróceniu o 90 stopni w lewo.
     */
    Klocek result;
    result.klocek_ksztalt = klocek_ksztalt;

    for ( int i = 0 ; i < 4 ; ++i )
    {
        result.ustawX( i , y( i ) );
        result.ustawY( i , -x( i ) );
    }

    return result;
}

Klocek Klocek::odwPrawo() const
{
    if (klocek_ksztalt == SquareShape)
    {
        return *this;
    }

    /*!
     * \brief result( )
     * Pokazuje klocek po obróceniu o 90 stopni w prawo.
     */
    Klocek result;
    result.klocek_ksztalt = klocek_ksztalt;

    for ( int i = 0 ; i < 4 ; ++i )
    {
        result.ustawX( i , -y( i ) );
        result.ustawY( i , x( i ) );
    }

    return result;
}
