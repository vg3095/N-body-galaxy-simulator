#include <SFML\Graphics.hpp>
#include<iostream>
#include<vector>
#include<cmath>
#include<cstdlib>
#include<ctime>
#define PI 3.14
#define G 0.1 //doubt units and orbital velocity
#define MAX_DEPTH 100
using namespace std;
sf::Color default_color(255,255,255,128);
struct Body
{
    float posX,posY;
    float velX,velY;
    double forceX, forceY;
    float mass;
};
void generateParticles();
void draw_nodes();
void reset_bodies();
void update();
void manage_view();
void SetView(sf::View* pView, sf::RenderWindow* pTarget, float pViewWidth, float pViewHeight);
void force_calculate(Body* o1,Body* o2);
Body* createBody(float posX,float posY,float velX,float velY,float mass);
void central_force();
float zoom=1;
class Node
{
public:
    vector<Body*> bodies;
    vector<Node*> child;
//    bool hasChild = false; //usage don`t know
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

//        hasChild=false;

    }

    void setParamaters(vector<Body*> bodies1,float width1,float height1,float posX1=0,float posY1=0)
    {
        //confusing parameters
        bodies=bodies1;
        posX=posX1;
        posY=posY1;
        width=width1;
        height=height1;

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

    void create_children()
    {

        //check logic can`t understand
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
//        hasChild=true;
    }


};
vector<Body*> bodies;
float min_distance= 50;
float max_distance= 20000;
float galaxy_mass = 100000;
long long totalParticles =10000;
float min_mass =1;
float max_mass =2;
float height_window_sim=327680;
float width_window_sim=327680;
float view_width=1920;
float view_height=1080;
sf::RenderWindow window(sf::VideoMode(1920, 1080), "N-Body simulation",sf::Style::Titlebar);
sf::View SimulationView;

int main()
{
    generateParticles();

    SetView(&SimulationView, &window, view_width, view_height);
   // run the program as long as the window is open
    while (window.isOpen())
    {
        manage_view();
        //central_force();
        update();
        reset_bodies();
        draw_nodes();
    }


}

void generateParticles()
{
    srand(time(0));

    for(long long i=0;i<totalParticles;i++)
    {
        float angle = (rand()/(RAND_MAX*1.0))*2*PI;
        float coefficient= (rand()/(RAND_MAX*1.0));
        float distance = min_distance + (max_distance-min_distance)*(coefficient*coefficient);

        float posX=sin(angle)*distance + width_window_sim/2 ; // check pwidth/2
        float posY=cos(angle)*distance + height_window_sim/2 ; // check pheight/2

        float orbitalVel= sqrt((galaxy_mass*G)/distance);
        float velX=sin(angle)*orbitalVel ;
        float velY=cos(angle)*orbitalVel ;
        float mass = min_mass + (max_mass-min_mass)*coefficient;
        cout<<posX<<" "<<posY<<" "<<distance<<endl;
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

void reset_bodies()
{
    for(long long i=0;i<bodies.size();i++)
    {
        bodies[i]->forceX=0;
        bodies[i]->forceY=0;
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

void draw_nodes()
{
    window.clear();

    sf::RectangleShape temp;
    temp.setFillColor(default_color);

    for(long long i=0;i<bodies.size();i++)
    {
        if(zoom>1)
            temp.setSize(sf::Vector2f(bodies[i]->mass*zoom,bodies[i]->mass*zoom));
        else
            temp.setSize(sf::Vector2f(bodies[i]->mass,bodies[i]->mass));

        //stuff missing
        //zoom capabilities

        temp.setFillColor(sf::Color(255,255,255));
        temp.setPosition(bodies[i]->posX,bodies[i]->posY);
        window.draw(temp);

    }
        window.display();
}

void manage_view()
{
    sf::Event event;

    while(window.pollEvent(event))
    {

        if(event.type==sf::Event::Closed)
            window.close();
        if(event.type==sf::Event::MouseWheelMoved)
        {
            zoom*=1+(float)(-event.mouseWheel.delta)/10;
            SimulationView.zoom(1+(float)(-event.mouseWheel.delta)/10);
        }
    }

    if (sf::Mouse::getPosition().x > (1920 - 20))
        SimulationView.move(2 * zoom, 0);
    if (sf::Mouse::getPosition().x < (0 + 20))
        SimulationView.move(-2 * zoom, 0);
    if (sf::Mouse::getPosition().y > (1080 - 20))
        SimulationView.move(0, 2 * zoom);
    if (sf::Mouse::getPosition().y < (0 + 20))
        SimulationView.move(0, -2 * zoom);

    window.setView(SimulationView);


}


void SetView(sf::View* pView, sf::RenderWindow* pTarget, float pViewWidth, float pViewHeight)
{
    pView->reset(sf::FloatRect(width_window_sim / 2 - pViewWidth / 2, height_window_sim / 2 - pViewHeight / 2, pViewWidth, pViewHeight));
    pView->setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
    pTarget->setView(*pView);
}
