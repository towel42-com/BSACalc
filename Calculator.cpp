// The MIT License( MIT )
//
// Copyright( c ) 2020 Scott Aron Bloom
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files( the "Software" ), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sub-license, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "Calculator.h"
#include "Utils/utils.h"

#include "ui_Calculator.h"

#include <QStringListModel>
#include <QKeyEvent>
#include <QMessageBox>
#include <unordered_map>
#include <cmath>

class CStringListModel : public QStringListModel
{
public:
    CStringListModel( QObject* parent ) :
        QStringListModel( parent )
    {
    }

    QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const override
    {
        if ( role == Qt::TextAlignmentRole )
            return static_cast<int>( Qt::AlignVCenter | Qt::AlignRight );
        return QStringListModel::data( index, role );
    }
};

CCalculator::CCalculator( QWidget* parent )
    : QDialog( parent ),
    fImpl( new Ui::CCalculator )
{
    fImpl->setupUi( this );
    //setWindowFlags( windowFlags() & ~Qt::WindowContextHelpButtonHint );

    fModel = new CStringListModel( this );
    fImpl->values->setModel( fModel );
    (void)connect( fModel, &QAbstractItemModel::dataChanged, this, [ this ]() { slotDataChanged(); } );
    (void)connect( fModel, &QAbstractItemModel::modelReset, this, [ this ]() { slotDataChanged(); } );
    (void)connect( fModel, &QAbstractItemModel::rowsInserted, this, [ this ]() { slotDataChanged(); } );
    (void)connect( fModel, &QAbstractItemModel::rowsRemoved, this, [ this ]() { slotDataChanged(); } );
    (void)connect( fModel, &QAbstractItemModel::rowsMoved, this, [ this ]() { slotDataChanged(); } );

    (void)connect( fImpl->btn_0, &QToolButton::clicked, this, [this](){ addValue( '0' ); } );
    (void)connect( fImpl->btn_1, &QToolButton::clicked, this, [this](){ addValue( '1' ); } );
    (void)connect( fImpl->btn_2, &QToolButton::clicked, this, [this](){ addValue( '2' ); } );
    (void)connect( fImpl->btn_3, &QToolButton::clicked, this, [this](){ addValue( '3' ); } );
    (void)connect( fImpl->btn_4, &QToolButton::clicked, this, [this](){ addValue( '4' ); } );
    (void)connect( fImpl->btn_5, &QToolButton::clicked, this, [this](){ addValue( '5' ); } );
    (void)connect( fImpl->btn_6, &QToolButton::clicked, this, [this](){ addValue( '6' ); } );
    (void)connect( fImpl->btn_7, &QToolButton::clicked, this, [this](){ addValue( '7' ); } );
    (void)connect( fImpl->btn_8, &QToolButton::clicked, this, [this](){ addValue( '8' ); } );
    (void)connect( fImpl->btn_9, &QToolButton::clicked, this, [this](){ addValue( '9' ); } );
    (void)connect( fImpl->btn_period, &QToolButton::clicked, this, [ this ]() { addValue( '.' ); } );
    (void)connect( fImpl->btn_plusMinus, &QToolButton::clicked, this, [ this ]() { slotNegate(); } );
    (void)connect( fImpl->btn_percent, &QToolButton::clicked, this, [ this ]() { slotPercent(); } );
    (void)connect( fImpl->btn_invert, &QToolButton::clicked, this, [ this ]() { slotInvert(); } );
    (void)connect( fImpl->btn_square, &QToolButton::clicked, this, [ this ]() { slotSquare(); } );
    (void)connect( fImpl->btn_sqrt, &QToolButton::clicked, this, [ this ]() { slotSquareRoot(); } );

    (void)connect( fImpl->btn_enter, &QToolButton::clicked, this, &CCalculator::btnEnterClicked );
    (void)connect( fImpl->btn_C, &QToolButton::clicked, this, &CCalculator::btnCClicked );
    (void)connect( fImpl->btn_Del, &QToolButton::clicked, this, &CCalculator::btnDelClicked );
    (void)connect( fImpl->btn_BS, &QToolButton::clicked, this, &CCalculator::btnBSClicked );

    (void)connect( fImpl->btn_plus, &QToolButton::clicked, this, [this](){ binaryOperatorClicked( '+' ); } );
    (void)connect( fImpl->btn_minus, &QToolButton::clicked, this, [ this ]() { binaryOperatorClicked( '-' ); } );
    (void)connect( fImpl->btn_mult, &QToolButton::clicked, this, [ this ]() { binaryOperatorClicked( '*' ); } );
    (void)connect( fImpl->btn_div, &QToolButton::clicked, this, [ this ]() { binaryOperatorClicked( '/' ); } );

    (void)connect( fImpl->btn_Average, &QToolButton::clicked, this, &CCalculator::btnAverageClicked );
    (void)connect( fImpl->btn_Median, &QToolButton::clicked, this, &CCalculator::btnMedianClicked );
    (void)connect( fImpl->btn_StdDeviation, &QToolButton::clicked, this, &CCalculator::btnStdDeviationClicked );
    (void)connect( fImpl->btn_SampleStdDeviation, &QToolButton::clicked, this, &CCalculator::btnSampleStdDeviationClicked );
    (void)connect( fImpl->btn_PctStdDeviation, &QToolButton::clicked, this, &CCalculator::btnPctStdDeviationClicked );

    (void)connect( fImpl->btn_Narcissistic, &QToolButton::clicked, this, &CCalculator::btnNarcissisticClicked );
    (void)connect( fImpl->btn_Factors, &QToolButton::clicked, this, [this](){ btnFactorsClicked( true ); } );
    (void)connect( fImpl->btn_ProperFactors, &QToolButton::clicked, this, [ this ]() { btnFactorsClicked( false ); } );
    (void)connect( fImpl->btn_PrimeFactors, &QToolButton::clicked, this, [ this ]() { btnPrimeFactorsClicked(); } );
    (void)connect( fImpl->btn_Perfect, &QToolButton::clicked, this, [ this ]() { btnPerfectClicked(); } );
    (void)connect( fImpl->btn_SemiPerfect, &QToolButton::clicked, this, [ this ]() { btnSemiPerfectClicked(); } );
    
    (void)connect( fImpl->btn_Abundant, &QToolButton::clicked, this, [ this ]() { btnAbundantClicked(); } );
    (void)connect( fImpl->btn_Weird, &QToolButton::clicked, this, [ this ]() { btnWeirdClicked(); } );
    (void)connect( fImpl->btn_Sublime, &QToolButton::clicked, this, [ this ]() { btnSublimeClicked(); } );
    (void)connect( fImpl->btn_Factorial, &QPushButton::clicked, this, [ this ]() { btnFactorialClicked(); } );
    (void)connect( fImpl->btnPower, &QPushButton::clicked, this, [ this ]() { btnFactorClicked(); } );

    (void)connect( fImpl->btn_sin, &QToolButton::clicked, this, [ this ]() { trigOperatorClicked( ETrigOperator::eSin ); } );
    (void)connect( fImpl->btn_arcsin, &QToolButton::clicked, this, [ this ]() { trigOperatorClicked( ETrigOperator::eArcSin ); } );
    (void)connect( fImpl->btn_sinh, &QToolButton::clicked, this, [ this ]() { trigOperatorClicked( ETrigOperator::eSinH ); } );
    (void)connect( fImpl->btn_cos, &QToolButton::clicked, this, [ this ]() { trigOperatorClicked( ETrigOperator::eCos ); } );
    (void)connect( fImpl->btn_arccos, &QToolButton::clicked, this, [ this ]() { trigOperatorClicked( ETrigOperator::eArcCos ); } );
    (void)connect( fImpl->btn_cosh, &QToolButton::clicked, this, [ this ]() { trigOperatorClicked( ETrigOperator::eCosH ); } );
    (void)connect( fImpl->btn_tan, &QToolButton::clicked, this, [ this ]() { trigOperatorClicked( ETrigOperator::eTan ); } );
    (void)connect( fImpl->btn_arctan, &QToolButton::clicked, this, [ this ]() { trigOperatorClicked( ETrigOperator::eArcTan ); } );
    (void)connect( fImpl->btn_tanh, &QToolButton::clicked, this, [ this ]() { trigOperatorClicked( ETrigOperator::eTanH ); } );
    (void)connect( fImpl->btn_sec, &QToolButton::clicked, this, [ this ]() { trigOperatorClicked( ETrigOperator::eSec ); } );
    (void)connect( fImpl->btn_arcsec, &QToolButton::clicked, this, [ this ]() { trigOperatorClicked( ETrigOperator::eArcSec ); } );
    (void)connect( fImpl->btn_sech, &QToolButton::clicked, this, [ this ]() { trigOperatorClicked( ETrigOperator::eSecH ); } );
    (void)connect( fImpl->btn_csc, &QToolButton::clicked, this, [ this ]() { trigOperatorClicked( ETrigOperator::eCsc ); } );
    (void)connect( fImpl->btn_arccsc, &QToolButton::clicked, this, [ this ]() { trigOperatorClicked( ETrigOperator::eArcCsc ); } );
    (void)connect( fImpl->btn_csch, &QToolButton::clicked, this, [ this ]() { trigOperatorClicked( ETrigOperator::eCscH ); } );
    (void)connect( fImpl->btn_cot, &QToolButton::clicked, this, [ this ]() { trigOperatorClicked( ETrigOperator::eCot ); } );
    (void)connect( fImpl->btn_arccot, &QToolButton::clicked, this, [ this ]() { trigOperatorClicked( ETrigOperator::eArcCot ); } );
    (void)connect( fImpl->btn_coth, &QToolButton::clicked, this, [ this ]() { trigOperatorClicked( ETrigOperator::eCotH ); } );

    fImpl->values->installEventFilter( this );
    setFocus( Qt::MouseFocusReason );
    initMaps();
    slotDataChanged();
}

void CCalculator::initMaps()
{
    fOpMap =
    {
          { '+', [ this ]( double lhs, double rhs ) { return lhs + rhs; } }
         ,{ '-', [ this ]( double lhs, double rhs ) { return lhs - rhs; } }
         ,{ '*', [ this ]( double lhs, double rhs ) { return lhs * rhs; } }
         ,{ '/', [ this ]( double lhs, double rhs ) { return lhs / rhs; } }
    };

    fNumRowsPerFunctionMap =
    {
         { fImpl->btn_0, 0 }
        ,{ fImpl->btn_1, 0 }
        ,{ fImpl->btn_2, 0 }
        ,{ fImpl->btn_3, 0 }
        ,{ fImpl->btn_4, 0 }
        ,{ fImpl->btn_5, 0 }
        ,{ fImpl->btn_6, 0 }
        ,{ fImpl->btn_7, 0 }
        ,{ fImpl->btn_8, 0 }
        ,{ fImpl->btn_9, 0 }
        ,{ fImpl->btn_enter, 1 }
        ,{ fImpl->btn_plus, 2 }
        ,{ fImpl->btn_minus, 2 }
        ,{ fImpl->btn_mult, 2 }
        ,{ fImpl->btn_div, 2 }
        ,{ fImpl->btn_C, 1 }
        ,{ fImpl->btn_BS, 1 }
        ,{ fImpl->btn_Del, 1 }
        ,{ fImpl->btn_period, 0 }
        ,{ fImpl->btn_plusMinus, 1 }
        ,{ fImpl->btn_percent, 1 }
        ,{ fImpl->btn_invert, 1 }
        ,{ fImpl->btn_square, 1 }
        ,{ fImpl->btn_sqrt, 1 }
        ,{ fImpl->btn_Average, 1 }
        ,{ fImpl->btn_Median, 1 }
        ,{ fImpl->btn_StdDeviation, 1 }
        ,{ fImpl->btn_SampleStdDeviation, 1 }
        ,{ fImpl->btn_PctStdDeviation, 1 }
        ,{ fImpl->btn_Narcissistic, 1 }
        ,{ fImpl->btn_Factors, 1 }
        ,{ fImpl->btn_ProperFactors, 1 }
        ,{ fImpl->btn_PrimeFactors, 1 }
        ,{ fImpl->btn_Perfect, 1 }
        ,{ fImpl->btn_SemiPerfect, 1 }
        ,{ fImpl->btn_Abundant, 1 }
        ,{ fImpl->btn_Weird, 1 }
        ,{ fImpl->btn_Sublime, 1 }
        ,{ fImpl->btn_Factorial, 1 }
        ,{ fImpl->btnPower, 2 }
        ,{ fImpl->btn_sin, 1 }
        ,{ fImpl->btn_arcsin, 1 }
        ,{ fImpl->btn_sinh, 1 }
        ,{ fImpl->btn_cos, 1 }
        ,{ fImpl->btn_arccos, 1 }
        ,{ fImpl->btn_cosh, 1 }
        ,{ fImpl->btn_tan, 1 }
        ,{ fImpl->btn_arctan, 1 }
        ,{ fImpl->btn_tanh, 1 }
        ,{ fImpl->btn_sec, 1 }
        ,{ fImpl->btn_arcsec, 1 }
        ,{ fImpl->btn_sech, 1 }
        ,{ fImpl->btn_csc, 1 }
        ,{ fImpl->btn_arccsc, 1 }
        ,{ fImpl->btn_csch, 1 }
        ,{ fImpl->btn_cot, 1 }
        ,{ fImpl->btn_arccot, 1 }
        ,{ fImpl->btn_coth, 1 }
    };

    fKeyMap =
    {
         { Qt::Key_0, [ this ]() { fImpl->btn_0->animateClick(); } }
        ,{ Qt::Key_1, [ this ]() { fImpl->btn_1->animateClick(); } }
        ,{ Qt::Key_2, [ this ]() { fImpl->btn_2->animateClick(); } }
        ,{ Qt::Key_3, [ this ]() { fImpl->btn_3->animateClick(); } }
        ,{ Qt::Key_4, [ this ]() { fImpl->btn_4->animateClick(); } }
        ,{ Qt::Key_5, [ this ]() { fImpl->btn_5->animateClick(); } }
        ,{ Qt::Key_6, [ this ]() { fImpl->btn_6->animateClick(); } }
        ,{ Qt::Key_7, [ this ]() { fImpl->btn_7->animateClick(); } }
        ,{ Qt::Key_8, [ this ]() { fImpl->btn_8->animateClick(); } }
        ,{ Qt::Key_9, [ this ]() { fImpl->btn_9->animateClick(); } }
        ,{ Qt::Key_Enter, [ this ]() { fImpl->btn_enter->animateClick(); } }
        ,{ Qt::Key_Return, [ this ]() { fImpl->btn_enter->animateClick(); } }
        ,{ Qt::Key_Plus, [ this ]() { fImpl->btn_plus->animateClick(); } }
        ,{ Qt::Key_Minus, [ this ]() { fImpl->btn_minus->animateClick(); } }
        ,{ Qt::Key_Asterisk, [ this ]() { fImpl->btn_mult->animateClick(); } }
        ,{ Qt::Key_Slash, [ this ]() { fImpl->btn_div->animateClick(); } }
        ,{ Qt::Key_Backspace, [ this ]() { fImpl->btn_BS->animateClick(); } }
        ,{ Qt::Key_Delete, [ this ]() { fImpl->btn_Del->animateClick(); } }
        ,{ Qt::Key_Period, [ this ]() { fImpl->btn_period->animateClick(); } }
    };
}
bool CCalculator::eventFilter( QObject* obj, QEvent* event )
{
    if ( ( obj == fImpl->values ) || ( obj == this ) )
    {
        if ( event->type() == QEvent::KeyPress )
        {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>( event );
            keyPressEvent( keyEvent );
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        // pass the event on to the parent class
        return QDialog::eventFilter( obj, event );
    }
}

void CCalculator::slotDataChanged()
{
    auto numRows = fModel->rowCount();
    for( auto && ii : fNumRowsPerFunctionMap )
    {
        ii.first->setEnabled( numRows >= ii.second );
    }
}

void CCalculator::keyPressEvent( QKeyEvent* event )
{
    auto key = event->key();
    auto pos = fKeyMap.find( key );
    if ( pos != fKeyMap.end() )
    {
        ((*pos).second)();
        return;
    }
    QDialog::keyPressEvent( event );
}

CCalculator::~CCalculator()
{
}

int CCalculator::numValues() const
{
    return fModel->rowCount();
}

template< typename T1 >
auto toNum( const QString& string, bool* aOK )
-> typename std::enable_if< std::is_floating_point< T1 >::value, T1 >::type
{
    return string.toDouble( aOK );
}

template< typename T1 >
auto toNum( const QString& string, bool * aOK )
-> typename std::enable_if< std::is_integral< T1 >::value, T1 >::type
{
    return string.toLongLong( aOK );
}


double CCalculator::getMean()
{
    auto values = getAllValues< double >( false );
    auto sum = std::accumulate( values.begin(), values.end(), 0.0 );
    double mean = sum / values.size();
    return mean;
}

template< typename T >
T CCalculator::getLastValue( bool popLast )
{
    QString lastValue;
    while ( lastValue.isEmpty() && ( fModel->rowCount() != 0 ) )
    {
        auto row = fModel->rowCount() - 1;
        QModelIndex mi = fModel->index( row );
        lastValue = mi.data().toString();
        if ( lastValue.isEmpty() )
        {
            fModel->removeRows( row, 1 );
            continue;
        }

        if ( popLast )
            fModel->removeRows( row, 1 );
    }

    if ( lastValue.isEmpty() )
        return 0;

    bool aOK = false;
    T currValue;
    if ( lastValue.toLower() == "nan" )
    {
        currValue = std::numeric_limits< T >::quiet_NaN();
        aOK = true;
    }
    else if ( lastValue.toLower() == "inf" )
    {
        currValue = std::numeric_limits< T >::infinity();
        aOK = true;
    }
    else
        currValue = toNum< T >( lastValue, &aOK );

    if ( !aOK )
    {
        return 0.0;
    }

    return currValue;
}

template< typename T >
std::vector<T> CCalculator::getAllValues( bool sorted )
{
    std::vector<T> retVal;
    while ( numValues() > 0 )
    {
        retVal.push_back( getLastValue< T >( true ) );
    }
    if ( sorted )
        std::sort( retVal.begin(), retVal.end() );
    return retVal;
}

void CCalculator::addValue( char value )
{
    QModelIndex mi;
    QString currValue;
    if ( fModel->rowCount() == 0 )
    {
        fModel->insertRows( fModel->rowCount(), 1 );
    }

    mi = fModel->index( fModel->rowCount() - 1 );
    currValue = fModel->data( mi, Qt::EditRole ).toString();

    currValue += value;
    fModel->setData( mi, currValue );
    fImpl->values->scrollTo( mi );

}

void CCalculator::addLastValue( double value )
{
    addLastValue( QString::number( value ) );
}

void CCalculator::addLastValue( int64_t value )
{
    addLastValue( QString::number( value ) );
}

void CCalculator::addLastValue( bool value )
{
    addLastValue( value ? tr( "Yes" ) : tr( "No" ) );
}

void CCalculator::addLastValue( const QString& newValue )
{
    fModel->insertRows( fModel->rowCount(), 1 );
    QModelIndex mi = fModel->index( fModel->rowCount() - 1 );
    fModel->setData( mi, newValue );
    fImpl->values->scrollTo( mi );
}

void CCalculator::btnEnterClicked()
{
    if ( fModel->rowCount() == 0 )
        return;

    QModelIndex mi = fModel->index( fModel->rowCount() - 1 );
    QString currValue = fModel->data( mi, Qt::EditRole ).toString();
    if ( currValue.isEmpty() )
        return;

    fModel->insertRows( fModel->rowCount(), 1 );
    fImpl->values->scrollTo( fModel->index( fModel->rowCount() - 1 ) );
}

void CCalculator::btnDelClicked()
{
    if ( numValues() == 0 )
        return;

    fModel->removeRows( fModel->rowCount() - 1, 1 );
}

void CCalculator::btnBSClicked()
{
    if ( numValues() == 0 )
        return;

    QModelIndex mi = fModel->index( fModel->rowCount() - 1 );
    QString currValue = fModel->data( mi, Qt::EditRole ).toString();
    if ( currValue.isEmpty() )
        return;
    currValue = currValue.left( currValue.length() - 1 );
    fModel->setData( mi, currValue );
}

void CCalculator::slotNegate()
{
    if ( numValues() == 0 )
        return;
    auto val = -getLastValue< double >( true );
    addLastValue( val );
}

void CCalculator::slotPercent()
{
    if ( numValues() == 0 )
        return;
    auto val = getLastValue< double >( true ) / 100.0;
    addLastValue( val );
}

void CCalculator::slotInvert()
{
    if ( numValues() == 0 )
        return;
    auto val = 1/getLastValue< double >( true );
    addLastValue( val );
}

void CCalculator::slotSquare()
{
    if ( numValues() == 0 )
        return;
    auto val = getLastValue< double >( true );
    val = val * val;
    addLastValue( val );
}

void CCalculator::slotSquareRoot()
{
    if ( numValues() == 0 )
        return;
    auto val = std::sqrt( getLastValue< double >( true ) );
    addLastValue( val );
}

void CCalculator::btnCClicked() // clear all
{
    fModel->setStringList( QStringList() );
}

void CCalculator::binaryOperatorClicked( char op )
{
    if ( fModel->rowCount() < 2 )
        return;

    auto val2 = getLastValue< double >( true );
    auto val1 = getLastValue< double >( true );

    auto pos = fOpMap.find( op );
    if ( pos == fOpMap.end() )
        return;
    auto newValue = (*pos).second( val1, val2 );
    addLastValue( newValue );
}

void CCalculator::reportPrime( std::list<int64_t>& factors, int64_t curr, bool incNum, int numShowPrime )
{
    if ( factors.size() == numShowPrime )
    {
        addLastValue( tr( "%1 is a prime number" ).arg( curr ) );
        return;
    }
    if ( !incNum )
        factors.pop_back();
    for ( auto&& ii : factors )
        addLastValue( ii );
}

void CCalculator::btnAverageClicked()
{
    if ( numValues() < 1 )
        return;

    auto mean = getMean();
    addLastValue( mean );
}

void CCalculator::btnMedianClicked()
{
    if ( numValues() < 1 )
        return;

    auto values = getAllValues< double >( true );

    if ( values.empty() )
        return;

    double retVal = 0;
    if ( ( values.size() % 2 ) == 0 ) // even
    {
        retVal = ( values[ values.size() / 2 ] + values[ 1 + ( values.size() / 2 ) ] ) / 2;
    }
    else
    {
        retVal = values[ values.size() / 2 ];
    }
    addLastValue( retVal );
}

void CCalculator::btnStdDeviationClicked()
{
    addLastValue( calcStdDeviation( false ).second );
}

void CCalculator::btnSampleStdDeviationClicked()
{
    addLastValue( calcStdDeviation( true ).second );
}

std::pair< double, double > CCalculator::calcStdDeviation( bool sample )
{
    if ( numValues() < 1 )
        return std::make_pair( 0.0, 0.0 );
    auto values = getAllValues< double >( false );
    auto sum = std::accumulate( values.begin(), values.end(), 0.0 );

    double mean = sum / values.size();

    double sumOfSquares = 0.0;
    std::for_each( values.begin(), values.end(), [ mean, &sumOfSquares ]( double curr ) { sumOfSquares += ( ( curr - mean ) * ( curr - mean ) ); } );

    sumOfSquares /= ( sample ? ( values.size() - 1 ) : values.size() );

    auto retVal = std::sqrt( sumOfSquares );
    return std::make_pair( mean, retVal );
}

void CCalculator::btnPctStdDeviationClicked()
{
    double stdDev = 0;
    double mean = 0;
    std::tie( mean, stdDev ) = calcStdDeviation( false );
    addLastValue( mean );
    addLastValue( stdDev );
    addLastValue( stdDev / mean );
}

void CCalculator::btnNarcissisticClicked()
{
    if ( numValues() < 1 )
        return;

    auto curr = getLastValue< int64_t >( false );

    bool aOK;
    bool isNarc = NUtils::isNarcissistic( curr, 10, aOK );
    if ( aOK )
    {
        addLastValue( isNarc );
    }
}

void CCalculator::btnFactorsClicked( bool incNum )
{
    if ( numValues() < 1 )
        return;
    auto curr = getLastValue< int64_t >( false );
    auto factors = NUtils::computeFactors( curr );

    reportPrime( factors, curr, incNum, 2 );
}

void CCalculator::btnPrimeFactorsClicked()
{
    if ( numValues() < 1 )
        return;
    auto curr = getLastValue< int64_t >( false );
    auto factors = NUtils::computePrimeFactors( curr );

    reportPrime( factors, curr, true, 1 );
}

void CCalculator::btnPerfectClicked()
{
    if ( numValues() < 1 )
        return;
    auto curr = getLastValue< int64_t >( false );

    addLastValue( NUtils::isPerfect( curr ).first );
}

void CCalculator::btnSemiPerfectClicked()
{
    if ( numValues() < 1 )
        return;
    auto curr = getLastValue< int64_t >( false );
    addLastValue( NUtils::isSemiPerfect( curr ).first );
}

void CCalculator::btnAbundantClicked()
{
    if ( numValues() < 1 )
        return;
    auto curr = getLastValue< int64_t >( false );
    addLastValue( NUtils::isAbundant( curr ).first );
}

void CCalculator::btnWeirdClicked()
{
    if ( numValues() < 1 )
        return;
    auto curr = getLastValue< int64_t >( false );
    auto isAbundant = NUtils::isAbundant( curr );
    if ( !isAbundant.first )
    {
        addLastValue( false );
        return;
    }

    std::vector< int64_t > factors( { isAbundant.second.begin(), isAbundant.second.end() } );
    addLastValue( !NUtils::isSemiPerfect( factors, factors.size(), curr ) );
}

void CCalculator::btnSublimeClicked()
{
    if ( numValues() < 1 )
        return;
    auto curr = getLastValue< int64_t >( false );
    auto sumOfFactors = NUtils::getSumOfFactors( curr, false );

    auto isNumFactorsPerfect = NUtils::isPerfect( sumOfFactors.second.size() ).first;
    auto isSumPerfect = NUtils::isPerfect( sumOfFactors.first ).first;
    addLastValue( isNumFactorsPerfect && isSumPerfect );
}

void CCalculator::btnFactorialClicked()
{
    if ( numValues() < 1 )
        return;
    auto curr = getLastValue< int64_t >( false );
    if ( curr <= 0 )
        return;

    int64_t retVal = 1;
    for( int64_t ii = curr; ii > 0; ii = ii - 1 )
        retVal = retVal * ii;
    addLastValue( retVal );
}

void CCalculator::btnFactorClicked()
{
    if ( numValues() < 2 )
        return;

    auto val2 = getLastValue< int64_t >( true );
    auto val1 = getLastValue< double >( true );

    double retVal = val1;

    for( int64_t ii = 1; ii < val2; ++ii )
    {
        retVal = retVal * val1;
    }
    addLastValue( retVal );
}

double CCalculator::pi() const
{
    return std::atan2( 0, -1 );
}

double CCalculator::degToRad( double degrees ) const
{
    return degrees * pi() / 180;
}

double CCalculator::radToDeg( double radians ) const
{
    return radians * 180.0 / pi();
}

void CCalculator::trigOperatorClicked( ETrigOperator op )
{
    if ( numValues() < 1 )
        return;

    auto value = getLastValue< double >( true );
    auto radians = degToRad( value );
    bool inRadians = fImpl->inRadians->isChecked();
    if ( inRadians )
        radians = value;

    double retVal = 0.0;
    switch ( op )
    {
        case ETrigOperator::eSin: retVal = std::sin( radians ); break;
        case ETrigOperator::eArcSin: retVal = std::asin( value ); if ( !inRadians ) retVal = radToDeg( retVal ); break;
        case ETrigOperator::eSinH: retVal = std::sinh( radians ); break;

        case ETrigOperator::eCos: retVal = std::cos( radians ); break;
        case ETrigOperator::eArcCos: retVal = std::acos( value ); if ( !inRadians ) retVal = radToDeg( retVal ); break;
        case ETrigOperator::eCosH: retVal = std::cosh( radians ); break;

        case ETrigOperator::eTan: retVal = std::tan( radians ); break;
        case ETrigOperator::eArcTan: retVal = std::atan( value ); if ( !inRadians ) retVal = radToDeg( retVal ); break;
        case ETrigOperator::eTanH: retVal = std::tanh( radians ); break;

        case ETrigOperator::eSec: retVal = 1.0/std::cos( radians ); break;
        case ETrigOperator::eArcSec: retVal = std::acos( 1/ value ); if ( !inRadians ) retVal = radToDeg( retVal ); break;
        case ETrigOperator::eSecH: retVal = 1.0/std::cosh( radians ); break;

        case ETrigOperator::eCsc: retVal = 1.0 / std::sin( radians ); break;
        case ETrigOperator::eArcCsc: retVal = std::asin( 1 / value ); if ( !inRadians ) retVal = radToDeg( retVal ); break;
        case ETrigOperator::eCscH: retVal = 1.0 / std::sinh( radians ); break;

        case ETrigOperator::eCot: retVal = 1.0 / std::tan( radians ); break;
        case ETrigOperator::eArcCot: retVal = std::atan( 1 / value ); if ( !inRadians ) retVal = radToDeg( retVal ); break;
        case ETrigOperator::eCotH: retVal = 1.0 / std::tanh( radians ); break;
        default:
            break;
    }
    addLastValue( retVal );
}
