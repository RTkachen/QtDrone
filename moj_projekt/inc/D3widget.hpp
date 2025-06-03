/**
 * @file D3widget.hpp
 * @brief Komponent OpenGL odpowiedzialny za wyświetlanie i obracanie obiektu 3D.
 *
 * Klasa D3widget wykorzystuje OpenGL (profil 3.3 Core) do renderowania prostopadłościanu
 * reprezentującego np. obiekt sterowany danymi z akcelerometru. Pozwala na ustawianie
 * kąta obrotu względem osi X i Y (pitch i roll), aktualizując odpowiednio transformacje
 * w przestrzeni 3D.
 */

#ifndef D3WIDGET_H
#define D3WIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>

/**
 * @class D3widget
 * @brief Komponent renderujący obiekty 3D (np. korpus drona wraz z belkami i wiatraczkami).
 */
class D3widget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

public:
    /**
     * @brief Inicjalizuje geometrię belki (słupka) używanej do montowania wiatraczków.
     */
    void initBeamGeometry();

    /**
     * @brief Inicjalizuje geometrię wiatraczków, czyli piasty oraz łopatek.
     */
    void initFanGeometry();

    /**
     * @brief Konstruktor klasy D3widget.
     * @param parent Rodzic widgetu (domyślnie nullptr).
     */
    explicit D3widget(QWidget* parent = nullptr);

    /**
     * @brief Destruktor wirtualny – zwalnia zasoby OpenGL.
     */
    ~D3widget() override;

    /**
     * @brief Ustawia kąty obrotu wokół osi X (pitch) oraz osi Y (roll) w radianach.
     * @param pitch Kąt obrotu wokół osi X.
     * @param roll  Kąt obrotu wokół osi Y.
     */
    void setRotationAngles(float pitch, float roll);

protected:
    /**
     * @brief Inicjalizacja kontekstu OpenGL – kompilacja shaderów i buforów.
     */
    void initializeGL() override;

    /**
     * @brief Przeskalowanie widoku w przypadku zmiany rozmiaru okna.
     * @param w Nowa szerokość.
     * @param h Nowa wysokość.
     */
    void resizeGL(int w, int h) override;

    /**
     * @brief Główna funkcja rysująca scenę 3D.
     */
    void paintGL() override;

    /**
     * @brief Obsługa zdarzenia timera do animacji.
     * @param event Wskaźnik do zdarzenia timera.
     */
    void timerEvent(QTimerEvent* event) override;

private:
    // -----------------------------------
    // Zmienne dla "słupka" (beam)
    // -----------------------------------
    GLuint  m_beamVAO   = 0;             ///< Vertex Array Object dla słupka (beam)
    GLuint  m_beamVBO   = 0;             ///< Vertex Buffer Object dla słupka (beam)
    GLuint  m_beamEBO   = 0;             ///< Element Buffer Object dla słupka (beam)
    GLsizei m_beamIndexCount = 0;        ///< Liczba indeksów dla geometrii słupka (beam)

    // -----------------------------------
    // Zmienne dla "wiatraczka" (hub + łopatki)
    // -----------------------------------
    // Piasta ("hub")
    GLuint  m_hubVAO    = 0;             ///< Vertex Array Object dla piasty wiatraczka (hub)
    GLuint  m_hubVBO    = 0;             ///< Vertex Buffer Object dla piasty wiatraczka (hub)
    GLuint  m_hubEBO    = 0;             ///< Element Buffer Object dla piasty wiatraczka (hub)
    GLsizei m_hubIndexCount = 0;         ///< Liczba indeksów dla geometrii piasty wiatraczka (hub)

    // Łopatki ("blades")
    GLuint  m_bladesVAO     = 0;         ///< Vertex Array Object dla łopatek wiatraczka (blades)
    GLuint  m_bladesVBO     = 0;         ///< Vertex Buffer Object dla łopatek wiatraczka (blades)
    GLuint  m_bladesEBO     = 0;         ///< Element Buffer Object dla łopatek wiatraczka (blades)
    GLsizei m_bladesIndexCount = 0;      ///< Liczba indeksów dla geometrii łopatek wiatraczka (blades)

    float   m_fanAngle = 0.0f;           ///< Aktualny kąt obrotu wiatraczka (prędkość animacji)
    GLsizei m_indexCount;                ///< Globalna liczba indeksów dla głównej geometrii (np. korpusu)

    /**
     * @brief Dodaje geometrię belki do globalnych wektorów wierzchołków i indeksów.
     * @param vertices Referencja do wektora wierzchołków.
     * @param indices Referencja do wektora indeksów.
     * @param offset Aktualny offset wierzchołków (liczba już dodanych wierzchołków).
     * @param start Początkowy punkt belki.
     * @param end Końcowy punkt belki.
     * @param width Szerokość belki.
     * @param thickness Grubość belki.
     */
    void addBeam(std::vector<GLfloat>& vertices,
                 std::vector<GLuint>& indices,
                 GLuint& offset,
                 const QVector3D& start,
                 const QVector3D& end,
                 float width,
                 float thickness);

    /**
     * @brief Inicjalizuje i kompiluje program shaderów.
     */
    void initShaders();

    /**
     * @brief Inicjalizuje geometrię głównego obiektu (korpusu) wraz z buforami.
     */
    void initGeometry();

    QOpenGLShaderProgram m_program; ///< Program shaderów (vertex + fragment)
    GLuint               m_vao;     ///< Vertex Array Object dla głównej geometrii
    GLuint               m_vbo;     ///< Vertex Buffer Object dla głównej geometrii
    GLuint               m_ebo;     ///< Element Buffer Object dla głównej geometrii
    QMatrix4x4           m_projection; ///< Macierz projekcji perspektywicznej
    float                m_pitch;   ///< Kąt obrotu wokół osi X (pitch)
    float                m_roll;    ///< Kąt obrotu wokół osi Y (roll)
};

#endif // D3WIDGET_H
