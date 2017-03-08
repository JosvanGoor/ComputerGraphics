#include "mainview.h"
#include "math.h"

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
    mainShaderProg->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader.glsl");
    mainShaderProg->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader.glsl");
    mainShaderProg->link();

    /* Add your other shaders below */

    /* End of custom shaders */

    // Store the locations (pointers in gpu memory) of uniforms in Glint's
    glModel = glGetUniformLocation(mainShaderProg->programId(), "model");
    glView = glGetUniformLocation(mainShaderProg->programId(), "view");
    glProjection = glGetUniformLocation(mainShaderProg->programId(), "projection");
    glNormal = glGetUniformLocation(mainShaderProg->programId(), "normal");

    glLightPosition = glGetUniformLocation(mainShaderProg->programId(), "lightPosition");
    glColorFrag = glGetUniformLocation(mainShaderProg->programId(), "colorFrag");
    glEye = glGetUniformLocation(mainShaderProg->programId(), "eyeFrag");
    glMaterial = glGetUniformLocation(mainShaderProg->programId(), "materialFrag");
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
    glEnableVertexAttribArray(2);

    size_t stride = 6 * sizeof(GLfloat);
    void* offsetVertex = 0;
    void* offesetColor = (void*)(3 * sizeof(GLfloat));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, offsetVertex);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, offesetColor);

    glBindVertexArray(0);
}

void MainView::loadModel(QString filename, GLuint bufferObject) {

    cubeModel = new Model(filename);
    numTris = cubeModel->getNumTriangles();
    srand(time(NULL));

    Q_UNUSED(bufferObject);

    QVector<QVector3D> data;

    for(size_t i = 0; i < numTris * 3; ++i)
    {
        data.append(cubeModel->getVertices()[i]);
        data.append(cubeModel->getNormals()[i]);
    }

    glBindBuffer(GL_ARRAY_BUFFER, bufferObject);
    glBufferData(GL_ARRAY_BUFFER, data.size() * 3 * sizeof(GLfloat), (GLfloat*)data.data(), GL_STATIC_DRAW);
}

void MainView::updateBuffers() {
    // Change the data inside buffers (if you want)
    // make sure to change GL_STATIC_DRAW to GL_DYNAMIC_DRAW
    // in the call to glBufferData for better performance

}


void MainView::updateUniforms() {
    // TODO: update the uniforms in the shaders using the glUniform<datatype> functions

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
    scale = 1.0f;
    nearPlane = 0.1f;
    farPlane = 2500.0f;

    rotation = QMatrix4x4();
    projection = QMatrix4x4();
    projection.perspective(30, 1, nearPlane, farPlane);

    createShaderPrograms();

    createBuffers();

    loadModel(":/models/sphere.obj", cubeBO);

    // For animation, you can start your timer here

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

    // TODO: Update projection to fit the new aspect ratio
    qDebug() << "MainView::resizeGL: Window is now size " << newWidth << " - " << newHeight;
    projection = QMatrix4x4();
    projection.perspective(30, ((float)newWidth)/((float)newHeight), nearPlane, farPlane);
}

/**
 * @brief MainView::paintGL
 *
 * Actual function used for drawing to the screen
 *
 */
void MainView::paintGL() {

    // Clear the screen before rendering
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mainShaderProg->bind();

    // TODO: implement your drawing functions
    view = QMatrix4x4();

    QVector3D eye(200.0, 200.0, 1500.0);
    QVector3D center(200.0, 200.0, 0.0);
    QVector3D up(0, 1, 0);

    model = QMatrix4x4();

    view.lookAt(eye, center, up);
    view = view * rotation;

    glBindVertexArray(vao);
    glUniformMatrix4fv(glModel, 1, GL_FALSE, model.data());
    glUniformMatrix4fv(glView, 1, GL_FALSE, view.data());
    glUniformMatrix4fv(glProjection, 1, GL_FALSE, projection.data());
    glUniformMatrix4fv(glNormal, 1, GL_FALSE, normal.data());

    glUniform3f(glEye, eye.x() - center.x(), eye.y() - center.y(), eye.z() - center.z());

    renderRaytracerScene();

    mainShaderProg->release();
}

// Add your function implementations below

// TODO: add your code
