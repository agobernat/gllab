#pragma once
#include <vector>
#include "include/glm/glm.hpp"

struct Collider
{
    glm::dvec2 bottomleft;
    glm::dvec2 topright;
    glm::dvec2 transform;
    enum Type {
        Kid,
        Block,
        Spike
    };
    Type type;
    //glm::dvec2 size = glm::dvec2(1, 1);


    inline bool collWithOther(Collider& other) {

        
        if (bottomleft.x + transform.x < other.topright.x + other.transform.x &&
            topright.x + transform.x > other.bottomleft.x + other.transform.x &&
            bottomleft.y + transform.y < other.topright.y + other.transform.y &&
            topright.y + transform.y > other.bottomleft.y + other.transform.y)
        {
            return true;
        }
        return false;
    }

    inline void moveTo(glm::dvec2 pos) {
        transform = pos;
        
    }

    inline void printbounds() {
        std::cout << bottomleft.x + transform.x << " "
                  << bottomleft.y + transform.y << " "
                  << topright.x + transform.x << " "
                  << topright.y + transform.y << "\n";
    }

    inline static Collider* unitBox() {
        Collider* coll = new Collider();
        coll->bottomleft = glm::dvec2(0, 0);
        coll->topright = glm::dvec2(1, 1);
        return coll;
    }


};