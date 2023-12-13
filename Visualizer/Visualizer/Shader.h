#include "Common.h"

class Shader
{
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    void Use() const;
    unsigned int GetID() const;

    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVec3(const std::string& name, const glm::vec3& value) const;
    void SetVec3(const std::string& name, float x, float y, float z) const;
    void SetMat4(const std::string& name, const glm::mat4& mat) const;

private:
    void Init(const char* vertexPath, const char* fragmentPath);
    void CheckCompileErrors(unsigned int shaderStencilTesting, std::string type);


private:
    unsigned int ID;
    unsigned int loc_model_matrix;
    unsigned int loc_view_matrix;
    unsigned int loc_projection_matrix;
};
