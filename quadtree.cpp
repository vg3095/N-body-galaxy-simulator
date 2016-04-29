#include<iostream>
#include<vector>
#include<cmath>
#include<cstdlib>
#include<ctime>
#define PI 3.14
#define G 0.1 //doubt units and orbital velocity
using namespace std;
void generateParticles();
Body* createBody(float posX,float posY,float velX,float velY,float mass);
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

vector<Body*> bodies;
float min_distance= 50;
float max_distance= 20000;
float galaxy_mass = 100000;
long long totalParticles =1000;
float min_mass =1;
float max_mass =2;
int main()
{
    generateParticles();

}

void generateParticles()
{
    srand(time(NULL));

    for(long long i=0;i<totalParticles;i++)
    {
        float angle = (rand()/RAND_MAX*1.0)*2*PI;
        float coefficient= (rand()/RAND_MAX*1.0);
        float distance = min_distance + (max_distance-min_distance)*coefficient;

        float posX=sin(angle)*distance ; // check pwidth/2
        float posY=cos(angle)*distance ; // check pheight/2

        float orbitalVel= sqrt((galaxy_mass*G)/distance);
        float velX=sin(angle)*orbitalVel ;
        float velY=cos(angle)*orbitalVel ;
        float mass = min_mass + (max_mass-min_mass)*coefficient;

        bodies.push_back(createBody(posX,posY,velX,velY,mass));
    }

}

Body* createBody(float posX,float posY,float velX,float velY,float mass)
{
    Body* temp=new Body;

    temp->posX=posX;
    temp->posY=posY;
    temp->mass=mass;
    temp->velX=velX;
    temp->velY=velY;
    temp->forceX=temp->forceY=0;

    return temp;
}


