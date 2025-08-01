#include "ecs/system/lighting_system.hpp"


void LightingSystem::Update(const ComponentManager &cm, float dt)
{
    const auto &dirLightsMap = cm.GetEntitiesWith<DirectionalLight>();

    for (auto &[entity, dirLight] : dirLightsMap)
        dirLight->rotation = cm.GetComponent<Transform>(entity)->GetWorldRot();

    std::vector<DirectionalLight::Data> dirLightsData;
    for (DirectionalLight dirLight : GetAllValues(dirLightsMap))
        dirLightsData.push_back(dirLight.GetData());

    this->UploadLights(dirLightsData, this->m_dirLightSSBO);
}
