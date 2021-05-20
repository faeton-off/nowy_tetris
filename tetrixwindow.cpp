
#include "tetrixboard.h"
#include "tetrixwindow.h"

#include <QCoreApplication>
#include <QGridLayout>
#include <QLabel>
#include <QLCDNumber>
#include <QPushButton>


TetrixWindow::TetrixWindow(QWidget *parent)
    : QWidget(parent), board(new TetrixBoard) //создание игрового поля //tworzenie ZG                     ZG- Zona Gry
{

    nextPieceLabel = new QLabel;
    nextPieceLabel->setFrameStyle(QFrame::Box | QFrame::Raised);
    nextPieceLabel->setAlignment(Qt::AlignCenter);
    board->setNextPieceLabel(nextPieceLabel);

    scoreLcd = new QLCDNumber(5); //widget do oceniania QLCDNumber //виджет для подсчёта очков QLCDNumber
    scoreLcd->setSegmentStyle(QLCDNumber::Filled);  //очки //punkty

    levelLcd = new QLCDNumber(2);                   //уровень //level
    levelLcd->setSegmentStyle(QLCDNumber::Filled);
    linesLcd = new QLCDNumber(5);
    linesLcd->setSegmentStyle(QLCDNumber::Filled);  //линии //linie


    startButton = new QPushButton(tr("&Start"));
    startButton->setFocusPolicy(Qt::NoFocus);
    quitButton = new QPushButton(tr("&Quit"));     // przyciski, nie otrzymują fokusa klawiatury //кнопки, не получают фокус с клавиатуры
    quitButton->setFocusPolicy(Qt::NoFocus);
    pauseButton = new QPushButton(tr("&Pause"));

    pauseButton->setFocusPolicy(Qt::NoFocus);


    connect(startButton, &QPushButton::clicked, board, &TetrixBoard::start);

    connect(quitButton , &QPushButton::clicked, qApp, &QCoreApplication::quit);
    connect(pauseButton, &QPushButton::clicked, board, &TetrixBoard::pause);
#if __cplusplus >= 201402L
    connect(board, &TetrixBoard::scoreChanged,
            scoreLcd, qOverload<int>(&QLCDNumber::display));
    connect(board, &TetrixBoard::levelChanged,
            levelLcd, qOverload<int>(&QLCDNumber::display));
    connect(board, &TetrixBoard::linesRemovedChanged,
            linesLcd, qOverload<int>(&QLCDNumber::display));
#else
    connect(board, &TetrixBoard::scoreChanged,
            scoreLcd, QOverload<int>::of(&QLCDNumber::display));
    connect(board, &TetrixBoard::levelChanged,
            levelLcd, QOverload<int>::of(&QLCDNumber::display));
    connect(board, &TetrixBoard::linesRemovedChanged,
            linesLcd, QOverload<int>::of(&QLCDNumber::display));
#endif

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(createLabel(tr("NEXT")), 0, 0);
    layout->addWidget(nextPieceLabel, 1, 0);
    layout->addWidget(createLabel(tr("LEVEL")), 2, 0);
    layout->addWidget(levelLcd, 3, 0);
    layout->addWidget(startButton, 4, 0);    //tworzenie inskrypcji //создание надписей
    layout->addWidget(board, 0, 1, 6, 1);
    layout->addWidget(createLabel(tr("SCORE")), 0, 2);
    layout->addWidget(scoreLcd, 1, 2);
    layout->addWidget(createLabel(tr("LINES REMOVED")), 2, 2);
    layout->addWidget(linesLcd, 3, 2);
    layout->addWidget(quitButton, 4, 2);
    layout->addWidget(pauseButton, 5, 2);
    setLayout(layout);

    setWindowTitle(tr("Tetrix"));
    resize(550, 370);
}

QLabel *TetrixWindow::createLabel(const QString &text)
{
    QLabel *label = new QLabel(text);
    label->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    return label;
}

