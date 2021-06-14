#ifndef KLOCEK_H

#define KLOCEK_H

enum KsztaltKlocka { NoShape, ZShape, SShape, LineShape, TShape, SquareShape, LShape, MirroredLShape };

class Klocek
{
    public:
        Klocek() { ustawKsztalt( NoShape ); }

        /*!
         * \brief
         * Generuje klocek o losowym kształcie, który w następnych etapach zostanie zmieniony.
         */
        void losowyKsztalt();

        /*!
         * \brief
         * Ustawia kształt klocka utworzonego poprzez funkcję losowyKsztalt().
         */
        void ustawKsztalt( KsztaltKlocka ksztalt );

        /*!
         * \brief
         * Zwraca aktualny kształt klocka.
         */
        KsztaltKlocka ksztalt() const
        {
            return klocek_ksztalt;
        }

        /*!
         * \brief
         * Tworzy oś poziomą X.
         */
        int x( int index ) const
        {
            return wsp[ index ][ 0 ];
        }

        /*!
         * \brief
         * Tworzy oś pionową Y.
         */
        int y( int index ) const
        {
            return wsp[ index ][ 1 ];
        }

        /*!
         * \brief
         * Zwraca minimalną wartość w poziomie.
         */
        int Xmin() const;

        /*!
         * \brief
         * Zwraca maksymalną wartość w poziomie.
         */
        int Xmax() const;

        /*!
         * \brief
         * Zwraca minimalną wartość w pionie.
         */
        int Ymin() const;

        /*!
         * \brief
         * Zwraca maksymalną wartość w pionie.
         */
        int Ymax() const;

        /*!
         * \brief
         * Tworzy kopię klocka obróconą o 90 stopni w lewo.
         */
        Klocek odwLewo() const;

        /*!
         * \brief
         * Tworzy kopię klocka obróconą o 90 stopni w prawo.
         */
        Klocek odwPrawo() const;

    private:
        /*!
         * \brief
         * Ustawia wartości osi poziomej.
         */
        void ustawX( int index , int x )
        {
            wsp[ index ][ 0 ] = x;
        }

        /*!
         * \brief
         * Ustawia wartości osi pionowej.
         */
        void ustawY( int index , int y )
        {
            wsp[ index ][ 1 ] = y;
        }

        KsztaltKlocka klocek_ksztalt;

        int wsp[ 4 ][ 2 ];
};

#endif
