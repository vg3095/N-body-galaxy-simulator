#include<iostream>
#include<vector>
#include<cmath>
#include<cstdlib>
#include<ctime>
#define PI 3.14
#define G 0.1 //doubt units and orbital velocity
using namespace std;
struct Body
{
    float posX,posY;
    float velX,velY;
    double forceX, forceY;
    float mass;
};
void generateParticles();
void force_calculate(Body* o1,Body* o2);
Body* createBody(float posX,float posY,float velX,float velY,float mass);
void central_force();

class Node
{
public:
    vector<Body*> bodies;
    vector<Node*> child;
    bool hasChild = false; //usage don`t know
    float posX,posY,width,height,total_mass;
    float centerOfMass_x,centerOfMass_y;
    long long depth;
    Node(long long depth)
    {
        depth=depth;

    }
    void reset()
    {
        bodies.clear();

        for(long long i=0;i<child.size();i++)
        {
            child[i]->reset();
            delete child[i];

        }
        child.clear();

        hasChild=false;

    }

    void setParamaters(vector<Body*> bodies;float width,float height,float posX=0,float posY=0)
    {
        //confusing parameters
        bodies=bodies;
        posX=posX;
        posY=posY;
        width=width;
        height=height;

        float mass=0;
        double center_x=0;
        double center_y=0;

        for(long long i=0;i<bodies.size();i++)
        {
            mass+=bodies[i]->mass;
            center_x+=bodies[i]->posX;
            center_y+=bodies[i]->posY;

        }

        total_mass=mass;

        centerOfMass_x=center_x/bodies.size();
        centerOfMass_y=center_y/bodies.size();

        if(bodies.size()>1 && depth< MAX_DEPTH)  //condition missing
        {
            create_children();

        }
    }

    void creat_children()
    {
        vector<Body*> q1,q2,q3,q4;

        for(int i=0;i<bodies.size();i++)
        {

            //I wrote something different
            if(bodies[i]->posX <(posX+(width/2)))
            {
                if(bodies[i]->posY <(posY+(height/2)))
                {
                    //quadrant 1
                    q1.push_back(bodies[i]);
                }
                else
                {
                    //quadrant 3
                    q3.push_back(bodies[i]);

                }

            }
            else{
               if(bodies[i]->posY < (posY+(height/2)) )
               {
                   //quadrant 2
                   q2.push_back(bodies[i]);

               }
               else
               {
                   //quadrant 4
                   q4.push_back(bodies[i]);
               }

            }
        }

        Node* c1=new Node(depth+1);
        Node* c2=new Node(depth+1);
        Node* c3=new Node(depth+1);
        Node* c4=new Node(depth+1);

        c1->setParamaters(q1,width/2,height/2,posX,posY);
        c2->setParamaters(q1,width/2,height/2,posX+width/2,posY);
        c2->setParamaters(q1,width/2,height/2,posX,posY+height/2);
        c2->setParamaters(q1,width/2,height/2,posX+width/2,posY+height/2);

        child.push_back(c1);
        child.push_back(c2);
        child.push_back(c3);
        child.push_back(c4);
        hasChild=true;
    }


};

vector<Body*> bodies;
float min_distance= 50;
float max_distance= 20000;
float galaxy_mass = 100000;
long long totalParticles =1000;
float min_mass =1;
float max_mass =2;
float height_window_sim=327680;
float width_window_sim=327680;
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

void central_force() //force b/w center and object
{
Body* temp = createBody(width_window_sim/2,height_window_sim/2,0,0,galaxy_mass);

for(long long i=0;i<bodies.size();i++)
{

    force_calculate(bodies[i],temp);
}
}

void force_calculate(Body* o1,Body* o2)
{
    float dis_sqr= pow((o2->posX-o1->posX),2)+pow((o2->posY-o1->posY),2);

    double force = (o1->mass*o2->mass*G)/(dis_sqr);

    o1->forceX+=(o2->posX-o1->posX)*force;
    o1->forceY+=(o2->posY-o1->posY)*force;



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

void update()
{
    for(long long i=0;i<bodies.size();i++)
    {
        //some conditions missing

        bodies[i]->velX+=bodies[i]->forceX/bodies[i]->mass;
        bodies[i]->velY+=bodies[i]->forceY/bodies[i]->mass;

        bodies[i]->posX+=bodies[i]->velX;
        bodies[i]->posY+=bodies[i]->velY;

    }

}
