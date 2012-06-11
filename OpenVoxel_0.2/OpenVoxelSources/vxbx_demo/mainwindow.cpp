#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "glframe.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    GLFrame = new QGLFrame();      // create our subclassed GLWidget
    GLFrame->setMouseTracking(true);
    ui->GLContainer->addWidget(GLFrame);

    QObject::connect(GLFrame, SIGNAL(showRePosition(QString)), ui->edit_re, SLOT(setText(QString)));
    QObject::connect(GLFrame, SIGNAL(showImPosition(QString)), ui->edit_im, SLOT(setText(QString)));
    QObject::connect(GLFrame, SIGNAL(showZoomValue(QString)), ui->edit_zoom, SLOT(setText(QString)));
    QObject::connect(GLFrame, SIGNAL(showIterationsValue(QString)), ui->edit_iterations, SLOT(setText(QString)));
    QObject::connect(&(GLFrame->RenderThread), SIGNAL(showFPS(QString)), ui->lab_fps, SLOT(setText(QString)));

    GLFrame->initRenderThread();    // start rendering
}

MainWindow::~MainWindow()
{
    delete GLFrame;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *evt)
{
    GLFrame->stopRenderThread();    // stop the thread befor exiting
    QMainWindow::closeEvent(evt);
}


void MainWindow::on_check_fullscreen_toggled(bool checked)
{
    if(checked)
        this->showFullScreen();
    else this->showNormal();
}

void MainWindow::on_btn_reloadshader_clicked()
{
    GLFrame->RenderThread.ReloadShader();
    GLFrame->RenderThread.Redraw();
}

void MainWindow::on_rbtn_float_toggled(bool checked)
{
    if(checked)
        {
        GLFrame->RenderThread.setRenderMode(0);
        GLFrame->RenderThread.Redraw();
        }
}

void MainWindow::on_check_Benchmark_toggled(bool checked)
{
    GLFrame->RenderThread.setBenchmark(checked);
    ui->lab_fps->setText("-");
    GLFrame->RenderThread.Redraw();
}

void MainWindow::on_edit_iterations_returnPressed()
{
    GLFrame->RenderThread.setIterations(ui->edit_iterations->text().toInt());
    GLFrame->RenderThread.Redraw();
}

void MainWindow::on_edit_zoom_returnPressed()
{
    GLFrame->RenderThread.setZoom(ui->edit_zoom->text().toDouble());
    GLFrame->RenderThread.Redraw();
}

void MainWindow::on_edit_im_returnPressed()
{
    GLFrame->RenderThread.setYpos(ui->edit_im->text().toDouble());
    GLFrame->RenderThread.Redraw();
}

void MainWindow::on_edit_re_returnPressed()
{
    GLFrame->RenderThread.setXpos(ui->edit_re->text().toDouble());
    GLFrame->RenderThread.Redraw();
}


