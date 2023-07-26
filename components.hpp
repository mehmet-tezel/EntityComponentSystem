#include "ecs/ecs.hpp"

/*
* [WARNING!] This header file has been created for the sole purpose of testing the Entity Component System. 
* This file has no connection with the main project. The components in this file were created to test the system in the header file named "ecs.hpp". 
* When using this system, you need to create different components depending on the project you have made. 
* The type and number of components will vary according to your project requirements.
*/

// Test component for examples. Used in example1, example2
struct PositionComponent : public Component {
    int x;
    int y;

    PositionComponent(int x_, int y_) : x(x_), y(y_) {}
};

// Test component for examples. Used in example1, example2
struct VelocityComponent : public Component {
    int dx;
    int dy;

    VelocityComponent(int dx_, int dy_) : dx(dx_), dy(dy_) {}
};

// Test system for components. Used in example1, example2
class MovementSystem {
public:
    void Process(std::shared_ptr<Entity> entity)
    {
        auto position = entity->GetComponent<PositionComponent>();
        auto velocity = entity->GetComponent<VelocityComponent>();
        if (position && velocity) {
            position->x += velocity->dx;
            position->y += velocity->dy;
        }
    }
};

// Test component for examples. Used in example2
struct HealthComponent : public Component {
    int health;

    HealthComponent(int initialHealth) : health(initialHealth) {}
};

// Test system for components. Used in example2
class CollisionSystem {
public:
    void Process(std::shared_ptr<Entity> entity1, std::shared_ptr<Entity> entity2) {
        auto pos1 = entity1->GetComponent<PositionComponent>();
        auto pos2 = entity2->GetComponent<PositionComponent>();

        if (CheckCollision(pos1, pos2)) {

            auto health1 = entity1->GetComponent<HealthComponent>();
            auto health2 = entity2->GetComponent<HealthComponent>();

            if (health1 && health2) {
                health1->health -= 10;
                health2->health -= 10;
            }
        }
    }

private:
    bool CheckCollision(const std::shared_ptr<PositionComponent>& pos1, const std::shared_ptr<PositionComponent>& pos2) {
        return (pos1->x == pos2->x) && (pos1->y == pos2->y);
    }
};