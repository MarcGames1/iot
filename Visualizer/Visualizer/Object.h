#include "Shader.h"


class Object {
public:
    Object(const std::string& fileName, int x, int y, int width, int height);
    ~Object() = default;
    void render(const Shader& shader);
    void render(const Shader& shader, glm::vec3 translate, glm::vec3 scale, glm::vec3 rotate);
    int getX() const;
    int getY() const;
    int getWidth() const;
    int getHeight() const;
    GLuint getTexture() const;

    void GenerateColorTexture(glm::vec3 color, int alpha, int size);
    void GenerateTexture(std::string filePath);

protected:
    void load(const std::string& fileName);

    unsigned int VAO, VBO[3], EBO;
    unsigned int indexCount;
    int x, y, width, height;
    GLuint texture;

    int valueSW, valueSE, valueN;
};