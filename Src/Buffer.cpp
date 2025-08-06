#include "Buffer.h"

#include <GL/glew.h>

//-----------------------------------------------------------------------------

void Buffer::Create(
  const float* vertexData, 
  const unsigned nVertices,
  const unsigned* indexData,
  const unsigned nIndices,
  const std::initializer_list<DataSpecification>& dataDescription
)
{
  ClearCurrent();

  LoadDataInGPU(vertexData, nVertices, indexData, nIndices);
  LinkShaderAttributes(dataDescription);

  this->nIndices = nIndices;
}

//-----------------------------------------------------------------------------

void Buffer::LoadDataInGPU(
  const float* vertexData, 
  const unsigned nVertices,
  const unsigned* indexData,
  const unsigned nIndices
)
{
  GenerateVertexArray();

  LoadVertexData(vertexData, nVertices);
  LoadIndexData(indexData, nIndices);
}

//-----------------------------------------------------------------------------

void Buffer::GenerateVertexArray()
{
  unsigned newVAO;
  glGenVertexArrays(1, &newVAO);
  glBindVertexArray(newVAO);

  this->VAO = newVAO;
}

//-----------------------------------------------------------------------------

void Buffer::LoadVertexData(
  const float* vertexData,
  const unsigned nVertices
) 
{
  unsigned newVBO;
  glGenBuffers(1, &newVBO);
  glBindBuffer(GL_ARRAY_BUFFER, newVBO);

  glBufferData(
    GL_ARRAY_BUFFER, 
    nVertices * sizeof(float), 
    vertexData, 
    GL_STATIC_DRAW
  );

  this->VBO = newVBO;
}

//-----------------------------------------------------------------------------

void Buffer::LoadIndexData(
  const unsigned* indexData,
  const unsigned nIndices
)
{
  unsigned newEBO;
  glGenBuffers(1, &newEBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newEBO);

  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER, 
    nIndices * sizeof(unsigned), 
    indexData, 
    GL_STATIC_DRAW
  );

  this->EBO = newEBO;
}

//-----------------------------------------------------------------------------

void Buffer::LinkShaderAttributes(
  const std::initializer_list<DataSpecification>& dataDescription
)
{
  const unsigned totalStride = CalculateTotalStride(dataDescription);

  unsigned currentStride = 0;
  for (const auto& dataSpec : dataDescription) {
    const unsigned attributeLocation = dataSpec.shaderAttributeLocation;
    const unsigned nFloats = dataSpec.nFloats;
    LinkAttribute(attributeLocation, nFloats, currentStride, totalStride);

    currentStride += dataSpec.nFloats;
  }
}

//-----------------------------------------------------------------------------

void Buffer::LinkAttribute(
  const unsigned attributeLocation,
  const unsigned nFloats,
  const unsigned initialStride,
  const unsigned totalStride
) const
{
  glVertexAttribPointer(
    attributeLocation,
    nFloats, 
    GL_FLOAT/*dataType*/,
    GL_FALSE/*isNormalized*/, 
    totalStride,
    (void*)(initialStride * sizeof(float))
  );
  
  glEnableVertexAttribArray(attributeLocation);
}

//-----------------------------------------------------------------------------

unsigned Buffer::CalculateTotalStride(
  const std::initializer_list<DataSpecification>& dataDescription
) const
{
  unsigned totalStride = 0;
  for (const auto& dataSpec : dataDescription) {
    totalStride += dataSpec.nFloats * sizeof(float);
  }
  return totalStride;
}

//-----------------------------------------------------------------------------

void Buffer::Delete()
{
  ClearCurrent();
}

//-----------------------------------------------------------------------------

void Buffer::ClearCurrent()
{
  if (VBO != 0) {
    glDeleteBuffers(1, &VBO); VBO = 0;
  }
  if (EBO != 0) {
    glDeleteBuffers(1, &EBO); EBO = 0;
  }
  if (VAO != 0) {
    glDeleteVertexArrays(1, &VAO); VAO = 0;
  }
  nIndices = 0;
}

//-----------------------------------------------------------------------------

void Buffer::Render()
{
  glBindVertexArray(this->VAO);
  glDrawElements(GL_TRIANGLES, this->nIndices, GL_UNSIGNED_INT, 0);
}

//-----------------------------------------------------------------------------
