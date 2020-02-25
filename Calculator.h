#ifndef _CALCULATOR_H
#define _CALCULATOR_H

#include <QDialog>
#include <memory>
#include <functional>
namespace Ui {class CCalculator;};

class QStringListModel;

class CCalculator : public QDialog
{
    Q_OBJECT

public:
    CCalculator(QWidget *parent = 0);
    ~CCalculator();
    void keyPressEvent( QKeyEvent * event );

    int numValues() const;
    bool eventFilter(QObject *obj, QEvent *event);
    
public slots:
    void binaryOperatorClicked( char op );

    void btnEnterClicked();

    void btnCClicked();
    void btnDelClicked();
    void btnBSClicked();

    void btnAverageClicked();
private:
    void binaryOperator( const std::function< double( double, double ) >& func );
    double getLastValue( bool popLast );
    void addValue( char value );
    void addLastValue( double value );

    std::unique_ptr< Ui::CCalculator > fImpl;
    QStringListModel * fModel;
};

#endif // _ALCULATOR_H
