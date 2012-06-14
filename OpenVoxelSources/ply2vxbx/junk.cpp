 class MyGLDrawer : public QGLWidget
 {
     Q_OBJECT        // must include this if you use Qt signals/slots

 public:
     MyGLDrawer(QWidget *parent)
         : QGLWidget(parent) {}

 protected:

     void initializeGL()
     {
         // Set up the rendering context, define display lists etc.:
         glClearColor(0.0, 0.0, 0.0, 0.0);
         glEnable(GL_DEPTH_TEST);
     }

     void resizeGL(int w, int h)
     {
         // setup viewport, projection etc.:
         glViewport(0, 0, (GLint)w, (GLint)h);
         ...
         glFrustum(...);
         ...
     }

     void paintGL()
     {
         // draw the scene:
         ...
         glRotatef(...);
         glMaterialfv(...);
         glBegin(GL_QUADS);
         glVertex3f(...);
         glVertex3f(...);
         ...
         glEnd();
         ...
     }

 };
