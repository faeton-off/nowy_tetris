#include "tetrixpiece.h"

#include <QtCore>


void TetrixPiece::setRandomShape()
{
    setShape(TetrixShape(QRandomGenerator::global()->bounded(7) + 1));//generuje losowu figuru 'coords'// генерирует рандомную фигуру coords из представленых в функции shape
}



void TetrixPiece::setShape(TetrixShape shape)//// wykonanie zadania pozycjonowania figury //выполнение задачи позиционирование фигуры
{
    static constexpr int coordsTable[8][4][2] = {
        { { 0, 0 },   { 0, 0 },   { 0, 0 },   { 0, 0 } },
        { { 0, -1 },  { 0, 0 },   { -1, 0 },  { -1, 1 } },
        { { 0, -1 },  { 0, 0 },   { 1, 0 },   { 1, 1 } },
        { { 0, -1 },  { 0, 0 },   { 0, 1 },   { 0, 2 } },
        { { -1, 0 },  { 0, 0 },   { 1, 0 },   { 0, 1 } },
        { { 0, 0 },   { 1, 0 },   { 0, 1 },   { 1, 1 } },
        { { -1, -1 }, { 0, -1 },  { 0, 0 },   { 0, 1 } },
        { { 1, -1 },  { 0, -1 },  { 0, 0 },   { 0, 1 } }
    };                                         //coords - каждая падающяя деталь, это переменная coords которая генерирует 4-блочную фигуру
    for (int i = 0; i < 4 ; i++) {
        for (int j = 0; j < 2; ++j)
            coords[i][j] = coordsTable[shape][i][j]; //do realizacji obrotu o 90 stopni //для реализации поворотов на 90 градусов
    }
    pieceShape = shape;

}



int TetrixPiece::minX() const
{
    int min = coords[0][0];
    for (int i = 1; i < 4; ++i)
        min = qMin(min, coords[i][0]);
    return min;
}
                                         //zwraca minimalne i maksymalne wartości poziome   //возращяют минимальное и максимально значиние по горизонтале
int TetrixPiece::maxX() const

{
    int max = coords[0][0];
    for (int i = 1; i < 4; ++i)
        max = qMax(max, coords[i][0]);
    return max;
}



int TetrixPiece::minY() const
{
    int min = coords[0][1];
    for (int i = 1; i < 4; ++i)
        min = qMin(min, coords[i][1]);
    return min;
}
                                          // zwraca minimalne i maksymalne wartości pionowe  //возращяют минимальное и максимально значиние по вертикале
int TetrixPiece::maxY() const

{
    int max = coords[0][1];
    for (int i = 1; i < 4; ++i)
        max = qMax(max, coords[i][1]);
    return max;
}



TetrixPiece TetrixPiece::rotatedLeft() const // tworzy obrócone kopie siebie o 90 stopni //создаёт поворнутые копии самой себя на 90 градусов
{
    if (pieceShape == SquareShape)
        return *this;

    TetrixPiece result;
    result.pieceShape = pieceShape;
    for (int i = 0; i < 4; ++i) {
        result.setX(i, y(i));
        result.setY(i, -x(i));
    }

    return result;
}


TetrixPiece TetrixPiece::rotatedRight() const
{
    if (pieceShape == SquareShape)
        return *this;

    TetrixPiece result;
    result.pieceShape = pieceShape;
    for (int i = 0; i < 4; ++i) {
        result.setX(i, -y(i));
        result.setY(i, x(i));
    }

    return result;
}
