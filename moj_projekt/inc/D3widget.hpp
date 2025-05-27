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
 * @brief Komponent renderujący prostopadłościan 3D z możliwością obrotu.
 */
class D3widget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

public:
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
     * @brief Ustawia kąty obrotu wokół osi X (pitch) i Y (roll) w radianach.
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

private:
    /**
     * @brief Inicjalizuje i kompiluje program shaderów.
     */
    void initShaders();

    /**
     * @brief Inicjalizuje geometrię prostopadłościanu (VAO, VBO, EBO).
     */
    void initGeometry();

    QOpenGLShaderProgram m_program; ///< Program shaderów (vertex + fragment)
    GLuint               m_vao;     ///< Vertex Array Object
    GLuint               m_vbo;     ///< Vertex Buffer Object
    GLuint               m_ebo;     ///< Element Buffer Object
    QMatrix4x4           m_projection; ///< Macierz projekcji perspektywicznej
    float                m_pitch;   ///< Kąt obrotu wokół osi X
    float                m_roll;    ///< Kąt obrotu wokół osi Y
};

#endif // D3WIDGET_H
