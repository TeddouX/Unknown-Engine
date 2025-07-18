#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include <nlohmann/json.hpp>

#include "../entity.hpp"
#include "../component_manager.hpp"


// This is very badly written, so it may be subject 
// to radical changes, including deletion or a complete rewrite.
// My apologies to anyone that will have to read this
// and try to understand it.


using json = nlohmann::json;


class IComponent;


class ISerializable
{
public:
    virtual void Serialize(json &j) const = 0;
    virtual void Deserialize(const json &j) = 0;
};


/// @brief This class helps for component deserialization
class ComponentRegistry
{
public:
    using AddComponentFun = void(*)(Entity, ComponentManager &cm, std::shared_ptr<IComponent>);
    using ComponentDeserializerFun = std::shared_ptr<ISerializable>(*)(const json&);

    static ComponentRegistry &GetInstance();

    /// @brief Adds component from name to an entity
    void AddComponentFromName(const std::string &name, Entity e, ComponentManager &cm, std::shared_ptr<IComponent> comp) const;

    /// @brief Adds a component to the registry so it can be used when deserializing 
    template <typename T>
    bool RegisterComponent(const std::string &name);

    /// @brief Adds a component its serializer to the registry. 
    /// `RegisterComponent` is implicitly called.
    template <typename T>
    bool RegisterComponentDeserializer(const std::string &name);
     
    /// @return `true` if the registry has a deserilier for the 
    /// component of name `comp`
    bool HasComponentDeserializer(const std::string &comp) const;

    /// @returns The deserilizer function associated 
    /// with the component of name `comp`
    ComponentDeserializerFun GetDeserializer(const std::string &comp) const;

private:
    std::unordered_map<std::string, AddComponentFun> m_componentReg;
    std::unordered_map<std::string, ComponentDeserializerFun> m_componentDeserializerReg;
};


/// @brief Serializing components that inherit from ISerializable
void to_json(json &j, const std::shared_ptr<ISerializable> &ptr);


/// @brief Deserializing components that inherit from ISerializable
void from_json(const json &j, std::shared_ptr<ISerializable> &ptr);


#include "component_registry.inl"
