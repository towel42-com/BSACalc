#include "main.h"
#include "Calculator.h"

#include <QApplication>
#include <QLabel>

int main( int argc, char ** argv )
{
    QApplication appl( argc, argv );
    Q_INIT_RESOURCE( application );
    CCalculator calc;
    return calc.exec();
}
