#ifndef _SHADER_H
#define _SHADER_H

class Shader {
  public:

    void Create(
      const char* vertexShaderCode, 
      const char* fragmentShaderCode
    );

    void Delete();

    void Use();

  private:

    unsigned id = 0;
};

#endif
