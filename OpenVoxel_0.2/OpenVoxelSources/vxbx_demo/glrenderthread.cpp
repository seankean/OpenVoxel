#include <QGLShader>
#include <QFileInfo>

#include "glframe.h"
#include "glrenderthread.h"

QGLRenderThread::QGLRenderThread(QGLFrame *parent) :
  QThread(),
  GLFrame(parent)
{
  doRendering = true;
  doResize = false;
  UpdateFrame = true;
  FrameCounter=0;
  benchmark = false;

  ShaderProgram = NULL;
  VertexShader = FragmentShader = NULL;

  xpos=-0.5;
  ypos=0.0;
  zoom=128.*exp2(0);         // see "http://en.wikipedia.org/wiki/Math.h" for more useful math functions
  max_iterations=400;
  radius=4.;
}


void QGLRenderThread::resizeViewport(const QSize &size)
{
  w = size.width();
  h = size.height();
  doResize = true;
}

void QGLRenderThread::Drag(const QPoint &dir)
{
  xpos = xpos-((double)dir.x())/zoom;
  ypos = ypos+((double)dir.y())/zoom;
}


void QGLRenderThread::setCenter(const QPoint &pos)
{
  xpos = xpos+(pos.x()-w/2.)/zoom;
  ypos = ypos-(pos.y()-h/2.)/zoom;
}


void QGLRenderThread::Zoom(bool dir, const QPoint &pos, double zfact)
{
  double c;

  c = xpos+(pos.x()-w/2)/zoom;
  xpos = (xpos+((dir)?1.0:-1.0)*(c-xpos)*((dir)?(1.0-1.0/zfact):(zfact-1.0)));

  c = ypos-(pos.y()-h/2)/zoom;
  ypos = (ypos+((dir)?1.0:-1.0)*(c-ypos)*((dir)?(1.0-1.0/zfact):(zfact-1.0)));

  zoom*=(dir)?zfact:(1.0/zfact);
}


void QGLRenderThread::getPosition(double& x, double& y)
{
  x = xpos;
  y = ypos;
}


void QGLRenderThread::getMousePosition(double& x, double& y, const QPoint &pos)
{
  x = xpos+(pos.x()-w/2)/zoom;
  y = ypos-(pos.y()-h/2)/zoom;
}


void QGLRenderThread::stop()
{
  doRendering = false;
  WaitCondition.wakeOne();
}


void QGLRenderThread::Redraw()
{
  WaitCondition.wakeOne();
}


void QGLRenderThread::run()
{
  long  ilPerfFreq, ilCurrentTime, ilStartTime;
  long double dlTimeElapsed;
  
  //    QueryPerformanceFrequency(&ilPerfFreq);
  //    ilPerfFreq.QuadPart = ilPerfFreq.QuadPart / 1000.;    // we want msecs
  
  GLFrame->makeCurrent();
  GLInit();
  LoadShader("./Basic.vsh", "./FloatMandel.fsh");
  
  //    QueryPerformanceFrequency(&ilStartTime);
  while (doRendering)
    {
      if(doResize)
	{
	  GLResize(w, h);
	  doResize = false;
	}
      if(reloadShader)
	{
	  LoadShader("./Basic.vsh", "./FloatMandel.fsh");
	  reloadShader = false;
	}
      
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glLoadIdentity();
      // render actual frame
      paintGL();
      
      GLFrame->swapBuffers();
      
      Mutex.lock();
      UpdateFrame = false;
      if(!UpdateFrame && doRendering && !reloadShader)
	WaitCondition.wait(&Mutex);
      Mutex.unlock();
      FrameCounter++;
    }
}


void QGLRenderThread::GLInit(void)
{
  glClearColor(0.25f, 0.25f, 0.4f, 0.0f);     // Background => dark blue
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);

  const GLubyte* pGPU = glGetString(GL_RENDERER);
  const GLubyte* pVersion = glGetString(GL_VERSION);
  const GLubyte* pShaderVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

  qDebug() << "GPU: " << QString((char*)pGPU);
  qDebug() << "OpenGL: " << QString((char*)pVersion);
  qDebug() << "GLSL: " << QString((char*)pShaderVersion);
}


void QGLRenderThread::GLResize(int& width, int& height)
{
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);

  glLoadIdentity();

  glOrtho (0, width, 0, height, 0, 1);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


void QGLRenderThread::paintGL(void)
{
  // common shader values
  ShaderProgram->setUniformValue("iterations", max_iterations);
  ShaderProgram->setUniformValue("radius", (float)radius);
  ShaderProgram->setUniformValue("frame", (float)FrameCounter);

  // single precision (float32) shader values
  ShaderProgram->setUniformValue("f_cx",  (float)xpos);
  ShaderProgram->setUniformValue("f_cy",  (float)ypos);

  ShaderProgram->setUniformValue("f_sx",  (float)(-((double)w)/2.0/zoom));
  ShaderProgram->setUniformValue("f_sy",  (float)(-((double)h)/2.0/zoom));

  ShaderProgram->setUniformValue("f_z", (float)(1./zoom));

  // draw canvas
  glBegin(GL_QUADS);
  glNormal3f(0.,0.,1.); glColor3f(1.,0.,0.); glTexCoord2f(0, 0); glVertex3i(0, 0, 0);
  glNormal3f(0.,0.,1.); glColor3f(1.,1.,0.); glTexCoord2f(w, 0); glVertex3i(w, 0, 0);
  glNormal3f(0.,0.,1.); glColor3f(1.,1.,1.); glTexCoord2f(w, h); glVertex3i(w, h, 0);
  glNormal3f(0.,0.,1.); glColor3f(1.,0.,1.); glTexCoord2f(0, h); glVertex3i(0, h, 0);

  //

  //
  glEnd();
}


void QGLRenderThread::LoadShader(const QString& vshader,const QString& fshader)
{
  if(ShaderProgram)
    {
      ShaderProgram->release();
      ShaderProgram->removeAllShaders();
    }
  else ShaderProgram = new QGLShaderProgram;

  if(VertexShader)
    {
      delete VertexShader;
      VertexShader = NULL;
    }

  if(FragmentShader)
    {
      delete FragmentShader;
      FragmentShader = NULL;
    }

  // load and compile vertex shader
  QFileInfo vsh(vshader);
  if(vsh.exists())
    {
      VertexShader = new QGLShader(QGLShader::Vertex);
      if(VertexShader->compileSourceFile(vshader))
	ShaderProgram->addShader(VertexShader);
      else qWarning() << "Vertex Shader Error" << VertexShader->log();
    }
  else qWarning() << "Vertex Shader source file " << vshader << " not found.";


  // load and compile fragment shader
  QFileInfo fsh(fshader);
  if(fsh.exists())
    {
      FragmentShader = new QGLShader(QGLShader::Fragment);
      if(FragmentShader->compileSourceFile(fshader))
	ShaderProgram->addShader(FragmentShader);
      else qWarning() << "Fragment Shader Error" << FragmentShader->log();
    }
  else qWarning() << "Fragment Shader source file " << fshader << " not found.";

  if(!ShaderProgram->link())
    {
      qWarning() << "Shader Program Linker Error" << ShaderProgram->log();
    }
  else ShaderProgram->bind();
}
