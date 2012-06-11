#ifndef GLRENDERTHREAD_H
#define GLRENDERTHREAD_H

#include <QThread>
#include <QGLWidget>
#include <QWaitCondition>
#include <QMutex>
#include <math.h>

#define GL_SHADING_LANGUAGE_VERSION       0x8B8C

class QGLFrame;
class QSize;
class QGLShaderProgram;
class QGLShader;

class QGLRenderThread : public QThread
{
    Q_OBJECT
public:
    explicit QGLRenderThread(QGLFrame *parent = 0);
    void resizeViewport(const QSize &size);

    void setXpos(double val) {xpos = val;}
    void setYpos(double val) {ypos = val;}
    void setZoom(double val) {zoom = ((double)(128.))*((double)(exp2(val)));}
    void setIterations(int val) {max_iterations = val;}
    void setBenchmark(bool OnOff) {benchmark = OnOff; FrameCounter=0;}
    void setRenderMode(int Mode) {RenderMode=Mode;reloadShader = true;}
    void setCenter(const QPoint &pos);
    void Zoom(bool dir, const QPoint &pos, double);
    void getPosition(double &x, double &y);
    void getZoom(double &z) {z=zoom;}
    int getIterations(void) {return(max_iterations);}
    void getMousePosition(double& x, double& y, const QPoint &pos);
    void Drag(const QPoint &dir);

    void Redraw();

    void run(void);
    void stop(void);
    void ReloadShader(void) {reloadShader = true;}

protected:
    void GLInit(void);
    void GLResize(int& width, int& height);
    void paintGL(void);
    void LoadShader(const QString& vshader,const QString& fshader);

signals:
    void showFPS(QString fps);

private:
    bool doRendering, doResize, UpdateFrame, reloadShader, benchmark;
    int w, h, FrameCounter;

    QGLFrame *GLFrame;

    QGLShaderProgram *ShaderProgram;
    QGLShader *VertexShader, *FragmentShader;

    double xpos, ypos, zoom, radius;
    int max_iterations;
    int RenderMode;
    QStringList VShader;

    QWaitCondition WaitCondition;
    QMutex Mutex;


signals:

public slots:

};

#endif // GLRENDERTHREAD_H
