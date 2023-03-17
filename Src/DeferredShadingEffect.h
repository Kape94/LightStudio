#ifndef _DEFERRED_SHADING_EFFECT_H
#define _DEFERRED_SHADING_EFFECT_H

#include "Buffer.h"
#include "IEffect.h"
#include "Shader.h"

class DeferredShadingEffect : public IEffect {
  public:

    void Create() override;

    void Cleanup() override;

    void Use(
      const LightModel& model,
      const Camera& camera
    ) override;

    void PostProcess(
      const LightModel& model,
      const Camera& camera
    ) override;

    std::string Name() const override;

  private:

    void CreateGBuffer();

    void CreateGBufferTextures();

    unsigned CreateGBufferTexture();

    void CreateDepthRenderBuffer();

    void CreateFrameBuffer();

    void CreateShaders();

    void CreateQuadGeometryBuffer();

    unsigned gBuffer;

    unsigned gPosition;
    unsigned gNormal;
    unsigned gAmbient;
    unsigned gDiffuse;
    unsigned gSpecular;

    unsigned depthRenderBuffer;

    Shader firstPassShader;
    Shader secondPassShader;
    Buffer quadBuffer;
};

#endif
