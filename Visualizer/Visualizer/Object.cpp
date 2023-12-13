// Object.cpp
#include "Object.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Constructor that takes the file name of the 3D model and the viewport parameters
Object::Object(const std::string& fileName, int x, int y, int width, int height) 
{
    // Declare a color vector
    glm::vec3 color(0.5f, 0.2f, 0.8f); // a purple color
    // Declare a size variable
    int size = 256; // a 256x256 texture
    // Call the generateColorTexture method
    generateColorTexture(texture, color, size);

    // Generate the vertex array object, vertex buffer objects, and element buffer object
    glGenVertexArrays(1, &VAO);
    glGenBuffers(3, VBO);
    glGenBuffers(1, &EBO);

    // Load the 3D model from the file
    load(fileName);

    // Set the viewport parameters
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

// Render the object with the given view and projection matrices
void Object::render(const Shader& shader) {
    glm::mat4 model = glm::mat4();
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0));
    model = glm::scale(model, glm::vec3(0.75f));
    shader.SetMat4("model", model);

    // Bind the vertex array object
    glBindVertexArray(VAO);

    // Draw the model using the element buffer object
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    // Unbind the vertex array object
    glBindVertexArray(0);
}

// Get the viewport parameters
int Object::getX() const {
    return x;
}
int Object::getY() const {
    return y;
}
int Object::getWidth() const {
    return width;
}
int Object::getHeight() const {
    return height;
}

GLuint Object::getTexture() const
{
    return texture;
}


// Load the 3D model from the file
void Object::load(const std::string& fileName) {
    // Create an Assimp importer and read the file
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs);

    // Get the first mesh in the scene
    aiMesh* mesh = scene->mMeshes[0];

    // Calculate the number of indices
    indexCount = mesh->mNumFaces * 3;

    // Allocate memory for the vertex, normal, uv, and index arrays
    GLfloat* vertexArray = new GLfloat[mesh->mNumFaces * 3 * 3];
    GLfloat* normalArray = new GLfloat[mesh->mNumFaces * 3 * 3];
    GLfloat* uvArray = new GLfloat[mesh->mNumFaces * 3 * 2];
    GLuint* indexArray = new GLuint[mesh->mNumFaces * 3];

    // Copy the data from the mesh to the arrays
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        const aiFace& face = mesh->mFaces[i];
        for (int j = 0; j < 3; j++) {
            aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[j]];
            memcpy(uvArray, &uv, sizeof(float) * 2);
            uvArray += 2;

            aiVector3D normal = mesh->mNormals[face.mIndices[j]];
            memcpy(normalArray, &normal, sizeof(float) * 3);
            normalArray += 3;

            aiVector3D pos = mesh->mVertices[face.mIndices[j]];
            memcpy(vertexArray, &pos, sizeof(float) * 3);
            vertexArray += 3;

            indexArray[i * 3 + j] = face.mIndices[j];
        }
    }

    // Reset the pointers to the beginning of the arrays
    uvArray -= mesh->mNumFaces * 3 * 2;
    normalArray -= mesh->mNumFaces * 3 * 3;
    vertexArray -= mesh->mNumFaces * 3 * 3;

    // Bind the vertex array object
    glBindVertexArray(VAO);

    // Bind and fill the vertex buffer object for the positions
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->mNumFaces * 3, vertexArray, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    // Bind and fill the vertex buffer object for the normals
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->mNumFaces * 3, normalArray, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    // Bind and fill the vertex buffer object for the texture coordinates
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * mesh->mNumFaces * 3, uvArray, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(2);

    // Bind and fill the element buffer object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh->mNumFaces * 3, indexArray, GL_STATIC_DRAW);

    // Unbind the vertex array object
    glBindVertexArray(0);

    // Delete the arrays
    delete[] vertexArray;
    delete[] normalArray;
    delete[] uvArray;
    delete[] indexArray;
}

void Object::generateColorTexture(GLuint& text, glm::vec3 color, int size)
{
    // Generate and bind the texture
    glGenTextures(1, &text);
    glBindTexture(GL_TEXTURE_2D, text);

    // Set the texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Allocate memory for the texture data
    unsigned char* data = new unsigned char[3 * size * size];

    // Fill the texture data with the color
    for (int i = 0; i < size * size; i++) {
        data[i * 3] = (unsigned char)(color.x * 255.0f);
        data[i * 3 + 1] = (unsigned char)(color.y * 255.0f);
        data[i * 3 + 2] = (unsigned char)(color.z * 255.0f);
    }

    // Upload the texture data and generate mipmaps
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Delete the texture data
    delete[] data;
}

void Object::renderScene(const Shader& shader)
{
    glm::mat4 model = glm::mat4();
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0));
    model = glm::scale(model, glm::vec3(0.75f));
    shader.SetMat4("model", model);

    // Bind the vertex array object
    glBindVertexArray(VAO);

    // Draw the model using the element buffer object
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    // Unbind the vertex array object
    glBindVertexArray(0);
}
