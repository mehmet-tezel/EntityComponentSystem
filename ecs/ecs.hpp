#include <memory>
#include <vector>
#include <unordered_map>
#include <optional>

// Forward declarations
class Component;
class Entity;
class EntityManager;

// Smart pointer vector to hold components
using ComponentPtr = std::shared_ptr<Component>;
using ComponentArray = std::vector<ComponentPtr>;

// Type definition for groups
using GroupId = size_t;
using EntityList = std::shared_ptr<std::vector<std::shared_ptr<Entity>>>;

// Main class for components
class Component {
public:
    virtual ~Component() {}
};

// Class representing entities
class Entity {
public:
    // Adds a specific component to the object
    template <typename T, typename... Args>
    void AddComponent(Args&&... args)
    {
        size_t typeId = typeid(T).hash_code();
        components[typeId] = std::make_shared<T>(std::forward<Args>(args)...);
    }

    // Simply removes the component
    template <typename T> 
    void RemoveComponent()
    {
        size_t typeId = typeid(T).hash_code();
        components.erase(typeId);
    }

    // Returns an existing component that has been created
    template <typename T>
    std::shared_ptr<T> GetComponent() const
    {
        size_t typeId = typeid(T).hash_code();
        auto it = components.find(typeId);
        if (it != components.end()) {
            return std::dynamic_pointer_cast<T>(it->second);
        }
        return nullptr;
    }

    ~Entity()
    {
        components.clear(); // Deletes all created components from memory
    }

private:
    std::unordered_map<size_t, ComponentPtr> components;
};

// Class responsible for entity management
class EntityManager {
public:
    // Creates entity for the system in an organised way
    std::shared_ptr<Entity> CreateEntity()
    {
        auto entity = std::make_shared<Entity>();
        return entity;
    }

    // Safely deletes the entity from memory, this function can be used when the entity dies or is to be removed from the programme stream
    void DestroyEntity(std::shared_ptr<Entity> entity)
    {
        entities.erase(
            std::remove(entities.begin(), entities.end(), entity), entities.end()
        );
    }

    ~EntityManager()
    {
        entities.clear(); // Deletes all created entities (objects) from memory
    }

private:
    std::vector<std::shared_ptr<Entity>> entities;
};

// This class is for grouping entities and is used to manage entities in groups
class GroupManager {
public:
    // Simply creates group
    template <typename... Components>
    GroupId CreateGroup()
    {
        GroupId groupId = nextGroupId;
        nextGroupId++;
        groups[groupId] = std::make_shared<std::vector<std::shared_ptr<Entity>>>();
        return groupId;
    }

    // Adds entity to the group
    void AddToGroup(GroupId groupId, std::shared_ptr<Entity> entity)
    {
        auto it = groups.find(groupId);
        if (it != groups.end()) {
            it->second->push_back(entity);
        }
    }

    // Removes entity from the group
    void RemoveFromGroup(GroupId groupId, std::shared_ptr<Entity> entity)
    {
        auto it = groups.find(groupId);
        if (it != groups.end()) {
            auto& entities = *(it->second);
            entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
        }
    }

    // Returns a vector that has all entities in the group
    template <typename... Components>
    std::vector<std::shared_ptr<Entity>> GetEntitiesInGroup(GroupId groupId)
    {
        auto it = groups.find(groupId);
        if (it != groups.end()) {
            return *(it->second);
        }

        return {};
    }

    // It takes all groups of entities
    std::unordered_map<GroupId, EntityList>& GetAllGroups()
    {
        return groups;
    }

    ~GroupManager()
    {
        groups.clear(); // Deletes all created groups from memory
    }

private:
    std::unordered_map<GroupId, EntityList> groups;
    GroupId nextGroupId = 1; // Initial group ID
};

// Returns the value of a specific component type
template <typename T> std::optional<T> GetComponentValue(std::shared_ptr<Entity> entity)
{
    auto component = entity->GetComponent<T>();
    if (component) {
        return *component; // Returns std::optional directly
    }

    return std::nullopt; // Returns null if no component is present
}