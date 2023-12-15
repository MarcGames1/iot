#include "Shader.h"


class Object {
public:
    Object(const std::string& fileName, int x, int y, int width, int height);
    ~Object() = default;
    void render(const Shader& shader);
    int getX() const;
    int getY() const;
    int getWidth() const;
    int getHeight() const;
    GLuint getTexture() const;

    void GenerateColorTexture(glm::vec3 color, int size);
    void GenerateTexture(std::string filePath);

protected:
    void load(const std::string& fileName);
    void renderScene(const Shader& shader);

    unsigned int VAO, VBO[3], EBO;
    unsigned int indexCount;
    int x, y, width, height;
    GLuint texture;

    static unsigned int textureCount;
};