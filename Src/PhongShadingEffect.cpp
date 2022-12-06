#include "PhongShadingEffect.h"

#include <string>

//---------------------------------------------------------------------------------------
// ShaderCodes
//---------------------------------------------------------------------------------------

namespace ShaderCodes {

  const char* vertexShaderCode = R"(
    #version 330

    layout (location = 0) in vec3 pos;
    layout (location = 1) in vec3 norm;

    out vec3 fragPos;
    out vec3 fragNorm;

    uniform mat4 cameraTransform;

    void main()
    {
      fragPos = pos;
      fragNorm = norm;
      gl_Position = cameraTransform * vec4(pos, 1.0);
    }
  )";

  const char* fragmentShaderCode = R"(
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

  struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
  };

  #define MAX_LIGHTS 4

  uniform Material material;

  uniform DirectionalLight directionalLights[MAX_LIGHTS];
  uniform int nDirectionalLights;

  uniform PointLight pointLights[MAX_LIGHTS];
  uniform int nPointLights;

  uniform vec3 viewPosition;

  in vec3 fragPos;
  in vec3 fragNorm;

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
      vec3 viewDir = normalize(viewPosition - fragPos);
    
      vec3 normal = normalize(fragNorm);

      vec3 result = vec3(0.0);
      int nLights = min(nDirectionalLights, MAX_LIGHTS);
      for (int i = 0; i < nLights; ++i) {
          DirectionalLight light = directionalLights[i];
        
	      vec3 lightDir = normalize(light.direction);
          vec3 invLightDir = -lightDir;
        
          float diff = max(dot(normal, invLightDir), 0.0);
        
          vec3 reflectDir = reflect(lightDir, normal);
          float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
        
          vec3 ambient = light.ambient * material.ambient;
          vec3 diffuse = light.diffuse * (diff * material.diffuse);
          vec3 specular = light.specular * (spec * material.specular);
        
          result += (ambient + diffuse + specular);
      }
      return result; 
  }

  vec3 CalculatePointLight() {
      vec3 viewDir = normalize(viewPosition - fragPos);
      vec3 normal = normalize(fragNorm);

      vec3 result = vec3(0.0);
      int nLights = min(nPointLights, MAX_LIGHTS);
      for (int i = 0; i < nLights; ++i) {
          PointLight light = pointLights[i];
        
	      vec3 lightDir = normalize(fragPos - light.position);
          vec3 invLightDir = -lightDir;
        
          float diff = max(dot(invLightDir, normal), 0.0);
        
          vec3 reflectDir = reflect(lightDir, normal);
          float spec = pow( max(dot(reflectDir, viewDir), 0.0) , material.shininess );
        
          float distance = length(light.position - fragPos);
          float factor = light.constant + light.linear * distance + light.quadratic * (distance * distance);
          float attenuation = 1.0 / factor;
        
          vec3 ambient = (light.ambient * material.ambient) * attenuation;
          vec3 diffuse = ( light.diffuse * (diff * material.diffuse) ) * attenuation;
          vec3 specular = ( light.specular * (spec * material.specular) ) * attenuation;
        
          result += (ambient + diffuse + specular);
        
      }
      return result;
  }
  )";

}

//---------------------------------------------------------------------------------------

void PhongShadingEffect::Create()
{
  shader.Create(ShaderCodes::vertexShaderCode, ShaderCodes::fragmentShaderCode);
}

//---------------------------------------------------------------------------------------

void PhongShadingEffect::Cleanup()
{
  shader.Delete();
}

//---------------------------------------------------------------------------------------

void PhongShadingEffect::Use(
  const LightModel& model,
  const Camera& camera
)
{
  const Material& material = model.GetMaterial();
  shader.SetUniform("material.ambient", material.ambient);
  shader.SetUniform("material.diffuse", material.diffuse);
  shader.SetUniform("material.specular", material.specular);
  shader.SetUniform("material.shininess", material.shininess);

  int nDirectionalLights = 0;
  int nPointLights = 0;

  for (size_t i = 0; i < 4; ++i) {
    const Light& light = model.GetLight(i);
    if (!light.enabled) {
      continue;
    }

    if (light.isDirectional) {
      const std::string iLight = "directionalLights[" + std::to_string(nDirectionalLights) + "].";
      shader.SetUniform(std::string(iLight + "ambient").c_str(), light.ambient);
      shader.SetUniform(std::string(iLight + "diffuse").c_str(), light.diffuse);
      shader.SetUniform(std::string(iLight + "specular").c_str(), light.specular);
      shader.SetUniform(std::string(iLight + "direction").c_str(), light.direction);

      ++nDirectionalLights;
    }
    else {
      const std::string iLight = "pointLights[" + std::to_string(nPointLights) + "].";
      shader.SetUniform(std::string(iLight + "ambient").c_str(), light.ambient);
      shader.SetUniform(std::string(iLight + "diffuse").c_str(), light.diffuse);
      shader.SetUniform(std::string(iLight + "specular").c_str(), light.specular);
      shader.SetUniform(std::string(iLight + "position").c_str(), light.position);
      shader.SetUniform(std::string(iLight + "quadratic").c_str(), light.quadratic);
      shader.SetUniform(std::string(iLight + "linear").c_str(), light.linear);
      shader.SetUniform(std::string(iLight + "constant").c_str(), light.constant);

      ++nPointLights;
    }
  }

  shader.SetUniform("nDirectionalLights", nDirectionalLights);
  shader.SetUniform("nPointLights", nPointLights);

  shader.SetUniform("viewPosition", camera.GetPosition());
  shader.SetUniform("cameraTransform", camera.CreateTransformationMatrix());

  shader.Use();
}

//---------------------------------------------------------------------------------------

std::string PhongShadingEffect::Name() const
{
  return "Phong lightning model";
}

//---------------------------------------------------------------------------------------
