#include "Calculator.h"
#include "ui_Calculator.h"

#include <QStringListModel>
#include <QKeyEvent>

CCalculator::CCalculator(QWidget *parent)
    : QDialog(parent)
{
	ui = new Ui::CCalculator();
	ui->setupUi(this);

    fModel = new QStringListModel( this );
    ui->values->setModel( fModel );

    connect( ui->btn_0, SIGNAL( clicked() ), this, SLOT( btn0Clicked() ) );
    connect( ui->btn_1, SIGNAL( clicked() ), this, SLOT( btn1Clicked() ) );
    connect( ui->btn_2, SIGNAL( clicked() ), this, SLOT( btn2Clicked() ) );
    connect( ui->btn_3, SIGNAL( clicked() ), this, SLOT( btn3Clicked() ) );
    connect( ui->btn_4, SIGNAL( clicked() ), this, SLOT( btn4Clicked() ) );
    connect( ui->btn_5, SIGNAL( clicked() ), this, SLOT( btn5Clicked() ) );
    connect( ui->btn_6, SIGNAL( clicked() ), this, SLOT( btn6Clicked() ) );
    connect( ui->btn_7, SIGNAL( clicked() ), this, SLOT( btn7Clicked() ) );
    connect( ui->btn_8, SIGNAL( clicked() ), this, SLOT( btn8Clicked() ) );
    connect( ui->btn_9, SIGNAL( clicked() ), this, SLOT( btn9Clicked() ) );
    connect( ui->btn_enter, SIGNAL( clicked() ), this, SLOT( btnEnterClicked() ) );
    connect( ui->btn_period, SIGNAL( clicked() ), this, SLOT( btnPeriodClicked() ) );
    connect( ui->btn_plus, SIGNAL( clicked() ), this, SLOT( btnPlusClicked() ) );

    connect( ui->btnMinus, SIGNAL( clicked() ), this, SLOT( btnMinusClicked() ) );

    connect( ui->btn_CA, SIGNAL( clicked() ), this, SLOT( btnCAClicked() ) );
    connect( ui->btn_Average, SIGNAL( clicked() ), this, SLOT( btnAverageClicked() ) );

    ui->values->installEventFilter( this );
    setFocus( Qt::MouseFocusReason );
}

bool CCalculator::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->values) 
    {
        if (event->type() == QEvent::KeyPress) 
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            keyPressEvent( keyEvent );
            return true;
        } 
        else 
        {
            return false;
        }
    } else 
    {
        // pass the event on to the parent class
        return QDialog::eventFilter(obj, event);
    }
}

void CCalculator::keyPressEvent( QKeyEvent * event )
{
     switch (event->key()) 
     {
     case Qt::Key_0:
         btn0Clicked();
         break;
     case Qt::Key_1:
         btn1Clicked();
         break;
     case Qt::Key_2:
         btn2Clicked();
         break;
     case Qt::Key_3:
         btn3Clicked();
         break;
     case Qt::Key_4:
         btn4Clicked();
         break;
     case Qt::Key_5:
         btn5Clicked();
         break;
     case Qt::Key_6:
         btn6Clicked();
         break;
     case Qt::Key_7:
         btn7Clicked();
         break;
     case Qt::Key_8:
         btn8Clicked();
         break;
     case Qt::Key_9:
         btn9Clicked();
         break;
     case Qt::Key_Enter:
     case Qt::Key_Return:
         btnEnterClicked();
         break;

     case Qt::Key_Backspace:
         btnBSClicked();  
         break;
     case Qt::Key_Delete:
         btnDeleteClicked();  
         break;
     default:
         QDialog::keyPressEvent(event);
     }
}

CCalculator::~CCalculator()
{
	delete ui;
}

void CCalculator::btn0Clicked()
{
    addValue( '0' );
}

void CCalculator::btn1Clicked()
{
    addValue( '1' );
}

void CCalculator::btn2Clicked()
{
    addValue( '2' );
}

void CCalculator::btn3Clicked()
{
    addValue( '3' );
}

void CCalculator::btn4Clicked()
{
    addValue( '4' );
}

void CCalculator::btn5Clicked()
{
    addValue( '5' );
}

void CCalculator::btn6Clicked()
{
    addValue( '6' );
}

void CCalculator::btn7Clicked()
{
    addValue( '7' );
}

void CCalculator::btn8Clicked()
{
    addValue( '8' );
}

void CCalculator::btn9Clicked()
{
    addValue( '9' );
}

void CCalculator::btnPeriodClicked()
{
    addValue( '.' );
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
        fModel->removeRows( fModel->rowCount()-1, 1 );
        mi = fModel->index( fModel->rowCount() - 1 );
    }

    bool aOK = false;
    double currValue = mi.data().toDouble( &aOK );
    if ( !aOK )
    {
        if ( popLast )
            fModel->removeRows( fModel->rowCount()-1, 1 );
        return 0.0;
    }

    if ( popLast )
        fModel->removeRows( fModel->rowCount()-1, 1 );
    return currValue;
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

void CCalculator::btnDeleteClicked()
{
    if ( numValues() == 0 )
        return;

    QModelIndex mi = fModel->index( fModel->rowCount() - 1 );
    fModel->setData( mi, "" ); 
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
    fModel->setData( mi , currValue );
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

int CCalculator::numValues() const
{
    return fModel->rowCount();
}

void CCalculator::btnPlusClicked()
{
    if ( fModel->rowCount() < 2 )
        return;

    double val1 = getLastValue( true );
    double val2 = getLastValue( true );
    double newValue = val1 + val2;
    addLastValue( newValue );
}

void CCalculator::btnMinusClicked ()
{
    if ( numValues() < 2 )
        return;
    double val2 = getLastValue( true ) ;
    double val1 = getLastValue ( true ) ;
    double newValue = val1 - val2 ;
    addLastValue (newValue) ;
}

void CCalculator::btnCAClicked()
{
    fModel->setStringList( QStringList() );
}

void CCalculator::btnAverageClicked()
{
    if ( numValues () < 1 )
        return;

    int num= 0;
    double total = 0;
    while ( numValues() > 0)
    {
        num = num + 1;
        double curr = getLastValue ( true);
        total += curr;
    }
    double newValue = total / num;
    addLastValue ( newValue);
}
