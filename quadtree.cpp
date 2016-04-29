#include<iostream>
#include<vector>
using namespace std;

struct Body
{
    float posX,posY;
    float velX,velY;
    double forceX, forceY;
    float mass;
};

class Node
{
public:
    vector<Body*> bodies;
    vector<Node*> child;


};
