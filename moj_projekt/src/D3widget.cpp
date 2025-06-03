#include "D3widget.hpp"



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

void D3widget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // 1) Inicjalizacja shaderów
    initShaders();

    // 2) Inicjalizacja geometrii kapsuły
    initGeometry();

    // 3) Inicjalizacja geometrii słupka (beam)
    initBeamGeometry();

    // 4) Inicjalizacja geometrii wiatraczka (hub + łopatki)
    initFanGeometry();

    // 5) Ustawiamy kąt początkowy i uruchamiamy timer do animacji wiatraczka
    m_fanAngle = 0.0f;
    startTimer(16); // ~60 FPS
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

/**
 * @brief Dodaje dane wierzchołków i indeksów dla prostopadłościanu reprezentującego belkę.
 *
 * Belka jest definiowana przez swój początkowy punkt i końcowy punkt wzdłuż głównej osi
 * (np. osi X) oraz wymiary (szerokość i grubość) w kierunkach prostopadłych.
 *
 * @param vertices Wektor, do którego zostaną dodane wierzchołki (x, y, z, r, g, b).
 * @param indices  Wektor, do którego zostaną dodane indeksy.
 * @param offset   Aktualny offset wierzchołków (liczba wierzchołków przed dodaniem tych belki).
 * @param start    Początkowy punkt (vec3) belki.
 * @param end      Końcowy punkt (vec3) belki.
 * @param width    Szerokość belki (wzdłuż np. osi Z dla belki poziomej).
 * @param thickness Grubość belki (wzdłuż np. osi Y dla belki poziomej).
 *
 * Dodawane wierzchołki tworzą prostopadłościan – 8 wierzchołków.
 */

void D3widget::initGeometry() {
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    // Parametry kapsuły
    const float R = 0.5f;     // promień półkul
    const float H = 0.4f;     // wysokość samego cylindra (od równika górnego do równika dolnego)
    const int sectors = 20;   // podział wzdłuż kąta (okrąg)
    const int stackTop = 10;      // liczba "warstw" w górnej półkuli (od bieguna do równika)
    const int stackBottom = 10;   // liczba "warstw" w dolnej półkuli (od równika do bieguna)
    const int stackCylinder = 4;  // liczba warstw pomiędzy równikami (nie licząc samych równików)

    // Środki półkul:
    // - górna półkula jest wyprowadzona ponad y = +H/2
    // - dolna półkula jest wyprowadzona poniżej y = −H/2
    const float centerTop =  H / 2.0f;   // = +0.2
    const float centerBottom = -H / 2.0f; // = -0.2

    // ========================
    // 1) Górna półkula
    // ========================
    // Generujemy od i=0 (bieguna) do i=stackTop (równik). Dla i=0: α=0 → y = centerTop + R (biegun).
    // Dla i=stackTop: α=π/2 → y = centerTop + R*cos(π/2) = centerTop  (równik).
    for (int i = 0; i <= stackTop; i++) {
        float alpha = (static_cast<float>(i) * (M_PI / 2.0f)) / static_cast<float>(stackTop); // α ∈ [0, π/2]
        float y = centerTop + R * cos(alpha);    // na biegunie cos(0)=1 → y = centerTop + R; na równiku cos(π/2)=0 → y = centerTop
        float rSlice = R * sin(alpha);           // promień przekroju w danej warstwie
        for (int j = 0; j <= sectors; j++) {
            float phi = static_cast<float>(j) * 2.0f * M_PI / static_cast<float>(sectors);
            float x = rSlice * cos(phi);
            float z = rSlice * sin(phi);
            // Kolorowanie – górna część jasna
            float r_col = 0.8f, g_col = 0.8f, b_col = 0.8f;
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            vertices.push_back(r_col);
            vertices.push_back(g_col);
            vertices.push_back(b_col);
        }
    }
    // Liczba wierzchołków górnej półkuli (oznaczona wierzchołkami od bieguna aż po równik):
    // (stackTop+1) warstw × (sectors+1) punktów na każdej warstwie
    int topVertexCount = (stackTop + 1) * (sectors + 1);

    // ========================
    // 2) Część cylindryczna
    // ========================
    for (int i = 1; i < stackCylinder; i++) {
        float t = static_cast<float>(i) / static_cast<float>(stackCylinder); // t ∈ (0,1), wykluczając dokładne 0 i 1
        float y = centerTop - (H * t);
        for (int j = 0; j <= sectors; j++) {
            float phi = static_cast<float>(j) * 2.0f * M_PI / static_cast<float>(sectors);
            float x = R * cos(phi);
            float z = R * sin(phi);
            // Kolor pomiędzy górą (0.8) a dołem (0.4) liniowo w zależności od t:
            float r_col = 0.8f * (1.0f - t) + 0.4f * t;
            float g_col = 0.8f * (1.0f - t) + 0.4f * t;
            float b_col = 0.8f * (1.0f - t) + 0.4f * t;
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            vertices.push_back(r_col);
            vertices.push_back(g_col);
            vertices.push_back(b_col);
        }
    }
    int cylinderVertexCount = (stackCylinder - 1) * (sectors + 1);

    // ========================
    // 3) Dolna półkula
    // ========================
    // Chcemy włącznie jeden okrąg w równiku (y = centerBottom) i ostatni punkt w biegunie (y = centerBottom - R).
    // Parametryzujemy kąt α ∈ [0, π/2]:
    //   dla α = 0 → y = centerBottom, rSlice = R   (równik)
    //   dla α = π/2 → y = centerBottom - R, rSlice = 0  (biegun)
    for (int i = 0; i <= stackBottom; i++) {
        float alpha = (static_cast<float>(i) * (M_PI / 2.0f)) / static_cast<float>(stackBottom); // α ∈ [0, π/2]
        float y = centerBottom - R * sin(alpha);
        float rSlice = R * cos(alpha);
        for (int j = 0; j <= sectors; j++) {
            float phi = static_cast<float>(j) * 2.0f * M_PI / static_cast<float>(sectors);
            float x = rSlice * cos(phi);
            float z = rSlice * sin(phi);
            // Kolor dolnej części – ciemniejszy
            float r_col = 0.4f, g_col = 0.4f, b_col = 0.4f;
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            vertices.push_back(r_col);
            vertices.push_back(g_col);
            vertices.push_back(b_col);
        }
    }
    // Dolna półkula daje (stackBottom+1) warstw × (sectors+1) punktów
    int bottomVertexCount = (stackBottom + 1) * (sectors + 1);

    // ========================
    // 4) Obliczamy offsety fragmentów w buforze wierzchołków
    // ========================
    int topOffset      = 0;
    int cylinderOffset = topOffset + topVertexCount;
    int bottomOffset   = cylinderOffset + cylinderVertexCount;

    // ========================
    // 5) Indeksy: GÓRNA PÓŁKULA (siatka trójkątna między kolejnymi "warstwami")
    // ========================
    // Łączymy każdą warstwę i ze "ścieżką" i+1:
    //   i = 0..stackTop-1, j = 0..sectors-1
    for (int i = 0; i < stackTop; i++) {
        for (int j = 0; j < sectors; j++) {
            int first  = topOffset + i * (sectors + 1) + j;
            int second = topOffset + (i + 1) * (sectors + 1) + j;
            // Dwie trójkątne pola w "kwadracie" [i,j] – [i+1,j] – [i,j+1] – [i+1,j+1]
            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(first + 1);
            indices.push_back(second);
            indices.push_back(second + 1);
        }
    }

    // ========================
    // 6) Indeksy: POŁĄCZENIE GÓRNEJ PÓŁKULI Z CYLINDREM
    // ========================
    // Równik górnej półkuli to warstwa i = stackTop (jej początek w tablicy wierzchołków: topOffset + stackTop*(sectors+1))
    // Pierwsza warstwa cylindryczna to cylinderOffset + 0*(sectors+1) (bo cylinder generowaliśmy od i=1 do <stackCylinder, ale to jest ring 0)
    // Dla j = 0..sectors-1 zrobiemy trójkąty łączące oba okręgi w równiku.
    {
        int ringTopEquator   = topOffset + stackTop * (sectors + 1);
        int ringCylinder0    = cylinderOffset + 0 * (sectors + 1);
        for (int j = 0; j < sectors; j++) {
            int topA = ringTopEquator + j;
            int topB = ringTopEquator + j + 1;
            int cyA  = ringCylinder0   + j;
            int cyB  = ringCylinder0   + j + 1;

            indices.push_back(topA);
            indices.push_back(cyA);
            indices.push_back(topB);

            indices.push_back(topB);
            indices.push_back(cyA);
            indices.push_back(cyB);
        }
    }

    // ========================
    // 7) Indeksy: CZĘŚĆ CYLINDRYCZNA (pomiędzy kolejnymi ringami cylindrycznymi)
    // ========================
    for (int i = 0; i < (stackCylinder - 1) - 1; i++) {

        for (int j = 0; j < sectors; j++) {
            int first  = cylinderOffset + i       * (sectors + 1) + j;
            int second = cylinderOffset + (i + 1) * (sectors + 1) + j;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(first + 1);
            indices.push_back(second);
            indices.push_back(second + 1);
        }
    }
    // UWAGA: jeśli stackCylinder-1 < 2, to pętla powyżej nie wykona się (ale w naszym przykładzie stackCylinder=4).

    // ========================
    // 8) Indeksy: POŁĄCZENIE CYLINDRA Z DOLNĄ PÓŁKULĄ
    // ========================
    // Ostatni ring cylindryczny: jego indeks to (stackCylinder-2) w zakresie 0..(stackCylinder-2)
    // Pierwszy ring dolnej półkuli (równik dolny) to bottomOffset + 0*(sectors+1).
    {
        int ringCylinderLast = cylinderOffset + (stackCylinder - 2) * (sectors + 1);
        int ringBottomEquator = bottomOffset + 0 * (sectors + 1);
        for (int j = 0; j < sectors; j++) {
            int cyA = ringCylinderLast + j;
            int cyB = ringCylinderLast + j + 1;
            int botA = ringBottomEquator + j;
            int botB = ringBottomEquator + j + 1;

            indices.push_back(cyA);
            indices.push_back(botA);
            indices.push_back(cyB);

            indices.push_back(cyB);
            indices.push_back(botA);
            indices.push_back(botB);
        }
    }

    // ========================
    // 9) Indeksy: DOLNA PÓŁKULA
    // ========================
    // Łączymy kolejne warstwy i i+1 w dolnej półkuli:
    // i = 0..stackBottom-1, j = 0..sectors-1.
    for (int i = 0; i < stackBottom; i++) {
        for (int j = 0; j < sectors; j++) {
            int first  = bottomOffset + i       * (sectors + 1) + j;
            int second = bottomOffset + (i + 1) * (sectors + 1) + j;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(first + 1);
            indices.push_back(second);
            indices.push_back(second + 1);
        }
    }

    // ========================
    // 10) Zapis długości indeksów i wczytanie do VAO/VBO/EBO
    // ========================
    m_indexCount = static_cast<GLsizei>(indices.size());

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(GLfloat),
                 vertices.data(),
                 GL_STATIC_DRAW);

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(GLuint),
                 indices.data(),
                 GL_STATIC_DRAW);

    // Atrybuty: pozycja (lok. 0), kolor (lok. 1)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    glBindVertexArray(0);
}

void D3widget::initBeamGeometry()
{
    const float R             = 0.5f;
    const float beamLength    = 0.6f;
    const float beamThickness = 0.05f;
    const float halfT         = beamThickness * 0.5f;

    std::vector<GLfloat> beamVerts;
    std::vector<GLuint>  beamIdx;

    for (int k = 0; k < 4; ++k) {
        float angle = M_PI / 4.0f + float(k) * (M_PI / 2.0f);
        float dx = std::cos(angle);
        float dz = std::sin(angle);
        float px = -dz;
        float pz =  dx;

        float x0 = R * dx;
        float z0 = R * dz;
        float x1 = (R + beamLength) * dx;
        float z1 = (R + beamLength) * dz;

        float yMin = -halfT;
        float yMax = +halfT;

        std::array<std::array<float,3>,8> corners = {
            std::array<float,3>{ x0 + px * halfT, yMin, z0 + pz * halfT }, // 0
            std::array<float,3>{ x0 - px * halfT, yMin, z0 - pz * halfT }, // 1
            std::array<float,3>{ x0 - px * halfT, yMax, z0 - pz * halfT }, // 2
            std::array<float,3>{ x0 + px * halfT, yMax, z0 + pz * halfT }, // 3
            std::array<float,3>{ x1 + px * halfT, yMin, z1 + pz * halfT }, // 4
            std::array<float,3>{ x1 - px * halfT, yMin, z1 - pz * halfT }, // 5
            std::array<float,3>{ x1 - px * halfT, yMax, z1 - pz * halfT }, // 6
            std::array<float,3>{ x1 + px * halfT, yMax, z1 + pz * halfT }  // 7
        };

        float r_col = 0.3f, g_col = 0.3f, b_col = 0.3f;
        GLuint indexOffset = static_cast<GLuint>(beamVerts.size() / 6);

        for (int i = 0; i < 8; ++i) {
            beamVerts.push_back(corners[i][0]);
            beamVerts.push_back(corners[i][1]);
            beamVerts.push_back(corners[i][2]);
            beamVerts.push_back(r_col);
            beamVerts.push_back(g_col);
            beamVerts.push_back(b_col);
        }

        std::array<GLuint,36> localIdx = {
            indexOffset+0, indexOffset+4, indexOffset+5,
            indexOffset+0, indexOffset+5, indexOffset+1,

            indexOffset+1, indexOffset+5, indexOffset+6,
            indexOffset+1, indexOffset+6, indexOffset+2,

            indexOffset+2, indexOffset+6, indexOffset+7,
            indexOffset+2, indexOffset+7, indexOffset+3,

            indexOffset+3, indexOffset+7, indexOffset+4,
            indexOffset+3, indexOffset+4, indexOffset+0,

            indexOffset+0, indexOffset+3, indexOffset+2,
            indexOffset+0, indexOffset+2, indexOffset+1,

            indexOffset+4, indexOffset+5, indexOffset+6,
            indexOffset+4, indexOffset+6, indexOffset+7
        };
        beamIdx.insert(beamIdx.end(), localIdx.begin(), localIdx.end());
    }

    m_beamIndexCount = static_cast<GLsizei>(beamIdx.size());

    glGenVertexArrays(1, &m_beamVAO);
    glBindVertexArray(m_beamVAO);

    glGenBuffers(1, &m_beamVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_beamVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 beamVerts.size() * sizeof(GLfloat),
                 beamVerts.data(),
                 GL_STATIC_DRAW);

    glGenBuffers(1, &m_beamEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_beamEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 beamIdx.size() * sizeof(GLuint),
                 beamIdx.data(),
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    glBindVertexArray(0);
}

// ===================================================
// 2) initFanGeometry(): HUB + ŁOPATKI (wspólna geometria)
// ===================================================
void D3widget::initFanGeometry()
{
    // -------- 2a) PIĄSTA (hub) – cylinder w (0,0,0) --------
    const float rHub       = 0.05f;
    const float hHub       = 0.02f;
    const int   hubSectors = 20;
    const int   hubStacks  = 1;

    const float yHubBottom = -hHub * 0.5f;
    const float yHubTop    = +hHub * 0.5f;

    std::vector<GLfloat> hubVerts;
    std::vector<GLuint>  hubIdx;

    for (int i = 0; i <= hubStacks; ++i) {
        float t = float(i) / float(hubStacks);
        float y = yHubBottom + t * (yHubTop - yHubBottom);
        for (int j = 0; j <= hubSectors; ++j) {
            float phi = float(j) * 2.0f * M_PI / float(hubSectors);
            float x = rHub * std::cos(phi);
            float z = rHub * std::sin(phi);
            float r_col = 1.0f, g_col = 0.0f, b_col = 0.0f;
            hubVerts.push_back(x);
            hubVerts.push_back(y);
            hubVerts.push_back(z);
            hubVerts.push_back(r_col);
            hubVerts.push_back(g_col);
            hubVerts.push_back(b_col);
        }
    }
    for (int i = 0; i < hubStacks; ++i) {
        for (int j = 0; j < hubSectors; ++j) {
            int first  = i       * (hubSectors + 1) + j;
            int second = (i + 1) * (hubSectors + 1) + j;
            hubIdx.push_back(first);
            hubIdx.push_back(second);
            hubIdx.push_back(first + 1);
            hubIdx.push_back(first + 1);
            hubIdx.push_back(second);
            hubIdx.push_back(second + 1);
        }
    }
    m_hubIndexCount = static_cast<GLsizei>(hubIdx.size());

    glGenVertexArrays(1, &m_hubVAO);
    glBindVertexArray(m_hubVAO);

    glGenBuffers(1, &m_hubVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_hubVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 hubVerts.size() * sizeof(GLfloat),
                 hubVerts.data(),
                 GL_STATIC_DRAW);

    glGenBuffers(1, &m_hubEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hubEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 hubIdx.size() * sizeof(GLuint),
                 hubIdx.data(),
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    glBindVertexArray(0);

    // ------ 2b) ŁOPATKI (blades): 4 prostokąty w Y=0 ------
    const float bladeLength = 0.4f;
    const float bladeWidth  = 0.05f;
    const int   bladeCount  = 4;

    std::vector<GLfloat> bladesVerts;
    std::vector<GLuint>  bladesIdx;
    int baseVertex = 0;

    for (int k = 0; k < bladeCount; ++k) {
        float angle = float(k) * (M_PI / 2.0f);
        float cosA  = std::cos(angle);
        float sinA  = std::sin(angle);

        float rHubLocal     = 0.05f;
        std::array<std::array<float,3>,4> localPos = {{
            { rHubLocal,      0.0f, -bladeWidth * 0.5f },
            { bladeLength,    0.0f, -bladeWidth * 0.5f },
            { bladeLength,    0.0f,  bladeWidth * 0.5f },
            { rHubLocal,      0.0f,  bladeWidth * 0.5f }
        }};
        float r_col = 0.0f, g_col = 0.0f, b_col = 1.0f;

        for (int vi = 0; vi < 4; ++vi) {
            float x0 = localPos[vi][0];
            float z0 = localPos[vi][2];
            float x1 = x0 * cosA - z0 * sinA;
            float z1 = x0 * sinA + z0 * cosA;
            bladesVerts.push_back(x1);
            bladesVerts.push_back(0.0f);
            bladesVerts.push_back(z1);
            bladesVerts.push_back(r_col);
            bladesVerts.push_back(g_col);
            bladesVerts.push_back(b_col);
        }

        bladesIdx.push_back(baseVertex + 0);
        bladesIdx.push_back(baseVertex + 1);
        bladesIdx.push_back(baseVertex + 2);
        bladesIdx.push_back(baseVertex + 0);
        bladesIdx.push_back(baseVertex + 2);
        bladesIdx.push_back(baseVertex + 3);

        baseVertex += 4;
    }

    m_bladesIndexCount = static_cast<GLsizei>(bladesIdx.size());

    glGenVertexArrays(1, &m_bladesVAO);
    glBindVertexArray(m_bladesVAO);

    glGenBuffers(1, &m_bladesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_bladesVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 bladesVerts.size() * sizeof(GLfloat),
                 bladesVerts.data(),
                 GL_STATIC_DRAW);

    glGenBuffers(1, &m_bladesEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bladesEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 bladesIdx.size() * sizeof(GLuint),
                 bladesIdx.data(),
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    glBindVertexArray(0);
}

// ===================================================
// 3) paintGL(): rysowanie kapsuły, belek i wiatraczków
// ===================================================
void D3widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // -------------------------------
    // 1) Macierz „view”
    // -------------------------------
    QMatrix4x4 view;
    view.translate(0, -0.5, -5.3);
    view.rotate(m_pitch * 180.0f / M_PI, -1, 0, 0);
    view.rotate(m_roll  * 180.0f / M_PI, 0, 0, 1);

    // -------------------------------
    // 2) Rysujemy kapsułę (m_vao)
    // -------------------------------
    {
        QMatrix4x4 mvpCapsule = m_projection * view;
        m_program.bind();
        m_program.setUniformValue("u_mvp", mvpCapsule);

        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        m_program.release();
    }

    // -------------------------------
    // 3) Rysujemy belki (m_beamVAO)
    // -------------------------------
    {
        QMatrix4x4 mvpBeam = m_projection * view;
        m_program.bind();
        m_program.setUniformValue("u_mvp", mvpBeam);

        glBindVertexArray(m_beamVAO);
        glDrawElements(GL_TRIANGLES, m_beamIndexCount, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        m_program.release();
    }

    // ----------------------------------------------------------------
    // 4) Rysujemy 4 wiatraczki (hub + łopatki), każdy trochę nad belką
    // ----------------------------------------------------------------
    const float R             = 0.5f;
    const float beamLength    = 0.6f;
    const float beamThickness = 0.05f;
    const float halfT         = beamThickness * 0.5f;
    const float rHub          = 0.05f;
    const float hHub          = 0.02f;

    // Obliczamy prędkość obrotu w zależności od roll i pitch:
    // baseSpeed + factor * (|roll| + |pitch|)
    float baseSpeed = 0.02f;
    float accelFactor = 0.01f;
    float tilt = std::fabs(m_roll) + std::fabs(m_pitch);
    float deltaAngle = baseSpeed + accelFactor * tilt;

    // Odległość od środka do środka hubu:
    float fanRadial = (R + beamLength) - (rHub * 0.5f);
    // Wysokość hubu tuż nad belką:
    float centerHubY = halfT + (hHub * 0.5f);

    for (int k = 0; k < 4; ++k) {
        // Dla dwóch wiatraczków obrót zgodny, dla dwóch przeciwny:
        // pozwalamy co drugi (k%2==0) mieć plus, a pozostałe minus
        float direction = (k % 2 == 0) ? +1.0f : -1.0f;
        float angleRad = M_PI / 4.0f + float(k) * (M_PI / 2.0f);
        float dx = std::cos(angleRad);
        float dz = std::sin(angleRad);

        float fx = fanRadial * dx;
        float fz = fanRadial * dz;
        float fy = centerHubY;

        float fanRotation = direction * m_fanAngle;

        // ----- 4a) Rysujemy HUB -----
        {
            QMatrix4x4 modelHub = view;
            modelHub.translate(fx, fy, fz);
            modelHub.rotate(fanRotation * 180.0f / M_PI, 0.0f, 1.0f, 0.0f);
            QMatrix4x4 mvpHub = m_projection * modelHub;

            m_program.bind();
            m_program.setUniformValue("u_mvp", mvpHub);

            glBindVertexArray(m_hubVAO);
            glDrawElements(GL_TRIANGLES, m_hubIndexCount, GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);

            m_program.release();
        }

        // ----- 4b) Rysujemy ŁOPATKI -----
        {
            QMatrix4x4 modelBlades = view;
            modelBlades.translate(fx, fy, fz);
            modelBlades.rotate(fanRotation * 180.0f / M_PI, 0.0f, 1.0f, 0.0f);
            QMatrix4x4 mvpBlades = m_projection * modelBlades;

            m_program.bind();
            m_program.setUniformValue("u_mvp", mvpBlades);

            glBindVertexArray(m_bladesVAO);
            glDrawElements(GL_TRIANGLES, m_bladesIndexCount, GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);

            m_program.release();
        }
    }

    // Zaktualizuj kąt obrotu:
    m_fanAngle += deltaAngle;
    if (m_fanAngle > 2.0f * M_PI) m_fanAngle -= 2.0f * M_PI;
}

void D3widget::timerEvent(QTimerEvent* /*event*/)
{
    update(); // Wywoła paintGL(), gdzie m_fanAngle jest aktualizowany
}
void D3widget::resizeGL(int w, int h){
    float aspect = float(w)/float(h? h:1);
    m_projection.setToIdentity();
    m_projection.perspective(45.0f, aspect, 0.1f, 100.0f);
}
void D3widget::setRotationAngles(float pitch, float roll){
    m_pitch = pitch;
    m_roll  = roll;
    update();  // wymuś repaint
}
