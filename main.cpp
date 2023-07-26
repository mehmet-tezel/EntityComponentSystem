#include "components.hpp"

#include <iostream>

// Group example, updates entities position and prints
void Example1();

// Group example, updates entities position, health, collision data and prints
void Example2();

int main() {

    Example2();

    return 0;
}

void Example1()
{
    EntityManager entityManager;

    // Creating entities
    auto entity1 = entityManager.CreateEntity();
    auto entity2 = entityManager.CreateEntity();
    auto entity3 = entityManager.CreateEntity();
    auto entity4 = entityManager.CreateEntity();

    // Adding components to entities
    entity1->AddComponent<PositionComponent>(0, 0);
    entity1->AddComponent<VelocityComponent>(1, 1);

    entity2->AddComponent<PositionComponent>(10, 10);
    entity2->AddComponent<VelocityComponent>(-1, -1);

    entity3->AddComponent<PositionComponent>(5, 5);
    entity3->AddComponent<VelocityComponent>(2, 2);

    entity4->AddComponent<PositionComponent>(9, 9);
    entity4->AddComponent<VelocityComponent>(-2, -2);

    MovementSystem movementSystem;

    GroupManager groupManager;

    // Creating groups
    GroupId group1 = groupManager.CreateGroup<PositionComponent, VelocityComponent>();
    GroupId group2 = groupManager.CreateGroup<PositionComponent, VelocityComponent>();

    // Adding entities to groups
    groupManager.AddToGroup(group1, entity1);
    groupManager.AddToGroup(group1, entity2);

    groupManager.AddToGroup(group2, entity3);
    groupManager.AddToGroup(group2, entity4);

    for (int i = 0; i < 5; ++i) {
        std::cout << "------- Turn " << i + 1 << " -------\n";

        // Processing entities in group1 and group2
        /* Group 1 */ auto entitiesInGroup1 = groupManager.GetEntitiesInGroup<PositionComponent, VelocityComponent>(group1);
            for (const auto& entity : entitiesInGroup1) {
                movementSystem.Process(entity);
            }

        /* Group 2 */ auto entitiesInGroup2 = groupManager.GetEntitiesInGroup<PositionComponent, VelocityComponent>(group2);
            for (const auto& entity : entitiesInGroup2) {
                movementSystem.Process(entity);
            }
        // Processing entities in group1 and group2

        // Prints entity information
        for (const auto& entity : entitiesInGroup1) {
            auto pos = entity->GetComponent<PositionComponent>(); // Get position data
            std::cout << "Group 1 - Entity Position: (" << pos->x << ", " << pos->y << ")\n";
        }

        for (const auto& entity : entitiesInGroup2) {
            auto pos = entity->GetComponent<PositionComponent>(); // Get position data
            std::cout << "Group 2 - Entity Position: (" << pos->x << ", " << pos->y << ")\n";
        }

        std::cout << '\n';
    }
}

void Example2()
{
    EntityManager entityManager;

    // Creating entities
    auto entity1 = entityManager.CreateEntity();
    auto entity2 = entityManager.CreateEntity();

    // Adding components to entities
    entity1->AddComponent<PositionComponent>(0, 0);
    entity1->AddComponent<VelocityComponent>(1, 1);
    entity1->AddComponent<HealthComponent>(100);

    entity2->AddComponent<PositionComponent>(10, 10);
    entity2->AddComponent<VelocityComponent>(-1, -1);
    entity2->AddComponent<HealthComponent>(100);

    MovementSystem movementSystem;
    CollisionSystem collisionSystem;

    GroupManager groupManager;

    // Creating groups
    GroupId group1 = groupManager.CreateGroup<PositionComponent, VelocityComponent, HealthComponent>();
    GroupId group2 = groupManager.CreateGroup<PositionComponent, VelocityComponent, HealthComponent>();

    // Adding entities to groups
    groupManager.AddToGroup(group1, entity1);
    groupManager.AddToGroup(group2, entity2);

    for (int i = 0; i < 5; ++i) {
        std::cout << "------- Turn " << i + 1 << " -------\n";

        // Processing entities in group1 and group2
        /* Group 1 */ auto entitiesInGroup1 = groupManager.GetEntitiesInGroup<PositionComponent, VelocityComponent, HealthComponent>(group1);
        for (const auto& entity : entitiesInGroup1) {
            movementSystem.Process(entity);
        }

        /* Group 2 */ auto entitiesInGroup2 = groupManager.GetEntitiesInGroup<PositionComponent, VelocityComponent, HealthComponent>(group2);
        for (const auto& entity : entitiesInGroup2) {
            movementSystem.Process(entity);
        }
        // Processing entities in group1 and group2

        // Collision process
        // NOTE: This section is temporarily created to test certain scenarios. 
        // For more complex and multi-entity projects, a more comprehensive collision system needs to be written.
        for (const auto& entity1 : entitiesInGroup1) {
            for (const auto& entity2 : entitiesInGroup2) {
                collisionSystem.Process(entity1, entity2);
            }
        }

        // Prints entity information
        for (const auto& entity : entitiesInGroup1) {
            auto pos = entity->GetComponent<PositionComponent>(); // Get position data 
            auto health = entity->GetComponent<HealthComponent>(); // Get health data
            std::cout << "Group 1 - Entity Position: (" << pos->x << ", " << pos->y << "), Health: " << health->health << '\n';
        }

        for (const auto& entity : entitiesInGroup2) {
            auto pos = entity->GetComponent<PositionComponent>(); // Get position data
            auto health = entity->GetComponent<HealthComponent>(); // Get health data
            std::cout << "Group 2 - Entity Position: (" << pos->x << ", " << pos->y << "), Health: " << health->health << '\n';
        }

        std::cout << '\n';
    }
}