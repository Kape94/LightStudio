#include "DeferredShadingEffect.h"

#include <gl/glew.h>

#include <string>
#include <vector>

//---------------------------------------------------------------------------------------
// ShaderCodes
//---------------------------------------------------------------------------------------

namespace DeferredShadingCodes {
  namespace FirstPass {
    const char* vertex = R"(
      #version 330

      layout (location = 0) in vec3 vPosition;
      layout (location = 1) in vec3 vNormal;

      out vec3 fragPos;
      out vec3 normal;

      uniform mat4x4 viewMatrix;

      void main() 
      {
        fragPos = vPosition;
        normal = vNormal;

        gl_Position = viewMatrix * vec4(vPosition, 1.0);
      }
    )";

    const char* fragment = R"(
      #version 330

      struct Material {
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        float shininess;
      };

      uniform Material material;

      layout (location = 0) out vec3 gPosition;
      layout (location = 1) out vec3 gNormal;
      layout (location = 2) out vec3 gAmbient;
      layout (location = 3) out vec3 gDiffuse;
      layout (location = 4) out vec4 gSpecular;

      in vec3 fragPos;
      in vec3 normal;
    
      void main()
      {
        gPosition = fragPos;

        gNormal = normalize(normal);

        gAmbient = material.ambient;
        gDiffuse = material.diffuse;
        gSpecular.rgb = material.specular;
        gSpecular.a = material.shininess / 256.0f;
      }
    )";
  }

  namespace SecondPass {
    const char* vertex = R"(
      #version 330

      layout (location = 0) in vec3 pos;
      layout (location = 1) in vec2 texCoord;

      out vec2 fragTexCoord;

      void main()
      {
        fragTexCoord = texCoord;
        gl_Position = vec4(pos, 1.0);
      }
    )";

    const char* fragment = R"(
  #version 330

  struct DirectionalLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
  };

  struct PointLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
  
    vec3 position;
    float constant;
    float linear;
    float quadratic;
  };

  #define MAX_LIGHTS 4

  uniform DirectionalLight directionalLights[MAX_LIGHTS];
  uniform int nDirectionalLights;

  uniform PointLight pointLights[MAX_LIGHTS];
  uniform int nPointLights;

  uniform vec3 viewPosition;

  uniform sampler2D gPosition;
  uniform sampler2D gNormal;
  uniform sampler2D gAmbient;
  uniform sampler2D gDiffuse;
  uniform sampler2D gSpecular;

  in vec2 fragTexCoord;

  out vec4 fragColor;

  vec3 CalculateDirectionalLight();
  vec3 CalculatePointLight();

  void main()
  {
      vec3 directional = CalculateDirectionalLight();
      vec3 point = CalculatePointLight();
    
      vec3 result = directional + point;
	    fragColor = vec4(result, 1.0);
  }


  vec3 CalculateDirectionalLight() {
      vec3 fragPos = texture(gPosition, fragTexCoord).rgb;
      vec3 normal = texture(gNormal, fragTexCoord).rgb;

      vec3 materialAmbient = texture(gAmbient, fragTexCoord).rgb;
      vec3 materialDiffuse = texture(gDiffuse, fragTexCoord).rgb;
      vec3 materialSpecular = texture(gSpecular, fragTexCoord).rgb;
      float materialShininess = texture(gSpecular, fragTexCoord).a * 256.0f;

      vec3 viewDir = normalize(viewPosition - fragPos);

      vec3 result = vec3(0.0);
      int nLights = min(nDirectionalLights, MAX_LIGHTS);
      for (int i = 0; i < nLights; ++i) {
          DirectionalLight light = directionalLights[i];
        
	        vec3 lightDir = normalize(light.direction);
          vec3 invLightDir = -lightDir;
        
          float diff = max(dot(normal, invLightDir), 0.0);
        
          vec3 reflectDir = reflect(lightDir, normal);
          float spec = pow(max(dot(reflectDir, viewDir), 0.0), materialShininess);
        
          vec3 ambient = light.ambient * materialAmbient;
          vec3 diffuse = light.diffuse * (diff * materialDiffuse);
          vec3 specular = light.specular * (spec * materialSpecular);
        
          result += (ambient + diffuse + specular);
      }
      return result; 
  }

  vec3 CalculatePointLight() {
      vec3 fragPos = texture(gPosition, fragTexCoord).rgb;
      vec3 normal = texture(gNormal, fragTexCoord).rgb;

      vec3 materialAmbient = texture(gAmbient, fragTexCoord).rgb;
      vec3 materialDiffuse = texture(gDiffuse, fragTexCoord).rgb;
      vec3 materialSpecular = texture(gSpecular, fragTexCoord).rgb;
      float materialShininess = texture(gSpecular, fragTexCoord).a * 256.0f;

      vec3 viewDir = normalize(viewPosition - fragPos);

      vec3 result = vec3(0.0);
      int nLights = min(nPointLights, MAX_LIGHTS);
      for (int i = 0; i < nLights; ++i) {
          PointLight light = pointLights[i];
        
	        vec3 lightDir = normalize(fragPos - light.position);
          vec3 invLightDir = -lightDir;
        
          float diff = max(dot(invLightDir, normal), 0.0);
        
          vec3 reflectDir = reflect(lightDir, normal);
          float spec = pow( max(dot(reflectDir, viewDir), 0.0) , materialShininess );
        
          float distance = length(light.position - fragPos);
          float factor = light.constant + light.linear * distance + light.quadratic * (distance * distance);
          float attenuation = 1.0 / factor;
        
          vec3 ambient = (light.ambient * materialAmbient) * attenuation;
          vec3 diffuse = ( light.diffuse * (diff * materialDiffuse) ) * attenuation;
          vec3 specular = ( light.specular * (spec * materialSpecular) ) * attenuation;
        
          result += (ambient + diffuse + specular);
        
      }
      return result;
    }
    )";
  }
}

//---------------------------------------------------------------------------------------
// DeferredShadingEffect
//---------------------------------------------------------------------------------------

constexpr unsigned SCREEN_WIDTH = 1280;
constexpr unsigned SCREEN_HEIGHT = 960;

void DeferredShadingEffect::Create()
{
  glGenFramebuffers(1, &gBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

  glGenTextures(1, &gPosition);
  glBindTexture(GL_TEXTURE_2D, gPosition);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

  glGenTextures(1, &gNormal);
  glBindTexture(GL_TEXTURE_2D, gNormal);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

  glGenTextures(1, &gAmbient);
  glBindTexture(GL_TEXTURE_2D, gAmbient);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAmbient, 0);

  glGenTextures(1, &gDiffuse);
  glBindTexture(GL_TEXTURE_2D, gDiffuse);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gDiffuse, 0);

  glGenTextures(1, &gSpecular);
  glBindTexture(GL_TEXTURE_2D, gSpecular);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, gSpecular, 0);

  unsigned attachments[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
  glDrawBuffers(5, attachments);

  glGenRenderbuffers(1, &depthRenderBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    exit(-1);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  firstPassShader.Create(DeferredShadingCodes::FirstPass::vertex, DeferredShadingCodes::FirstPass::fragment);
  secondPassShader.Create(DeferredShadingCodes::SecondPass::vertex, DeferredShadingCodes::SecondPass::fragment);

  const std::vector<float> vertexData = {
    //x, y, z, u, v
    -1, -1, 0, 0, 0,
     1, -1, 0, 1, 0,
     1, 1, 0, 1, 1,
     -1, 1, 0, 0, 1
  };
  const std::vector<unsigned> indexData = {
    0, 1, 2,
    2, 3, 0
  };

  quadBuffer.Create(vertexData.data(), vertexData.size(), indexData.data(), indexData.size(), 
    { {3, 0/*position*/}, {2, 1/*uv*/} }
  );
}

//---------------------------------------------------------------------------------------

void DeferredShadingEffect::Cleanup()
{
  glDeleteFramebuffers(1, &gBuffer);

  glDeleteTextures(1, &gPosition);
  glDeleteTextures(1, &gNormal);
  glDeleteTextures(1, &gAmbient);
  glDeleteTextures(1, &gDiffuse);
  glDeleteTextures(1, &gSpecular);

  glDeleteRenderbuffers(1, &depthRenderBuffer);
}

//---------------------------------------------------------------------------------------

void DeferredShadingEffect::Use(
  const LightModel& model, 
  const Camera& camera
)
{
  glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  firstPassShader.Use();

  firstPassShader.SetUniform("viewMatrix", camera.CreateTransformationMatrix());

  const Material& material = model.GetMaterial();
  firstPassShader.SetUniform("material.ambient", material.ambient);
  firstPassShader.SetUniform("material.diffuse", material.diffuse);
  firstPassShader.SetUniform("material.specular", material.specular);
  firstPassShader.SetUniform("material.shininess", material.shininess);
}

//---------------------------------------------------------------------------------------

void DeferredShadingEffect::PostProcess(
  const LightModel& model,
  const Camera& camera
)
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glDisable(GL_DEPTH_TEST);

  glClear(GL_COLOR_BUFFER_BIT);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, gPosition);
  
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, gNormal);
  
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, gAmbient);

  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, gDiffuse);

  glActiveTexture(GL_TEXTURE4);
  glBindTexture(GL_TEXTURE_2D, gSpecular);

  secondPassShader.Use();

  secondPassShader.SetUniform("gPosition", 0);
  secondPassShader.SetUniform("gNormal", 1);
  secondPassShader.SetUniform("gAmbient", 2);
  secondPassShader.SetUniform("gDiffuse", 3);
  secondPassShader.SetUniform("gSpecular", 4);

  int nDirectionalLights = 0;
  int nPointLights = 0;

  for (size_t i = 0; i < 4; ++i) {
    const Light& light = model.GetLight(i);
    if (!light.enabled) {
      continue;
    }

    if (light.isDirectional) {
      const std::string iLight = "directionalLights[" + std::to_string(nDirectionalLights) + "].";
      secondPassShader.SetUniform(std::string(iLight + "ambient").c_str(), light.ambient);
      secondPassShader.SetUniform(std::string(iLight + "diffuse").c_str(), light.diffuse);
      secondPassShader.SetUniform(std::string(iLight + "specular").c_str(), light.specular);
      secondPassShader.SetUniform(std::string(iLight + "direction").c_str(), light.direction);

      ++nDirectionalLights;
    }
    else {
      const std::string iLight = "pointLights[" + std::to_string(nPointLights) + "].";
      secondPassShader.SetUniform(std::string(iLight + "ambient").c_str(), light.ambient);
      secondPassShader.SetUniform(std::string(iLight + "diffuse").c_str(), light.diffuse);
      secondPassShader.SetUniform(std::string(iLight + "specular").c_str(), light.specular);
      secondPassShader.SetUniform(std::string(iLight + "position").c_str(), light.position);
      secondPassShader.SetUniform(std::string(iLight + "quadratic").c_str(), light.quadratic);
      secondPassShader.SetUniform(std::string(iLight + "linear").c_str(), light.linear);
      secondPassShader.SetUniform(std::string(iLight + "constant").c_str(), light.constant);

      ++nPointLights;
    }
  }

  secondPassShader.SetUniform("nDirectionalLights", nDirectionalLights);
  secondPassShader.SetUniform("nPointLights", nPointLights);

  secondPassShader.SetUniform("viewPosition", camera.GetPosition());

  quadBuffer.Render();

  glEnable(GL_DEPTH_TEST);
}

//---------------------------------------------------------------------------------------

std::string DeferredShadingEffect::Name() const
{
  return "Deferred shading";
}

//---------------------------------------------------------------------------------------
