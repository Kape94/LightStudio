#ifndef _BUFFER_H
#define _BUFFER_H

#include <initializer_list>

class Buffer {
  public:

    struct DataSpecification {
      unsigned nFloats = 0;
      unsigned shaderAttributeLocation = 0;
    };

    void Create(
      const float* vertexData,
      const unsigned nVertices,
      const unsigned* indexData,
      const unsigned nIndices,
      const std::initializer_list<DataSpecification>& dataDescription
    );

    void Delete();

    void Render();

  private:

    void ClearCurrent();

    void LoadDataInGPU(
      const float* vertexData,
      const unsigned nVertices,
      const unsigned* indexData,
      const unsigned nIndices
    );

    void GenerateVertexArray();

    void LoadVertexData(
      const float* vertexData,
      const unsigned nVertices
    );

    void LoadIndexData(
      const unsigned* indexData,
      const unsigned nIndices
    );

    void LinkShaderAttributes(
      const std::initializer_list<DataSpecification>& dataDescription
    );

    void LinkAttribute(
      const unsigned attributeLocation, 
      const unsigned nFloats, 
      const unsigned initialStride,
      const unsigned totalStride
    ) const;

    unsigned CalculateTotalStride(
      const std::initializer_list<DataSpecification>& dataDescription
    ) const;

    unsigned nIndices = 0;

    unsigned VAO = 0;
    unsigned VBO = 0;
    unsigned EBO = 0;
};

#endif
