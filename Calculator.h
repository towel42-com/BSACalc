#ifndef _CALCULATOR_H
#define _CALCULATOR_H

#include <QDialog>
#include <memory>
#include <unordered_map>
#include <functional>
#include <list>
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
    void btnNarcissisticClicked();
    void btnFactorsClicked( bool incNum );

    void reportPrime( std::list<int64_t>& factors, int64_t curr, bool incNum );

    void btnPrimeFactorsClicked();
    void btnPerfectClicked();
    void btnSemiPerfectClicked();

    void btnWeirdClicked();
    void btnSublimeClicked();
    void btnAbundantClicked();
private:
    template< typename T >
    T getLastValue( bool popLast );
    void addValue( char value );
    void addLastValue( double value );
    void addLastValue( bool value );
    void addLastValue( int64_t value );
    void addLastValue( const QString & newValue );
    std::pair< int64_t, std::list< int64_t > > getSumOfFactors( int64_t curr, bool properFactors ) const;
    std::pair< bool, std::list< int64_t > > isAbundant( int64_t num ) const;
    std::pair< bool, std::list< int64_t > > isSemiPerfect( int64_t num ) const;
    std::pair< bool, std::list< int64_t > > isPerfect( int64_t num ) const;

    std::list< int64_t > computeFactors( int64_t num ) const;
    std::list< int64_t > computePrimeFactors( int64_t num ) const;
    bool isSemiPerfect( std::vector< int64_t >& numbers, int64_t num ) const;

    std::unique_ptr< Ui::CCalculator > fImpl;
    QStringListModel * fModel;
};

#endif // _ALCULATOR_H
