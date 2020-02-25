#ifndef _CALCULATOR_H
#define _CALCULATOR_H

#include <QDialog>
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
    void btn0Clicked();
    void btn1Clicked();
    void btn2Clicked();
    void btn3Clicked();
    void btn4Clicked();
    void btn5Clicked();
    void btn6Clicked();
    void btn7Clicked();
    void btn8Clicked();
    void btn9Clicked();
    void btnPeriodClicked();
    void btnEnterClicked();
    void btnDeleteClicked();
    void btnBSClicked();  

    void btnPlusClicked();
    void btnMinusClicked();
    void btnCAClicked();
    void btnAverageClicked();
private:
    double getLastValue( bool popLast );
    void addValue( char value );
    void addLastValue( double value );

    Ui::CCalculator *ui;
    QStringListModel * fModel;
};

#endif // _ALCULATOR_H
