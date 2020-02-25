#include "Calculator.h"
#include "ui_Calculator.h"

#include <QStringListModel>
#include <QKeyEvent>
#include <unordered_map>

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
    setWindowFlags( windowFlags() & ~Qt::WindowContextHelpButtonHint );

    fModel = new CStringListModel( this );
    fImpl->values->setModel( fModel );

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

    (void)connect( fImpl->btn_enter, &QToolButton::clicked, this, &CCalculator::btnEnterClicked );
    (void)connect( fImpl->btn_C, &QToolButton::clicked, this, &CCalculator::btnCClicked );
    (void)connect( fImpl->btn_Del, &QToolButton::clicked, this, &CCalculator::btnDelClicked );
    (void)connect( fImpl->btn_BS, &QToolButton::clicked, this, &CCalculator::btnBSClicked );

    (void)connect( fImpl->btn_plus, &QToolButton::clicked, this, [this](){ binaryOperatorClicked( '+' ); } );
    (void)connect( fImpl->btn_minus, &QToolButton::clicked, this, [ this ]() { binaryOperatorClicked( '-' ); } );
    (void)connect( fImpl->btn_mult, &QToolButton::clicked, this, [ this ]() { binaryOperatorClicked( '*' ); } );
    (void)connect( fImpl->btn_div, &QToolButton::clicked, this, [ this ]() { binaryOperatorClicked( '/' ); } );

    (void)connect( fImpl->btn_Average, &QToolButton::clicked, this, &CCalculator::btnAverageClicked );

    fImpl->values->installEventFilter( this );
    setFocus( Qt::MouseFocusReason );
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

void CCalculator::keyPressEvent( QKeyEvent* event )
{
    using TKeyPressedFunction = std::function< void(void) >;
    static std::unordered_map< int, TKeyPressedFunction > sKeyMap = 
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
    };

    auto pos = sKeyMap.find( event->key() );
    if ( pos != sKeyMap.end() )
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

double CCalculator::getLastValue( bool popLast )
{
    if ( fModel->rowCount() == 0 )
        return 0.0;

    QModelIndex mi = fModel->index( fModel->rowCount() - 1 );
    if ( mi.data().toString().isEmpty() )
    {
        if ( !popLast )
            return 0.0;
        fModel->removeRows( fModel->rowCount() - 1, 1 );
        mi = fModel->index( fModel->rowCount() - 1 );
    }

    bool aOK = false;
    double currValue = mi.data().toDouble( &aOK );
    if ( !aOK )
    {
        if ( popLast )
            fModel->removeRows( fModel->rowCount() - 1, 1 );
        return 0.0;
    }

    if ( popLast )
        fModel->removeRows( fModel->rowCount() - 1, 1 );
    return currValue;
}


void CCalculator::addValue( char value )
{
    QModelIndex mi;
    QString currValue;
    if ( fModel->rowCount() )
    {
        mi = fModel->index( fModel->rowCount() - 1 );
        currValue = fModel->data( mi, Qt::EditRole ).toString();
    }
    else
    {
        fModel->insertRows( fModel->rowCount(), 1 );
        mi = fModel->index( 0, 0 );
    }
    currValue += value;
    fModel->setData( mi, currValue );
}

void CCalculator::addLastValue( double value )
{
    QString newValue = QString::number( value );
    fModel->insertRows( fModel->rowCount(), 1 );
    QModelIndex mi = fModel->index( fModel->rowCount() - 1 );
    fModel->setData( mi, newValue );
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

void CCalculator::btnCClicked() // clear all
{
    fModel->setStringList( QStringList() );
}

void CCalculator::binaryOperatorClicked( char op )
{
    using TBinaryOpFunc = std::function< double( double, double ) >;
    static std::unordered_map< char, TBinaryOpFunc > sOpMap =
    {
          { '+', [ this ]( double lhs, double rhs ) { return lhs + rhs; } }
         ,{ '-', [ this ]( double lhs, double rhs ) { return lhs - rhs; } }
         ,{ '*', [ this ]( double lhs, double rhs ) { return lhs * rhs; } }
         ,{ '/', [ this ]( double lhs, double rhs ) { return lhs / rhs; } }
    };

    if ( fModel->rowCount() < 2 )
        return;

    auto val2 = getLastValue( true );
    auto val1 = getLastValue( true );

    auto pos = sOpMap.find( op );
    if ( pos == sOpMap.end() )
        return;
    auto newValue = (*pos).second( val1, val2 );
    addLastValue( newValue );
}

void CCalculator::btnAverageClicked()
{
    if ( numValues() < 1 )
        return;

    int num = 0;
    double total = 0;
    while ( numValues() > 0 )
    {
        num = num + 1;
        double curr = getLastValue( true );
        total += curr;
    }
    double newValue = total / num;
    addLastValue( newValue );
}
