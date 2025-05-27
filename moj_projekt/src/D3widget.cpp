#include "D3widget.hpp"

// Współrzędne wierzchołków prostopadłościanu (8 punktów)
// Każdy wierzchołek: pozycja (x,y,z) oraz kolor (r,g,b)
static const GLfloat vertices[] = {
    // Front face (z = +0.75) - czerwony
    -0.5f, -0.3f, +0.75f,   1.0f, 0.0f, 0.0f, // 0
    0.5f, -0.3f, +0.75f,   1.0f, 0.0f, 0.0f, // 1
    0.5f, +0.3f, +0.75f,   1.0f, 0.0f, 0.0f, // 2
    -0.5f, +0.3f, +0.75f,   1.0f, 0.0f, 0.0f, // 3

    // Back face (z = -0.75) - zielony
    -0.5f, -0.3f, -0.75f,   0.0f, 1.0f, 0.0f, // 4
    -0.5f, +0.3f, -0.75f,   0.0f, 1.0f, 0.0f, // 5
    0.5f, +0.3f, -0.75f,   0.0f, 1.0f, 0.0f, // 6
    0.5f, -0.3f, -0.75f,   0.0f, 1.0f, 0.0f, // 7

    // Left face (x = -0.5) - niebieski
    -0.5f, -0.3f, -0.75f,   0.0f, 0.0f, 1.0f, // 8
    -0.5f, -0.3f, +0.75f,   0.0f, 0.0f, 1.0f, // 9
    -0.5f, +0.3f, +0.75f,   0.0f, 0.0f, 1.0f, // 10
    -0.5f, +0.3f, -0.75f,   0.0f, 0.0f, 1.0f, // 11

    // Right face (x = +0.5) - żółty
    0.5f, -0.3f, +0.75f,   1.0f, 1.0f, 0.0f, // 12
    0.5f, -0.3f, -0.75f,   1.0f, 1.0f, 0.0f, // 13
    0.5f, +0.3f, -0.75f,   1.0f, 1.0f, 0.0f, // 14
    0.5f, +0.3f, +0.75f,   1.0f, 1.0f, 0.0f, // 15

    // Top face (y = +0.3) - cyjan
    -0.5f, +0.3f, +0.75f,   0.0f, 1.0f, 1.0f, // 16
    0.5f, +0.3f, +0.75f,   0.0f, 1.0f, 1.0f, // 17
    0.5f, +0.3f, -0.75f,   0.0f, 1.0f, 1.0f, // 18
    -0.5f, +0.3f, -0.75f,   0.0f, 1.0f, 1.0f, // 19

    // Bottom face (y = -0.3) - magenta
    -0.5f, -0.3f, +0.75f,   1.0f, 0.0f, 1.0f, // 20
    0.5f, -0.3f, +0.75f,   1.0f, 0.0f, 1.0f, // 21
    0.5f, -0.3f, -0.75f,   1.0f, 0.0f, 1.0f, // 22
    -0.5f, -0.3f, -0.75f,   1.0f, 0.0f, 1.0f  // 23
};

// Indeksy do narysowania 12 trójkątów (6 ścian po dwa trójkąty)
static const GLuint indices[] = {
    // Front face
    0,  1, 2,   2,  3, 0,
    // Back face
    4,  5, 6,   6,  7, 4,
    // Left face
    8,  9,10,  10, 11, 8,
    // Right face
    12, 13,14,  14, 15,12,
    // Top face
    16, 17,18,  18, 19,16,
    // Bottom face
    20, 21,22,  22, 23,20,
};

D3widget::D3widget(QWidget* parent)
    : QOpenGLWidget(parent)
    , m_vao(0), m_vbo(0), m_ebo(0)
    , m_pitch(0), m_roll(0)
{}

D3widget::~D3widget(){
    makeCurrent();
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
    glDeleteVertexArrays(1, &m_vao);
    doneCurrent();
}

void D3widget::initializeGL(){
    initializeOpenGLFunctions();
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    initShaders();
    initGeometry();
}

void D3widget::initShaders(){
    m_program.addShaderFromSourceCode(
        QOpenGLShader::Vertex,
        "#version 330 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "layout(location = 1) in vec3 aColor;\n"
        "uniform mat4 u_mvp;\n"
        "out vec3 vColor;\n"
        "void main(){\n"
        "    gl_Position = u_mvp * vec4(aPos, 1.0);\n"
        "    vColor = aColor;\n"
        "}"
        );
    m_program.addShaderFromSourceCode(
        QOpenGLShader::Fragment,
        "#version 330 core\n"
        "in vec3 vColor;\n"
        "out vec4 FragColor;\n"
        "void main(){\n"
        "    FragColor = vec4(vColor, 1.0);\n"
        "}"
        );
    m_program.link();
}

void D3widget::initGeometry(){
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Atrybut pozycji: lokalizacja 0, 3 składowe, stride = 6 * sizeof(GLfloat)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);

    // Atrybut koloru: lokalizacja 1, 3 składowe, offset = 3 * sizeof(GLfloat)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    glBindVertexArray(0);
}

void D3widget::resizeGL(int w, int h){
    float aspect = float(w)/float(h? h:1);
    m_projection.setToIdentity();
    m_projection.perspective(45.0f, aspect, 0.1f, 100.0f);
}

void D3widget::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 view;
    view.translate(0,0,-5);
    view.rotate(m_pitch * 180.0f/M_PI, -1,0,0);
    view.rotate(m_roll  * 180.0f/M_PI, 0,0,1);

    QMatrix4x4 mvp = m_projection * view;
    m_program.bind();
    m_program.setUniformValue("u_mvp", mvp);

    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(GLuint),
                   GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void D3widget::setRotationAngles(float pitch, float roll){
    m_pitch = pitch;
    m_roll  = roll;
    update();  // wymuś repaint
}
