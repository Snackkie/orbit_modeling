#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <world.h>

#include <QString>
#include <QTimer>
#include <QMainWindow>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>


#include <QwtPlotShapeItem>
#include <QwtPlotRescaler>

#include <qwt_plot_layout.h>

#include <QBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>

#include <QComboBox>

#include <QScrollArea>

#include "coreLib.h"
#include "orbitLib.h"
#include <fstream>

#include <QMessageBox>

#include <QtConcurrent>
#include <QFuture>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QwtPlot *plot;
    QwtPlot *plot1;
    void formatPlot();

    QwtPlotGrid *grid, *grid1;
    void addPlotGrid();

    QVector<QwtPlotShapeItem*> pointImageVector;
    void attach_replotXY();
    void attach_replotXZ();
    void detachXY();
    void detachXZ();
    void replot();

    QVector<QwtPlotShapeItem*> tempImageVectorXY,tempImageVectorXZ;

    QwtPlotShapeItem* createImageXY(Point *a);
    QwtPlotShapeItem* createImageXZ(Point *a);

    World *world;
    QPolygonF points;

    QTimer *timer;

    QwtPlotPanner *panner, *panner1;
    void enableMovingOnPlot();

    QwtPlotMagnifier *magnifier, *magnifier1;
    void enableMagnifier();

    QwtPlotPicker *picker, *picker1;
    void enablePicker();

    void addStatusBar();

    bool isStart;

    //layout
    QHBoxLayout *mainLayout;
    void addLayout();

    //input
    QLineEdit *tleNameEdit;
    QLineEdit *str1Edit;
    QLineEdit *str2Edit;

    QLabel *clusterStatus;
    QVector<cSatellite> tempTlePointVector;

    int labelCounter;

    //buttons
    QPushButton *startButton;
    QPushButton *resetButton;
    QPushButton *addToClusterButton;
    QPushButton *configClusterButton;
    QPushButton *deleteLastClusterButton;
    QPushButton *cancelConfigClusterButton;

    QPushButton *checkErrorButton;
    QLabel *checkErrorLabel;


    QVector<cSatellite> readTleFile(QString file_name);


    int color_int=0;
    QColor generate_color();

    //threads
    QThread *thread_1;
    QThread *thread_2;
    QThread *thread_3;
    QThread *thread_4;
    QThread *thread_5;

private Q_SLOTS:
    void click_on_canvas( const QPoint &pos );
    void time_changed_slot();
    void startStopButton_clicked();
    void resetButton_clicked();
    void cancelConfigClusterButton_clicked();
    void deleteLastCluster_clicked();

    void checkErrorButton_clicked();

    void attach_replotXY_NoTLE();
    void attach_replotXY_TLE();
    void attach_replotXZ_NoTLE();
    void attach_replotXZ_TLE();



    //threads
signals:
    void startUpdate();
};
#endif // MAINWINDOW_H

















