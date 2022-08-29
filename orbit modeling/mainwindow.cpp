#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    formatPlot();
    addLayout();

    //world setup
    world = new World();

    timer= new QTimer;
    connect(timer, SIGNAL(timeout()),this, SLOT(time_changed_slot()));
    connect(timer, SIGNAL(timeout()), this, SLOT(checkErrorButton_clicked()));

    //threads
    thread_1 = new QThread(this);

    connect(this, SIGNAL(destroyed()), thread_1, SLOT(quit()));

    connect(this, SIGNAL(startUpdate()), world, SLOT(runUpdate()));

    //planet
    createImageXY(world->planet)->attach(ui->plot);
    createImageXZ(world->planet)->attach(ui->plot1);
    enableMagnifier();
    enableMovingOnPlot();
    enablePicker();;
    connect(picker, SIGNAL(appended(QPoint)),
    SLOT(click_on_canvas(QPoint)));
    world->moveToThread(thread_1);
    thread_1->start();
}

void MainWindow::time_changed_slot()
{
    emit startUpdate();
    replot();
}

void MainWindow::attach_replotXY()
{
    for(int j=0;j<world->clusterVector.size();j++)
    {
        if(!world->clusterVector[j]->isTle)
        {
            for(int i=0;i<world->clusterVector[j]->pointVector.size();i++)
            {
                if(!(pow(world->clusterVector[j]->pointVector[i]->currCoord[0],2)+
                        pow(world->clusterVector[j]->pointVector[i]->currCoord[1],2)<pow(world->planet->radius,2)
                        && world->clusterVector[j]->pointVector[i]->currCoord[2]<0))
                {
                    tempImageVectorXY << createImageXY(world->clusterVector[j]->pointVector[i]);
                    tempImageVectorXY.last()->attach(ui->plot);
                }
            }
        }
        else
            for(int i=0;i<world->clusterVector[j]->pointVector.size();i++)
            {
                double timeEpochMinutes=world->timeEpoch/60;
                cEciTime eci=world->clusterVector[j]->tlePointVector[i].PositionEci(timeEpochMinutes);

                if(!(pow(eci.Position().m_x,2)+
                        pow(eci.Position().m_y,2)<pow(world->planet->radius,2)
                        && eci.Position().m_z<0))
                {
                    tempImageVectorXY << createImageXY(world->clusterVector[j]->pointVector[i]);
                    tempImageVectorXY.last()->attach(ui->plot);
                }
            }
    }

    ui->plot->replot();
    ui->plot->repaint();
}

void MainWindow::attach_replotXY_NoTLE()
{
    for(int j=0;j<world->clusterNoTLE.size();j++)
    {
        for(int i=0;i<world->clusterNoTLE[j]->pointVector.size();i++)
        {
            if(!(pow(world->clusterNoTLE[j]->pointVector[i]->currCoord[0],2)+
                    pow(world->clusterNoTLE[j]->pointVector[i]->currCoord[1],2)<pow(world->planet->radius,2)
                    && world->clusterNoTLE[j]->pointVector[i]->currCoord[2]<0))
            {
                tempImageVectorXY << createImageXY(world->clusterNoTLE[j]->pointVector[i]);
                tempImageVectorXY.last()->attach(ui->plot);
            }
        }
    }
}

void MainWindow::attach_replotXY_TLE()
{
    for(int j=0;j<world->clusterTLE.size();j++)
    {
        for(int i=0;i<world->clusterTLE[j]->pointVector.size();i++)
        {
            double timeEpochMinutes=world->timeEpoch/60;
            cEciTime eci=world->clusterTLE[j]->tlePointVector[i].PositionEci(timeEpochMinutes);

            if(!(pow(eci.Position().m_x,2)+
                    pow(eci.Position().m_y,2)<pow(world->planet->radius,2)
                    && eci.Position().m_z<0))
            {
                tempImageVectorXY << createImageXY(world->clusterTLE[j]->pointVector[i]);
                tempImageVectorXY.last()->attach(ui->plot);
            }
        }
    }
}

void MainWindow::attach_replotXZ()
{
    for(int j=0;j<world->clusterVector.size();j++)
    {
        if(!world->clusterVector[j]->isTle)
        {
            for(int i=0;i<world->clusterVector[j]->pointVector.size();i++)
            {
                if(!(pow(world->clusterVector[j]->pointVector[i]->currCoord[0],2)+
                        pow(world->clusterVector[j]->pointVector[i]->currCoord[2],2)<pow(world->planet->radius,2)
                        && world->clusterVector[j]->pointVector[i]->currCoord[1]>0))
                {
                    tempImageVectorXZ << createImageXZ(world->clusterVector[j]->pointVector[i]);
                    tempImageVectorXZ.last()->attach(ui->plot1);
                }
            }
        }
        else
            for(int i=0;i<world->clusterVector[j]->pointVector.size();i++)
            {
                double timeEpochMinutes=world->timeEpoch/60;
                cEciTime eci=world->clusterVector[j]->tlePointVector[i].PositionEci(timeEpochMinutes);

                if(!(pow(eci.Position().m_x,2)+
                        pow(eci.Position().m_z,2)<pow(world->planet->radius,2)
                        && eci.Position().m_y>0))
                {
                    tempImageVectorXZ << createImageXZ(world->clusterVector[j]->pointVector[i]);
                    tempImageVectorXZ.last()->attach(ui->plot1);
                }
            }
    }

    ui->plot1->replot();
    ui->plot1->repaint();
}

void MainWindow::attach_replotXZ_NoTLE()
{
    for(int j=0;j<world->clusterNoTLE.size();j++)
    {
        for(int i=0;i<world->clusterNoTLE[j]->pointVector.size();i++)
        {
            if(!(pow(world->clusterNoTLE[j]->pointVector[i]->currCoord[0],2)+
                    pow(world->clusterNoTLE[j]->pointVector[i]->currCoord[2],2)<pow(world->planet->radius,2)
                    && world->clusterNoTLE[j]->pointVector[i]->currCoord[1]>0))
            {
                tempImageVectorXZ << createImageXZ(world->clusterNoTLE[j]->pointVector[i]);
                tempImageVectorXZ.last()->attach(ui->plot1);
            }
        }
    }
}

void MainWindow::attach_replotXZ_TLE()
{
    for(int j=0;j<world->clusterTLE.size();j++)
    {
        for(int i=0;i<world->clusterTLE[j]->pointVector.size();i++)
        {
            double timeEpochMinutes=world->timeEpoch/60;
            cEciTime eci=world->clusterTLE[j]->tlePointVector[i].PositionEci(timeEpochMinutes);

            if(!(pow(eci.Position().m_x,2)+
                    pow(eci.Position().m_z,2)<pow(world->planet->radius,2)
                    && eci.Position().m_y>0))
            {
                tempImageVectorXZ << createImageXZ(world->clusterTLE[j]->pointVector[i]);
                tempImageVectorXZ.last()->attach(ui->plot1);
            }
        }
    }
}

void MainWindow::replot()
{
    detachXY();
    detachXZ();

    attach_replotXY();
    ui->plot->replot();
    ui->plot->repaint();

    attach_replotXZ();

    ui->plot1->replot();
    ui->plot1->repaint();
}

void MainWindow::detachXY()
{
    for(int i=0;i<tempImageVectorXY.size();i++)
    {
        tempImageVectorXY[i]->detach();
    }
    tempImageVectorXY.clear();
}

void MainWindow::detachXZ()
{
    for(int i=0;i<tempImageVectorXZ.size();i++)
    {
        tempImageVectorXZ[i]->detach();
    }
    tempImageVectorXZ.clear();
}

void MainWindow::startStopButton_clicked()
{
    if(isStart)
    {
        timer->start(10);
        ui->startButton->setText("Стоп");
        isStart=false;
    }
    else
    {
        timer->stop();
        ui->startButton->setText("Старт");
        isStart=true;
    }
}

void MainWindow::resetButton_clicked()
{
    timer->stop();
    world->reset();
    ui->startButton->setText("Старт");
    isStart=true;
    detachXY();
    attach_replotXY();

    detachXZ();
    attach_replotXZ();
}

void MainWindow::deleteLastCluster_clicked()
{
    detachXY();
    detachXZ();

    world->clusterVector.pop_back();

    attach_replotXY();
    attach_replotXZ();


}

void MainWindow::checkErrorButton_clicked()
{
    double timeEpochMinutes=world->timeEpoch/60;

    double sum=0;
    double tempsum;
    for(unsigned int i=0;i<world->clusterVector.size();i++)
    {
        for(unsigned int j=0;j<world->clusterVector[i]->pointVector.size();j++)
        {
            cEciTime eci=world->clusterVector[i]->tlePointVector[j].PositionEci(timeEpochMinutes);

            double x = world->clusterVector[i]->pointVector[j]->currCoord[0];
            double y = world->clusterVector[i]->pointVector[j]->currCoord[1];
            double z = world->clusterVector[i]->pointVector[j]->currCoord[2];
            tempsum=sqrt(pow(x - eci.Position().m_x, 2) + pow(y - eci.Position().m_y,2) + pow(z - eci.Position().m_z,2));

            sum+=tempsum;
        }
    }

    ui->checkErrorLabel->setText("norm: " + QString::number(sum));
}

void MainWindow::click_on_canvas( const QPoint &pos )
{

    double x = ui->plot->invTransform(QwtPlot::xBottom, pos.x());
    double y = ui->plot->invTransform(QwtPlot::yLeft, pos.y());

    QString info = "x= " + QString::number(x) +
    "; y = " + QString::number(y);

    statusBar()->showMessage(info);
}


void MainWindow::formatPlot()
{

    ui->plot->replot();
    ui->plot1->replot();

    QwtPlotRescaler *rescaler = new QwtPlotRescaler(ui->plot->canvas());
    QwtPlotRescaler *rescaler1 = new QwtPlotRescaler(ui->plot1->canvas());

    rescaler->setReferenceAxis(QwtPlot::xBottom);
    rescaler->setAspectRatio(QwtPlot::yLeft, 1.0);
    rescaler->setRescalePolicy(QwtPlotRescaler::Expanding);
    rescaler->setEnabled(true);
    rescaler->rescale();

    rescaler1->setReferenceAxis(QwtPlot::xBottom);
    rescaler1->setAspectRatio(QwtPlot::yLeft, 1.0);
    rescaler1->setRescalePolicy(QwtPlotRescaler::Expanding);
    rescaler1->setEnabled(true);
    rescaler1->rescale();

    ui->plot->plotLayout()->setAlignCanvasToScales(true);

    ui->plot1->plotLayout()->setAlignCanvasToScales(true);

    ui->plot->setCanvasBackground( Qt::white );

    ui->plot1->setCanvasBackground( Qt::white );

    ui->plot->setAxisTitle(QwtPlot::yLeft, "Y");
    ui->plot->setAxisTitle(QwtPlot::xBottom, "X");

    ui->plot1->setAxisTitle(QwtPlot::yLeft, "Z");
    ui->plot1->setAxisTitle(QwtPlot::xBottom, "X");
}

void MainWindow::addPlotGrid()
{
    grid = new QwtPlotGrid();
    grid->setMajorPen(QPen( Qt::gray, 1 ));
    grid->attach( ui->plot );

    grid1 = new QwtPlotGrid();
    grid1->setMajorPen(QPen( Qt::gray, 1 ));
    grid1->attach( ui->plot1 );
}


void MainWindow::enableMagnifier()
{
    magnifier = new QwtPlotMagnifier(ui->plot->canvas());
    magnifier->setMouseButton(Qt::MiddleButton);
    magnifier->rescale(30);
    magnifier1 = new QwtPlotMagnifier(ui->plot1->canvas());
    magnifier1->setMouseButton(Qt::MiddleButton);
    magnifier1->rescale(30);
}

void MainWindow::enableMovingOnPlot()
{
    panner = new QwtPlotPanner( ui->plot->canvas() );
    panner->setMouseButton( Qt::RightButton );
    panner->moveCanvas(0, -20);

    panner1 = new QwtPlotPanner( ui->plot1->canvas() );

    panner1->setMouseButton( Qt::RightButton );
    panner1->moveCanvas(0, -20);
}

void MainWindow::enablePicker()
{
    picker = new QwtPlotPicker(
                QwtPlot::xBottom, QwtPlot::yLeft,
                QwtPlotPicker::CrossRubberBand,
                QwtPicker::AlwaysOn,
                ui->plot->canvas() );

    picker->setRubberBandPen( QColor( Qt::gray ) );

    picker->setTrackerPen( QColor( Qt::white ) );

    picker->setStateMachine( new QwtPickerDragPointMachine() );


    picker1 = new QwtPlotPicker(
                QwtPlot::xBottom, QwtPlot::yLeft,
                QwtPlotPicker::CrossRubberBand,
                QwtPicker::AlwaysOn,
                ui->plot1->canvas() );

    picker1->setRubberBandPen( QColor( Qt::gray ) );
    picker1->setTrackerPen( QColor( Qt::white ) );
    picker1->setStateMachine( new QwtPickerDragPointMachine() );
}

void MainWindow::addStatusBar()
{
    #ifndef QT_NO_STATUSBAR
    ( void )statusBar();
    #endif
}

QwtPlotShapeItem* MainWindow::createImageXY(Point *a)
{
    QPainterPath path;
    path.addEllipse(a->rectXY);
    QwtPlotShapeItem *aImage= new QwtPlotShapeItem;

    QPen pen( a->color, 2 );
    pen.setJoinStyle( Qt::MiterJoin );
    aImage->setPen( pen );


    aImage->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    aImage->setShape( path );
    aImage->setBrush( a->color );
    return aImage;
}

QwtPlotShapeItem* MainWindow::createImageXZ(Point *a)
{
    QPainterPath path;
    path.addEllipse(a->rectXZ);
    QwtPlotShapeItem *aImage= new QwtPlotShapeItem;

    QPen pen( a->color, 2 );
    pen.setJoinStyle( Qt::MiterJoin );
    aImage->setPen( pen );


    aImage->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    aImage->setShape( path );
    aImage->setBrush( a->color );
    return aImage;
}

void MainWindow::cancelConfigClusterButton_clicked()
{
    if(!tempTlePointVector.empty())
    {
        world->addCluster(new Cluster(tempTlePointVector, generate_color(),world->planet,world->G,world->timeMultiplier, false));
        tempTlePointVector.clear();
    }
    else
    {
        if(!ui->tleNameEdit->text().isEmpty() && str1Edit->text().isEmpty() && str2Edit->text().isEmpty())
        {
            world->addCluster(new Cluster(readTleFile(ui->tleNameEdit->text()),
                                          generate_color(),world->planet,world->G,world->timeMultiplier, false));
            world->addCluster(new Cluster(readTleFile(ui->tleNameEdit->text()),
                                          generate_color(),world->planet,world->G,world->timeMultiplier, true));

        }
    }

    addToClusterButton->setEnabled(false);
    cancelConfigClusterButton->setEnabled(false);
    configClusterButton->setEnabled(true);
    clusterStatus->setText("----------");
    labelCounter=0;


    world->reset();

    detachXY();
    detachXZ();
    attach_replotXY();
    attach_replotXZ();
}

void MainWindow::addLayout()
{

    QHBoxLayout *mainLayout=new QHBoxLayout();
    //QGroupBox *mainGroupBox=new QGroupBox();


    //QScrollArea *mainScrollArea = new QScrollArea();


    //side bar
    QVBoxLayout *sideBarLayout=new QVBoxLayout();
    QGroupBox *sideBarGroupBox=new QGroupBox();

    //buttons
    QHBoxLayout *buttonLayout=new QHBoxLayout();
    QGroupBox *buttonGroupBox=new QGroupBox();

    startButton=new QPushButton(tr("start"));
    resetButton=new QPushButton(tr("reset"));


    addToClusterButton=new QPushButton(tr("add point to cluster"));
    configClusterButton=new QPushButton(tr("config cluster"));
    cancelConfigClusterButton=new QPushButton(tr("add cluster"));
    labelCounter=0;

    clusterStatus=new QLabel;

    clusterStatus->setText("----------");

    addToClusterButton->setEnabled(false);
    cancelConfigClusterButton->setEnabled(false);

    configClusterButton->setEnabled(true);

    deleteLastClusterButton=new QPushButton(tr("delete last cluster"));

    checkErrorButton=new QPushButton("check error");
    checkErrorLabel=new QLabel;

    connect(ui->startButton,SIGNAL(clicked()),this,SLOT(startStopButton_clicked()));
    connect(ui->resetButton,SIGNAL(clicked()),this,SLOT(resetButton_clicked()));

    connect(ui->cancelConfigClusterButton,SIGNAL(clicked()),this,SLOT(cancelConfigClusterButton_clicked()));
    connect(ui->deleteLastClusterButton,SIGNAL(clicked()),this,SLOT(deleteLastCluster_clicked()));

    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(resetButton);

    buttonGroupBox->setLayout(buttonLayout);

    sideBarLayout->addWidget(buttonGroupBox);
    sideBarLayout->addWidget(configClusterButton);
    sideBarLayout->addWidget(cancelConfigClusterButton);

    QFormLayout *tleInputLayout = new QFormLayout();
    QGroupBox *tleInputGroupBox = new QGroupBox();

    tleNameEdit = new QLineEdit;
    str1Edit = new QLineEdit;
    str2Edit = new QLineEdit;

    tleInputLayout->addRow(tr("Name"), tleNameEdit);
    tleInputLayout->addRow(tr("str1"), str1Edit);
    tleInputLayout->addRow(tr("str2"), str2Edit);

    tleInputGroupBox->setLayout(tleInputLayout);

    sideBarLayout->addWidget(tleInputGroupBox);
    sideBarLayout->addWidget(addToClusterButton);
    sideBarLayout->addWidget(clusterStatus);
    sideBarLayout->addWidget(deleteLastClusterButton);
    sideBarLayout->addWidget(checkErrorButton);
    sideBarLayout->addWidget(checkErrorLabel);

    sideBarGroupBox->setLayout(sideBarLayout);


    QVBoxLayout *widgetLayout=new QVBoxLayout();
    QGroupBox *widgetGroupBox=new QGroupBox();

    widgetGroupBox->setLayout(widgetLayout);

    mainLayout->addWidget(widgetGroupBox,4);
    mainLayout->addWidget(sideBarGroupBox,1);

    mainLayout->setAlignment(sideBarGroupBox,Qt::AlignTop);
}

QVector<cSatellite> MainWindow::readTleFile(QString file_name)
{
    QFile myfile("C:/project/orbit modeling/data/"+file_name);
    QTextStream in(&myfile);

    if(!myfile.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", myfile.errorString());
    }

    QVector<cSatellite> out;

    int linecount=0;
    while (!myfile.atEnd())
    {
        linecount++;
        QString str1=myfile.readLine();
        QString str2=myfile.readLine();
        QString str3=myfile.readLine();

        out << cSatellite(cTle(str1.toStdString(),str2.toStdString(),str3.toStdString()));
    }
    cout << linecount << endl;
    myfile.close();
    return out;
}

QColor MainWindow::generate_color()
{
    int red=0,green=0,blue=0;

    if(color_int%2==0)
    {
        red=255*(color_int+1)*77%256;
        if (red<126)
            red += 100;
        green=0;
        blue=0;
    }
    if(color_int%2==1)
    {
        red=0;
        green=255*(color_int*77)%256;
        if (green<126)
            green += 100;
        blue=0;
    }

    color_int+=1;

    return QColor(red,green,blue);
}

MainWindow::~MainWindow()
{
    delete ui;
}
