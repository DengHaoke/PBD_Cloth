#pragma once
#ifndef Node_h
#define Node_h

#include "vector.h"

struct Node
{
    bool movable;
    float mass;
    float invMass;
    Vec3 pos;
    Vec3 nextPos;
    Vec3 force;
    Vec3 velocity;
    Vec3 normal;

    Node(Vec3 pos) : pos(pos), nextPos(pos), force(Vec3(0, 0, 0)), velocity(Vec3(0, 0, 0)), mass(1.0), invMass(1.0 / mass), movable(true), normal(Vec3(0, 0, 0)) {}
    Node() {}

    void addForce(Vec3 f)
    {
        force += f;
    }

    // Given the equation "F = M * a" the next position is found through verlet integration
    void timeStep(float damping, float timestep)
    {
        velocity += force * timestep * invMass;
        velocity = velocity * damping;
        nextPos = pos + velocity * timestep;


    }
};
#endif