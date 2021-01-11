#pragma once
#ifndef Constraint_h
#define Constraint_h

#include <math.h>
#include "Node.h"
#include<iostream>

using namespace std;

struct Constraint
{
    Node* p1, * p2;
    float stiffness;
    float restLen;

    Constraint(Node* p1, Node* p2, float stiffness) : p1(p1), p2(p2)
    {
        Vec3 diffVec = p1->pos - p2->pos;
        this->stiffness = stiffness;
        restLen = diffVec.length();
    }

    void satisfyConstraint(int iterTimes)
    {
        float k = 1 - pow((1 - stiffness), 1.0 / iterTimes);
        //cout << 1/iterTimes << endl;

        Vec3 diffVec = p1->pos - p2->pos;
        float diffLen = diffVec.length();
        float w12 = p1->invMass + p2->invMass;
        diffVec.normalize();
        Vec3 correctVec = diffVec * (diffLen - restLen) / w12;
        Vec3 correctP1 = correctVec * -(p1->invMass);
        Vec3 correctP2 = correctVec * p2->invMass;

        p1->nextPos = p1->nextPos + correctP1 * k;
        p2->nextPos = p2->nextPos + correctP2 * k;
    }
};

#endif /* Constraint_h */

