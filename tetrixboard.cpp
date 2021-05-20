
#include "tetrixboard.h"

#include <QKeyEvent>
#include <QLabel>
#include <QPainter>

//    ЛОГИКА ИГРЫ И ОТОБРАЖЕНИЕ ФУНКЦИЙ
TetrixBoard::TetrixBoard(QWidget *parent)
    : QFrame(parent), isStarted(false), isPaused(false)
{
    setFrameStyle(QFrame::Panel | QFrame::Sunken);
    setFocusPolicy(Qt::StrongFocus); // получение ввода с клавиатуры //pobieranie danych z klawiatury
    clearBoard();

    nextPiece.setRandomShape();
}

void TetrixBoard::setNextPieceLabel(QLabel *label)
{
    nextPieceLabel = label;
}
                                                //рисовка этикеток рядом с игровой площадкой //rysowanie kształtów obok ZG
QSize TetrixBoard::sizeHint() const
{
    return QSize(BoardWidth * 15 + frameWidth() * 2,
                 BoardHeight * 15 + frameWidth() * 2);
}

QSize TetrixBoard::minimumSizeHint() const

{
    return QSize(BoardWidth * 5 + frameWidth() * 2,
                 BoardHeight * 5 + frameWidth() * 2);
}
                                                  //wskazówka, że ​​obszar gry nie powinien być mniejszy niż określony rozmiar
                                                //подсказка что игровая зона не должна быть меньше заданного размера
void TetrixBoard::start()                      //rozpoczynając nową grę, resetuje stan (gra, punkty i poziom) oraz zawartość pól
                                               //начало новой игры, сбрасывает состояние (игры, очков и уровня) и содержания поля
{
    if (isPaused)
        return;

    isStarted = true;
    isWaitingAfterLine = false;
    numLinesRemoved = 0;
    numPiecesDropped = 0;
    score = 0;
    level = 1;
    clearBoard();

    emit linesRemovedChanged(numLinesRemoved);
    emit scoreChanged(score);
    emit levelChanged(level);

    newPiece();
    timer.start(timeoutTime(), this);
}



void TetrixBoard::pause()// временная остановка игры //tymczasowe zatrzymanie gry
{
    if (!isStarted)
        return;

    isPaused = !isPaused;
    if (isPaused) {//git proverka
        timer.stop();
    } else {
        timer.start(timeoutTime(), this);
    }
    update();

}

void TetrixBoard::paintEvent(QPaintEvent *event) //рисовка доски //rysunek tablicy
{
    QFrame::paintEvent(event);

    QPainter painter(this);
    QRect rect = contentsRect();


    if (isPaused) {
        painter.drawText(rect, Qt::AlignCenter, tr("PAUSE"));
        return;
        }



    int boardTop = rect.bottom() - BoardHeight*squareHeight();

    for (int i = 0; i < BoardHeight; ++i) {
        for (int j = 0; j < BoardWidth; ++j) {
            TetrixShape shape = shapeAt(j, BoardHeight - i - 1);
            if (shape != NoShape)
                drawSquare(painter, rect.left() + j * squareWidth(), // рисовка блока в данной позиции //rysowanie bloku w zadanym położeniu
                           boardTop + i * squareHeight(), shape);
        }

    }



    if (curPiece.shape() != NoShape) {
        for (int i = 0; i < 4; ++i) {
            int x = curX + curPiece.x(i);
            int y = curY - curPiece.y(i);
            drawSquare(painter, rect.left() + x * squareWidth(),
                       boardTop + (BoardHeight - y - 1) * squareHeight(),
                       curPiece.shape());
        }

    }

}



void TetrixBoard::keyPressEvent(QKeyEvent *event)
//jeśli nie ma aktualnej figury do sterowania, po prostu przekazujemy zdarzenie do klasy bazowej.
//если нет текущей формы для управления, мы просто передаем событие базовому классу.
// naciśnij klawisz, blok jest rysowany w innej pozycji
//нажимаешь клавишу, рисуется блок в другом положении
{
    if (!isStarted || isPaused || curPiece.shape() == NoShape) {
        QFrame::keyPressEvent(event);
        return;
    }



    switch (event->key()) {
    case Qt::Key_Left:
        tryMove(curPiece, curX - 1, curY);
        break;
    case Qt::Key_Right:
        tryMove(curPiece, curX + 1, curY);
        break;
    case Qt::Key_Down:
        tryMove(curPiece.rotatedRight(), curX, curY);
        break;
    case Qt::Key_Up:
        tryMove(curPiece.rotatedLeft(), curX, curY);
        break;
    case Qt::Key_Space:
        dropDown();
        break;
    case Qt::Key_D:
        oneLineDown();
        break;
    default:
        QFrame::keyPressEvent(event);
    }

}


void TetrixBoard::timerEvent(QTimerEvent *event) //QBasicTimer - управления скоростью с которой фишки падают в нижнюю часть игрового поля
                                                //QBasicTimer - kontrolować prędkość, z jaką figura spadaje na spód pola gry
{
    if (event->timerId() == timer.timerId()) {
        if (isWaitingAfterLine) {
            isWaitingAfterLine = false;
            newPiece();
            timer.start(timeoutTime(), this);
        } else {
            oneLineDown();
        }
    } else {
        QFrame::timerEvent(event);

    }

}

void TetrixBoard::clearBoard() //очистка игрового поля //czyszczenie ZG
{
    for (int i = 0; i < BoardHeight * BoardWidth; ++i)
        board[i] = NoShape;
}

void TetrixBoard::dropDown() // падение вниз // spadanie w dół
{
    int dropHeight = 0;
    int newY = curY;
    while (newY > 0) {
        if (!tryMove(curPiece, curX, newY - 1))
            break;
        --newY;
        ++dropHeight;
    }
    pieceDropped(dropHeight);

}



void TetrixBoard::oneLineDown() //przejdź o jedną linię w dół //перемещение вниз на одну строку
{
    if (!tryMove(curPiece, curX, curY - 1))
        pieceDropped(0); //jeśli figura nie może poruszyć się o jedną linię //если кусок не может перместиться на одну строку
}

void TetrixBoard::pieceDropped(int dropHeight)//naliczenia punktów //начсление очков игроку за (размещение фигуры, проверку полных рядов на доске), создание новой фигуры
{
    for (int i = 0; i < 4; ++i) {
        int x = curX + curPiece.x(i);
        int y = curY - curPiece.y(i);
        shapeAt(x, y) = curPiece.shape();
    }

    ++numPiecesDropped;
    if (numPiecesDropped % 25 == 0) {
        ++level;
        timer.start(timeoutTime(), this);
        emit levelChanged(level);
    }

    score += dropHeight + 7;
    emit scoreChanged(score);
    removeFullLines();

    if (!isWaitingAfterLine)
        newPiece();

}



void TetrixBoard::removeFullLines()//skanuje ZG od góry do dołu, aby znaleźć puste miejsce w każdej linii//сканирует доску сверху вниз, для поиска пустого места в каждой строке
{
    int numFullLines = 0;

    for (int i = BoardHeight - 1; i >= 0; --i) {
        bool lineIsFull = true;

        for (int j = 0; j < BoardWidth; ++j) {
            if (shapeAt(j, i) == NoShape) {
                lineIsFull = false;
                break;
            }
        }

        if (lineIsFull) {

            ++numFullLines;
            for (int k = i; k < BoardHeight - 1; ++k) {
                for (int j = 0; j < BoardWidth; ++j)
                    shapeAt(j, k) = shapeAt(j, k + 1);
            }

            for (int j = 0; j < BoardWidth; ++j)
                shapeAt(j, BoardHeight - 1) = NoShape;
        }

    }



    if (numFullLines > 0) {              //aktualizowanie punktów i linii  //обновление очков и линий
        numLinesRemoved += numFullLines;
        score += 10 * numFullLines;
        emit linesRemovedChanged(numLinesRemoved);
        emit scoreChanged(score);

        timer.start(500, this);
        isWaitingAfterLine = true;//wstrzymywanie gry w trakcie (removeline) //приостановка игры во время (removeline)
        curPiece.setShape(NoShape);
        update();
    }

}



void TetrixBoard::newPiece() // tworzenie nowej figury //создание новой фигуры
{
    curPiece = nextPiece;
    nextPiece.setRandomShape();
    showNextPiece();
    curX = BoardWidth / 2 + 1;
    curY = BoardHeight - 1 + curPiece.minY();

    if (!tryMove(curPiece, curX, curY)) { //zatrzymanie gry //остановка игры
        curPiece.setShape(NoShape);
        timer.stop();
        isStarted = false;
    }

}



void TetrixBoard::showNextPiece()//aktualizowanie obszaru o następnej figury //обновление области со следующей фигурой
{
    if (!nextPieceLabel)
        return;

    int dx = nextPiece.maxX() - nextPiece.minX() + 1;
    int dy = nextPiece.maxY() - nextPiece.minY() + 1;

    QPixmap pixmap(dx * squareWidth(), dy * squareHeight());
    QPainter painter(&pixmap);
    painter.fillRect(pixmap.rect(), nextPieceLabel->palette().window());

    for (int i = 0; i < 4; ++i) {
        int x = nextPiece.x(i) - nextPiece.minX();
        int y = nextPiece.y(i) - nextPiece.minY();
        drawSquare(painter, x * squareWidth(), y * squareHeight(),
                   nextPiece.shape());
    }
    nextPieceLabel->setPixmap(pixmap);

}



bool TetrixBoard::tryMove(const TetrixPiece &newPiece, int newX, int newY)// sprawdzanie, czy figura może mieć podane współrzędne //проверки, может ли фигура находится в данных координатах
{
    for (int i = 0; i < 4; ++i) {
        int x = newX + newPiece.x(i);
        int y = newY - newPiece.y(i);
        if (x < 0 || x >= BoardWidth || y < 0 || y >= BoardHeight)
            return false;
        if (shapeAt(x, y) != NoShape)
            return false;
    }



    curPiece = newPiece;
    curX = newX;
    curY = newY;
    update();
    return true;
}



void TetrixBoard::drawSquare(QPainter &painter, int x, int y, TetrixShape shape) //// kolor i rysowanie figur //цвет и рисовка фигур
{
    static constexpr QRgb colorTable[8] = {
        0x212121, 0x3F51B5, 0x00BCD4, 0x8BC34A,
        0xFFC107, 0xFF5722, 0x673AB7, 0x607D8B,
    };

    QColor color = colorTable[int(shape)];
    painter.fillRect(x + 1, y + 1, squareWidth() - 2, squareHeight() - 2,
                     color);

    painter.setPen(color.lighter());
    painter.drawLine(x, y + squareHeight() - 1, x, y);
    painter.drawLine(x, y, x + squareWidth() - 1, y);

    painter.setPen(color.darker());
    painter.drawLine(x + 1, y + squareHeight() - 1,
                     x + squareWidth() - 1, y + squareHeight() - 1);
    painter.drawLine(x + squareWidth() - 1, y + squareHeight() - 1,
                     x + squareWidth() - 1, y + 1);
}

