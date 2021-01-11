#pragma once
#ifndef cloth_h
#define cloth_h

//#include<gl/glew.h>
#include<vector>
#include "Node.h"
#include "Constraint.h"

#define CONSTRAINT_ITERATIONS 3 // 每帧约束满足的迭代

class mass_cloth
{
public:
    int pInRow; // “width”方向上的粒子数
    int pInCol; // “height”方向上的粒子数
    std::vector<Node> Nodes; // 这块布的所有粒子
    const float structualCoef = 0.4;
    const float shearCoef = 0.4;
    const float bendingCoef = 0.4;
    int drawMode;
    std::vector<Constraint> constraints; // 这块布上所有粒子间的约束

    enum DrawModeEnum {
        DRAW_MASS_NODES,
        DRAW_SPRINGS,
        DRAW_FACES
    };
    mass_cloth()
    {
    }
    Node* getNode(int x, int y) { return &Nodes[y * pInRow + x]; }

    Vec3 computeFaceNormal(Node* p1, Node* p2, Node* p3)
    {
        Vec3 v1 = p2->pos - p1->pos;
        Vec3 v2 = p3->pos - p1->pos;

        return v1.cross(v2);
    }

    void drawFace(Node* p1, Node* p2, Node* p3)
    {
        glNormal3fv((GLfloat*)&(p1->normal));
        glVertex3fv((GLfloat*)&(p1->pos));

        glNormal3fv((GLfloat*)&(p2->normal));
        glVertex3fv((GLfloat*)&(p2->pos));

        glNormal3fv((GLfloat*)&(p3->normal));
        glVertex3fv((GLfloat*)&(p3->pos));
    }

public:
    /* This is a important constructor for the entire system of Nodes and constraints*/
    mass_cloth(float width, float height, int pInRow, int pInCol)
    {
        this->pInRow = pInRow;
        this->pInCol = pInCol;

        Nodes.resize(pInRow * pInCol);
        for (int x = 0; x < pInRow; x++)
        {
            for (int y = 0; y < pInCol; y++)
            {
                Vec3 pos = Vec3(width * (x / (float)pInRow) - 25.0, 30, height * (y / (float)pInCol) - 25.0);
                Nodes[y * pInRow + x] = Node(pos);
            }
        }

        // Connecting immediate neighbor Nodes with constraints (distance 1 and sqrt(2) in the grid)
        for (int x = 0; x < pInRow; x++)
        {
            for (int y = 0; y < pInCol; y++)
            {
                if (x < pInRow - 1)
                    constraints.push_back(Constraint(getNode(x, y), getNode(x + 1, y), structualCoef));
                if (y < pInCol - 1)
                    constraints.push_back(Constraint(getNode(x, y), getNode(x, y + 1), structualCoef));
                if (x < pInRow - 1 && y < pInCol - 1)
                    constraints.push_back(Constraint(getNode(x, y), getNode(x + 1, y + 1), shearCoef));
                if (x < pInRow - 1 && y < pInCol - 1)
                    constraints.push_back(Constraint(getNode(x + 1, y), getNode(x, y + 1), shearCoef));
            }
        }

        // Connecting secondary neighbors with constraints (distance 2 in the grid)
        for (int x = 0; x < pInRow; x++)
        {
            for (int y = 0; y < pInCol; y++)
            {
                if (x < pInRow - 2)
                    constraints.push_back(Constraint(getNode(x, y), getNode(x + 2, y), bendingCoef));
                if (y < pInCol - 2)
                    constraints.push_back(Constraint(getNode(x, y), getNode(x, y + 2), bendingCoef));
            }
        }

        //// pin cloth
        //for (int i = 0;i < 1; i++)
        //{
        //    getNode(i, 0)->pos += Vec3(0.5, 0.0, 0.0);
        //    getNode(i, 0)->movable = false;
        //    getNode(pInRow - 1 - i, 0)->pos += Vec3(-0.5, 0.0, 0.0);
        //    getNode(pInRow - 1 - i, 0)->movable = false;
        //}
                // pin cloth
        for (int i = 0; i < 1; i++)
        {
            //getNode(i, 0)->pos += Vec3(0.5, 0.0, 0.0);
            getNode(i, 0)->movable = false;
            // getNode(pInRow - 1 - i, 0)->pos += Vec3(-0.5, 0.0, 0.0);
            getNode(pInRow - 1 - i, 0)->movable = false;
        }
    }

    void drawShaded(Vec3 color)
    {
        // reset normals (which where written to last frame)
        std::vector<Node>::iterator Node;
        for (Node = Nodes.begin(); Node != Nodes.end(); Node++)
        {
            (*Node).normal = Vec3(0, 0, 0);
        }

        for (int x = 0; x < pInRow - 1; x++)
        {
            for (int y = 0; y < pInCol - 1; y++)
            {
                Vec3 normal = computeFaceNormal(getNode(x + 1, y), getNode(x, y), getNode(x, y + 1));
                getNode(x + 1, y)->normal += normal;
                getNode(x, y)->normal += normal;
                getNode(x, y + 1)->normal += normal;

                normal = computeFaceNormal(getNode(x + 1, y + 1), getNode(x + 1, y), getNode(x, y + 1));
                getNode(x + 1, y + 1)->normal += normal;
                getNode(x + 1, y)->normal += normal;
                getNode(x, y + 1)->normal += normal;
            }
        }

        // normalize
        for (Node = Nodes.begin(); Node != Nodes.end(); Node++)
        {
            (*Node).normal.normalize();
        }

        glBegin(GL_TRIANGLES);
        glColor3fv((GLfloat*)&color);
        for (int x = 0; x < pInRow - 1; x++)
        {
            for (int y = 0; y < pInCol - 1; y++)
            {
                drawFace(getNode(x + 1, y), getNode(x, y), getNode(x, y + 1));
                drawFace(getNode(x + 1, y + 1), getNode(x + 1, y), getNode(x, y + 1));
            }
        }
        glEnd();
    }

    void timeStep(Vec3 center, float radius, float damping, float timestep, double groundHeight) // SatisfyConstraint + NodeTimeStep

    {


        for (int i = 0; i < Nodes.size(); i++)
        {
            Nodes[i].timeStep(damping, timestep); // calculate the position of each Node at the next time step.
        }

        //ballCollision(center, radius, groundHeight);

        for (int i = 0; i < CONSTRAINT_ITERATIONS; i++) // iterate over all constraints several times
        {
            for (int i = 0; i < constraints.size(); i++)
            {
                constraints[i].satisfyConstraint(CONSTRAINT_ITERATIONS); // satisfy constraint.
                //ballCollision(center, radius, groundHeight);
            }
        }
        //ballCollision(center, radius, groundHeight);



        ballCollision(center, radius, groundHeight);
        for (int i = 0; i < Nodes.size(); i++)
        {
            if (Nodes[i].movable) {
                Nodes[i].velocity = (Nodes[i].nextPos - Nodes[i].pos) / timestep;
                Nodes[i].pos = Nodes[i].nextPos;
                Nodes[i].force = Vec3(0, 0, 0);
            }
        }
    }

    /* used to add gravity (or any other arbitrary vector) to all Nodes*/
    void addForce(const Vec3 f)
    {
        std::vector<Node>::iterator Node;
        for (Node = Nodes.begin(); Node != Nodes.end(); Node++)
        {
            (*Node).addForce(f); // add the forces to each Node
        }
    }

    void ballCollision(const Vec3 center, const float radius, const double groundHeight)
    {
        std::vector<Node>::iterator Node;
        for (Node = Nodes.begin(); Node != Nodes.end(); Node++)
        {
            //cout << groundHeight << endl;
            if (Node->pos.y < groundHeight + 0.1)
            {
                Node->nextPos = Vec3(Node->nextPos.x, groundHeight + 0.1, Node->nextPos.z);
                Node->velocity.y = 0.0;
            }
            //Vec3 diffVec = (*Node).nextPos - center;
            ////diffVec - -diffVec;
            //float diffLen = diffVec.length();
            //if (diffVec.length() <= radius) // if the Node is inside the ball
            //{
            //    diffVec.normalize();
            //    (*Node).nextPos += diffVec * (radius - diffLen); // project the Node to the surface of the ball

            //    //(*Node).velocity = (*Node).velocity - diffVec * 2 * (*Node).velocity.dot(diffVec) ;
            //}
        }
    }

    void draw()
    {
        switch (drawMode)
        {
        case DRAW_MASS_NODES:
            glDisable(GL_LIGHTING);
            glEnable(GL_DEPTH_TEST);
            glPointSize(3.0);
            glBegin(GL_POINTS);
            glColor3f(1.0f, 1.0f, 0.0f);
            for (int x = 0; x < pInRow; x++)
            {
                for (int y = 0; y < pInCol; y++)
                {
                    glVertex3f(getNode(x, y)->pos.x, getNode(x, y)->pos.y, getNode(x, y)->pos.z);
                }
            }
            glEnd();
            glEnable(GL_LIGHTING);
            break;
        case DRAW_SPRINGS:
            glDisable(GL_LIGHTING);
            glEnable(GL_DEPTH_TEST);
            for (int i = 0; i < constraints.size(); i++)
                glDisable(GL_LIGHTING);
            glPointSize(3.0);
            glBegin(GL_LINES);
            glColor3f(0.3f, 0.3f, 0.8f);
            for (int i = 0; i < constraints.size(); i++)
            {
                glVertex3f(constraints[i].p1->pos.x, constraints[i].p1->pos.y, constraints[i].p1->pos.z);
                glVertex3f(constraints[i].p2->pos.x, constraints[i].p2->pos.y, constraints[i].p2->pos.z);
            }
            glEnd();
            glEnable(GL_LIGHTING);
            break;
        case DRAW_FACES:
            glEnable(GL_DEPTH_TEST);
            drawShaded(Vec3(0.0f, 0.8f, 0.9f));
            break;
        default:
            break;
        }
        glPopMatrix();
    }
};

#endif /* cloth_h */

