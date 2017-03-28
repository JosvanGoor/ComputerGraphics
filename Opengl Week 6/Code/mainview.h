#ifndef MAINVIEW_H
#define MAINVIEW_H

#include "model.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLDebugLogger>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QVector3D>

class MainView : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

public:
    MainView(QWidget *parent = 0);
    ~MainView();

    void updateRotation(int x, int y, int z);
    void updateModel(QString name);
    void updateShader(QString name);
    void updateScale(float scale);

    /* Add your public members below */
    QVector<quint8> imageToBytes(QImage image);
    /* End of public members */

    QVector3D convertHSLtoRGB(float H, float S, float L);

private slots:
    void updateScene();


protected:
    void initializeGL();
    void resizeGL(int newWidth, int newHeight);
    void paintGL();


    // Functions for keyboard input events
    void keyPressEvent(QKeyEvent *ev);
    void keyReleaseEvent(QKeyEvent *ev);

    // Function for mouse input events
    void mouseDoubleClickEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void wheelEvent(QWheelEvent *ev);

private:
    QOpenGLDebugLogger *debugLogger;

    void createShaderPrograms();
    void createBuffers();
    void updateBuffers();
    void updateUniforms();

    // Raytracer scene functions
    void renderSphere(GLint type, GLint theta, QVector3D pos, QVector3D color, QVector4D material, QVector3D lightpos);
    void renderRaytracerScene();

    /* Personally added variables */
    GLuint earth;
    GLuint mars;
    GLuint jupiter;
    GLuint saturn;
    GLuint sun;
    GLuint cat_diff;

    GLuint fbo;
    GLint defaultFbo;
    GLuint fboDepthBuffer;
    GLuint fboColorBuffer;
    GLuint fboNormalBuffer;

    GLuint vao;
    GLuint coords;
    GLuint normals;
    GLuint colors;

    GLuint quadVao;
    GLuint quadBo;

    GLuint vaoEarth;
    GLuint vaoMoon;

    GLint glSampler_1;
    GLint glModel;
    GLint glView;
    GLint glProjection;
    GLint glNormal;
    GLint glLightPosition;
    GLint glMaterial;
    GLint glColorFrag;
    GLint glEye;
    GLint glIsSun;
    GLint glInverseMatrix;

    GLuint gl2Sampler_diffuse;
    GLuint gl2Sampler_normals;
    GLuint gl2Sampler_depth;

    float scale;
    float nearPlane;
    float farPlane;

    QMatrix4x4 view;
    QMatrix4x4 projection;
    QMatrix4x4 model;


    QMatrix4x4 rotation;

    QMatrix3x3 normal;

    GLdouble angle1, angle2, angle3, angle4, angle5;
    //GLint angle1, angle2, angle3;


    /* Add your private members below */
    void loadModel(QString filename, GLuint bufferObject);
    GLuint loadTexture(QString filename);

    // Shader programs, GLint for uniforms/buffer objects, other variables
    QOpenGLShaderProgram *mainShaderProg;
    QOpenGLShaderProgram *secondShaderProg;

    QTimer timer; // timer used for animation

    Model *cubeModel;
    GLuint cubeBO;

    unsigned numTris;

    /* End of private members */

private slots:
    void onMessageLogged( QOpenGLDebugMessage Message );

};

#endif // MAINVIEW_H
