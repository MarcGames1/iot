#include "Shader.h"


class Object {
public:
    Object(const std::string& fileName, int x, int y, int width, int height);
    void render(const Shader& shader);
    int getX() const;
    int getY() const;
    int getWidth() const;
    int getHeight() const;
    GLuint getTexture() const;
protected:
    void load(const std::string& fileName);

    void generateColorTexture(GLuint& texture, glm::vec3 color, int size);
    void renderScene(const Shader& shader);

    unsigned int VAO, VBO[3], EBO;
    unsigned int indexCount;
    int x, y, width, height;
    GLuint texture;
};