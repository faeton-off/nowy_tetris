QT += widgets

HEADERS       = \
                klocek.h \
                okno.h \
                plansza.h
SOURCES       = main.cpp \
                klocek.cpp \
                okno.cpp \
                plansza.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/widgets/tetrix
INSTALLS += target

FORMS += \
    form.ui
