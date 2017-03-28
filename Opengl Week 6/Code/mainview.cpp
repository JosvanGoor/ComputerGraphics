#include "mainview.h"
#include "math.h"

#include <fstream>

#include <QDateTime>

/**
 * @brief MainView::MainView
 *
 * Constructor of MainView
 *
 * @param parent
 */
MainView::MainView(QWidget *parent) : QOpenGLWidget(parent) {
    qDebug() << "MainView constructor";

    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
}

/**
 * @brief MainView::~MainView
 *
 * Destructor of MainView
 * This is the last function called, before exit of the program
 * Use this to clean up your variables, buffers etc.
 *
 */
MainView::~MainView() {
    delete cubeModel;

    // Free Buffer Objects before Vertex Arrays
    glDeleteBuffers(1, &cubeBO);
    glDeleteVertexArrays(1, &vao);

    // Free the main shader
    delete mainShaderProg;

    debugLogger->stopLogging();

    qDebug() << "MainView destructor";
}

/**
 * @brief MainView::createShaderPrograms
 *
 * Creates the shaderprogram for OpenGL rendering
 */
void MainView::createShaderPrograms() {
    // Qt wrapper (way cleaner than using pure OpenGL)
    mainShaderProg = new QOpenGLShaderProgram();
    mainShaderProg->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader_pass1.glsl");
    mainShaderProg->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader_pass1.glsl");
    mainShaderProg->link();
    /* Add your other shaders below */
    secondShaderProg = new QOpenGLShaderProgram();
    secondShaderProg->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader_pass2.glsl");
    secondShaderProg->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader_pass2.glsl");
    secondShaderProg->link();

    gl2Sampler_diffuse = glGetUniformLocation(secondShaderProg->programId(), "diffuse");
    glUniform1i(gl2Sampler_diffuse, 0);
    gl2Sampler_normals = glGetUniformLocation(secondShaderProg->programId(), "normals");
    glUniform1i(gl2Sampler_normals, 1);
    gl2Sampler_depth = glGetUniformLocation(secondShaderProg->programId(), "depth");
    glUniform1i(gl2Sampler_depth, 2);

    glInverseMatrix = glGetUniformLocation(secondShaderProg->programId(), "inverseMatrix");
    glLightPosition = glGetUniformLocation(secondShaderProg->programId(), "lightPos");
    glEye = glGetUniformLocation(secondShaderProg->programId(), "viewPos");
    glColorFrag = glGetUniformLocation(secondShaderProg->programId(), "lightColor");
    glMaterial = glGetUniformLocation(secondShaderProg->programId(), "material");

    /* End of custom shaders */

    // Store the locations (pointers in gpu memory) of uniforms in Glint's
    glModel = glGetUniformLocation(mainShaderProg->programId(), "model");
    glView = glGetUniformLocation(mainShaderProg->programId(), "view");
    glProjection = glGetUniformLocation(mainShaderProg->programId(), "projection");
    glNormal = glGetUniformLocation(mainShaderProg->programId(), "normal");
    glSampler_1 = glGetUniformLocation(mainShaderProg->programId(), "diffuse");

    qDebug() << glModel << " " << glView << " " << glProjection << " " << glNormal;
}

/**
 * @brief MainView::createBuffers
 *
 * Creates necessary buffers for your application
 */
void MainView::createBuffers() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &cubeBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeBO);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    size_t stride = (3 + 3 + 2) * sizeof(GLfloat);
    void* offsetVertex = 0;
    void* offsetNormal = (void*)(3 * sizeof(GLfloat));
    void* offsetTexture = (void*)((3 * 2) * sizeof(GLfloat));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, offsetVertex);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, offsetNormal);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, offsetTexture);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //generate quad vao
    glGenVertexArrays(1, &quadVao);
    glBindVertexArray(quadVao);

    glGenBuffers(1, &quadBo);
    glBindBuffer(GL_ARRAY_BUFFER, quadBo);

    GLfloat VLOW = -1.0;
    GLfloat TLOW = 0.0;
    GLfloat HIGH = 1.0;
    GLfloat floats[] = {-1.0, -1.0, 0.0, 0.0, 0.0,
                        1.0, -1.0, 0.0, 1.0, 0.0,
                        -1.0, 1.0, 0.0, 0.0, 1.0,
                        1.0, -1.0, 0.0, 1.0, 0.0,
                        1.0, 1.0, 0.0, 1.0, 1.0,
                        -1.0, 1.0, 0.0, 0.0, 1.0};
    glBufferData(GL_ARRAY_BUFFER, sizeof(floats), floats, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    stride = (3 + 2) * sizeof(GLfloat);
    void *voffset = (void*)0;
    void *toffset = (void*)(3 * sizeof(GLfloat));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, voffset);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, toffset);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //Generate fbo (dummy size for textures, will be set correctly at resize.)
    glGenTextures(1, &fboDepthBuffer);
    glBindTexture(GL_TEXTURE_2D, fboDepthBuffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 128, 128, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

    glGenTextures(1, &fboColorBuffer);
    glBindTexture(GL_TEXTURE_2D, fboColorBuffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glGenTextures(1, &fboNormalBuffer);
    glBindTexture(GL_TEXTURE_2D, fboNormalBuffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboColorBuffer, 0);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, fboNormalBuffer, 0);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fboDepthBuffer, 0);

    GLenum drawBuffers[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, drawBuffers);

    GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
    if(status != GL_FRAMEBUFFER_COMPLETE) qDebug() << "Framebuffer error: " << status;

    //glBindFramebuffer(GL_DRAW_FRAMEBUFFER, defaultFbo);
}

void MainView::loadModel(QString filename, GLuint bufferObject) {

    cubeModel = new Model(filename);
    numTris = cubeModel->getNumTriangles();
    srand(time(NULL));

    QVector<float> data = cubeModel->getVNTInterleaved();

    glBindBuffer(GL_ARRAY_BUFFER, bufferObject);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), (GLfloat*)data.data(), GL_STATIC_DRAW);
}

GLuint MainView::loadTexture(QString filename)
{
    GLuint rval = 0;
    QImage img(filename);
    QVector<quint8> data = imageToBytes(img);

    glGenTextures(1, &rval);
    glBindTexture(GL_TEXTURE_2D, rval);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, img.width(), img.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    return rval;
}

void MainView::updateBuffers() {
    // Change the data inside buffers (if you want)
    // make sure to change GL_STATIC_DRAW to GL_DYNAMIC_DRAW
    // in the call to glBufferData for better performance

}


void MainView::updateUniforms() {
    // TODO: update the uniforms in the shaders using the glUniform<datatype> functions

}

void MainView::updateScene() {
   update();
}

/**
 * @brief MainView::initializeGL
 *
 * Called upon OpenGL initialization
 * Attaches a debugger and calls other init functions
 */
void MainView::initializeGL() {
    qDebug() << ":: Initializing OpenGL";
    initializeOpenGLFunctions();

    debugLogger = new QOpenGLDebugLogger();
    connect( debugLogger, SIGNAL( messageLogged( QOpenGLDebugMessage ) ), this, SLOT( onMessageLogged( QOpenGLDebugMessage ) ), Qt::DirectConnection );

    if ( debugLogger->initialize() ) {
        qDebug() << ":: Logging initialized";
        debugLogger->startLogging( QOpenGLDebugLogger::SynchronousLogging );
        debugLogger->enableMessages();
    }

    QString glVersion;
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    qDebug() << ":: Using OpenGL" << qPrintable(glVersion);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable backface culling
    glEnable(GL_CULL_FACE);

    // Default is GL_LESS
    glDepthFunc(GL_LEQUAL);

    // Set the color of the screen to be black on clear (new frame)
    glClearColor(0.0, 0.0, 0.0, 1.0);

    /* TODO: call your initialization functions here */
    scale = 0.6f;
    nearPlane = 0.1f;
    farPlane = 2500.0f;

    projection = QMatrix4x4();
    projection.perspective(30, 1, nearPlane, farPlane);

    createShaderPrograms();

    qDebug() << "Create dem buffers yo!";
    createBuffers();

    loadModel(":/models/cat.obj", cubeBO);
    cat_diff = loadTexture(":/textures/cat_diff.png");

    // For animation, you can start your timer here
  //  QTimer *timer = new QTimer(this);
  //  connect(timer, SIGNAL(timeout()), this, SLOT(updateScene()));
  //  timer->start(1/60);
}

/**
 * @brief MainView::resizeGL
 *
 * Called upon resizing of the screen
 *
 * @param newWidth
 * @param newHeight
 */
void MainView::resizeGL(int newWidth, int newHeight) {
    qDebug() << "MainView::resizeGL: Window is now size " << newWidth << " - " << newHeight;
    projection = QMatrix4x4();
    projection.perspective(30, ((float)newWidth)/((float)newHeight), nearPlane, farPlane);

    
    //resize the fbo's buffers
    glBindTexture(GL_TEXTURE_2D, fboDepthBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, newWidth, newHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

    glBindTexture(GL_TEXTURE_2D, fboColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newWidth, newHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glBindTexture(GL_TEXTURE_2D, fboNormalBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newWidth, newHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    
}

void screenshot (char filename[160],int x, int y)
{// get the image data
    long imageSize = x * y * 3;
    unsigned char *data = new unsigned char[imageSize];

    //glReadPixels(0,0,x,y, GL_BGR,GL_UNSIGNED_BYTE,data);// split x and y sizes into bytes
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    int xa= x % 256;
    int xb= (x-xa)/256;int ya= y % 256;
    int yb= (y-ya)/256;//assemble the header
    unsigned char header[18]={0,0,2,0,0,0,0,0,0,0,0,0,(unsigned char)xa,(unsigned char)xb,(unsigned char)ya,(unsigned char)yb,24,0};
    // write header and data to file
    std::fstream File(filename, std::ios::out | std::ios::binary);
    File.write (reinterpret_cast<char *>(header), sizeof (char)*18);
    File.write (reinterpret_cast<char *>(data), sizeof (char)*imageSize);
    File.close();

    delete[] data;
    data=NULL;
}

/**
 * @brief MainView::paintGL
 *
 * Actual function used for drawing to the screen
 *
 */
void MainView::paintGL() {


    //FIRST PASS
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
    glBindVertexArray(vao);
    //glBindBuffer(GL_ARRAY_BUFFER, cubeBO);

    // Clear the screen before rendering
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mainShaderProg->bind();

    view = QMatrix4x4();
    QVector3D eye(0.5, 0.7, 4.0);
    QVector3D center(0.5, 0.7, 0.0);

    //QVector3D eye(0.0, 0.0, 2.0);
    //QVector3D center(0.0, 0.0, 0.0);
    QVector3D up(0, 1, 0);
    view.lookAt(eye, center, up);
    view = view * rotation;

    renderRaytracerScene();

    mainShaderProg->release();

    //SECOND PASS
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, defaultFbo);

    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    secondShaderProg->bind();

    glBindVertexArray(quadVao);
    glBindBuffer(GL_ARRAY_BUFFER, quadBo);

    //sending uniform
    QMatrix4x4 inverse = (model * view * projection).inverted();
    glUniformMatrix4fv(glInverseMatrix, 1, GL_FALSE, inverse.data());

    QVector3D lightPos = QVector3D(0, 0, 200);
    glUniform3f(glLightPosition, lightPos.x(), lightPos.y(), lightPos.z());

    QVector3D lightColor = QVector3D(1.0, 1.0, 1.0);
    glUniform3f(glColorFrag, lightColor.x(), lightColor.y(), lightColor.z());

    QVector4D material = QVector4D(0.2f,0.7f,0.5f,64);
    glUniform4f(glMaterial, material.x(), material.y(), material.z(), material.w());

    glUniform3f(glEye, eye.x(), eye.y(), eye.z());

    glUniform1i(glGetUniformLocation(secondShaderProg->programId(), "diffuse"), 0);
    glUniform1i(glGetUniformLocation(secondShaderProg->programId(), "normals"), 1);
    glUniform1i(glGetUniformLocation(secondShaderProg->programId(), "depth"), 2);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fboColorBuffer);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, fboNormalBuffer);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, fboDepthBuffer);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    secondShaderProg->release();
    glBindVertexArray(0);

}

// Add your function implementations below

// TODO: add your code
