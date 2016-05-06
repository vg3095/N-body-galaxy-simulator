#include <SFML\Graphics.hpp>
#include<iostream>
#include<vector>
#include<cmath>
#include<cstdlib>
#include<ctime>
#define PI 3.14
#define G 0.1
#define MAX_LEVEL 100
using namespace std;

struct Body
{
    float posX,posY;
    float velX,velY;
    double forceX, forceY;
    float mass;
};

sf::Color default_color(255,255,255,128);

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

float zoom=1;

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

void generateParticles()
{
    srand(time(0));

    for(long long i=0;i<totalParticles;i++)
    {
        float angle = (rand()/(RAND_MAX*1.0))*2*PI;
        float coefficient= (rand()/(RAND_MAX*1.0));
        float distance = min_distance + (max_distance-min_distance)*(coefficient*coefficient);

        float posX=cos(angle)*distance + width_window_sim/2 ; // check pwidth/2
        float posY=sin(angle)*distance + height_window_sim/2 ; // check pheight/2

        float orbitalVel= sqrt(1.0*(galaxy_mass*G)/distance);
        float velX=sin(angle)*orbitalVel*1.0 ;
        float velY=-cos(angle)*orbitalVel*1.0 ;
        float mass = min_mass + rand()%(int)(max_mass-min_mass);
        //cout<<posX<<" "<<posY<<" "<<distance<<endl;
        bodies.push_back(createBody(posX,posY,velX,velY,mass));
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

        float force_coeff= 1.0*sqrt((pow(bodies[i]->forceX,2)+pow(bodies[i]->forceY,2)))*40000*G;
        //cout<<force_coeff<<endl;
        if (force_coeff>1)
            force_coeff=1;
        float R,Gr,B;
        B=1-force_coeff;
        if(force_coeff<0.2)
            R=force_coeff*5;
        else
            R=1;

        if(force_coeff<0.5)
            Gr=force_coeff*2;
        else
            Gr=1;
        temp.setFillColor(sf::Color(R*255,Gr*255,B*255));
        temp.setPosition(bodies[i]->posX,bodies[i]->posY);
        window.draw(temp);

    }
        window.display();
}
void reset_bodies()
{
    for(long long i=0;i<bodies.size();i++)
    {
        bodies[i]->forceX=0;
        bodies[i]->forceY=0;
    }

}


void update()
{
    for(long long i=0;i<bodies.size();i++)
    {
       /** if(bodies[i]->posX>width_window_sim && bodies[i]->velX > 0 || bodies[i]->posX<width_window_sim && bodies[i]->velX < 0)
        {
            bodies[i]->velX=-bodies[i]->velX;
        }

        if(bodies[i]->posY>height_window_sim && bodies[i]->velY > 0 || bodies[i]->posY<height_window_sim && bodies[i]->velY < 0)
        {
            bodies[i]->velY=-bodies[i]->velY;
        } */

        bodies[i]->velX+=bodies[i]->forceX/bodies[i]->mass;
        bodies[i]->velY+=bodies[i]->forceY/bodies[i]->mass;

        bodies[i]->posX+=bodies[i]->velX;
        bodies[i]->posY+=bodies[i]->velY;

    }

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

void force_calculate(Body* o1,Body* o2)
{

    float v_x=o2->posX-o1->posX;
    float v_y=o2->posY-o1->posY;
    float dis_sqr= v_x*v_x + v_y*v_y;
    float dis_cube_sqr=dis_sqr*dis_sqr*dis_sqr;
    float dis= 1.0f/sqrt(dis_cube_sqr);
    double force = 1.0f*(o1->mass*o2->mass*G*dis);

    o1->forceX+=v_x*force;
    o1->forceY+=v_y*force;


}

void central_force() //force b/w center and object
{

Body* temp = createBody(width_window_sim/2,height_window_sim/2,0,0,galaxy_mass);

for(long long i=0;i<bodies.size();i++)
{
    force_calculate(bodies[i],temp);
}

delete temp;

}





class QuadTreeNode
{
public:
    vector<QuadTreeNode*> child;
    vector<Body*> bodies;

    float posX,posY,width,height,total_mass;
    float COM_x,COM_y;
    long long depth;
    bool hasChild;

    QuadTreeNode()
    {
        depth=0;
    }
    QuadTreeNode(int depth_1)
    {
        depth=depth_1;
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

    void setParamaters(vector<Body*> bodies1,float width1,float height1,float posX1=0,float posY1=0)
    {
        //cout<<"q"<<endl;
        bodies=bodies1;
        posX=posX1;
        posY=posY1;
        width=width1;
        height=height1;


        double center_x=0;
        double center_y=0;

        for(long long i=0;i<bodies.size();i++)
        {
            total_mass+=bodies[i]->mass;
            center_x+=bodies[i]->mass*bodies[i]->posX;
            center_y+=bodies[i]->mass*bodies[i]->posY;

        }



        COM_x=1.0*(center_x/total_mass);
        COM_y=1.0*(center_y/total_mass);

        if(bodies.size()>1 && depth< MAX_LEVEL)
        {
            create_children();

        }
    }

    void create_children()
    {


        vector<Body*> q1,q2,q3,q4;
        float x_mid,y_mid;
        x_mid=width/2;
        y_mid=height/2;

        for(int i=0;i<bodies.size();i++)
        {
            if(bodies[i]->posX >posX+x_mid)
            {
                if(bodies[i]->posY>posY+y_mid)
                    q1.push_back(bodies[i]);
                else
                    q4.push_back(bodies[i]);
            }
            else
            {
                if(bodies[i]->posY>posY+y_mid)
                    q2.push_back(bodies[i]);
                else
                    q3.push_back(bodies[i]);
            }
        }

        QuadTreeNode* c1=new QuadTreeNode(depth+1);
        QuadTreeNode* c2=new QuadTreeNode(depth+1);
        QuadTreeNode* c3=new QuadTreeNode(depth+1);
        QuadTreeNode* c4=new QuadTreeNode(depth+1);


        c1->setParamaters(q1,width/2,height/2,posX,posY);
        c2->setParamaters(q2,width/2,height/2,posX+width/2,posY);
        c2->setParamaters(q3,width/2,height/2,posX,posY+height/2);
        c2->setParamaters(q4,width/2,height/2,posX+width/2,posY+height/2);

        child.push_back(c1);
        child.push_back(c2);
        child.push_back(c3);
        child.push_back(c4);
        hasChild=true;
    }


};
QuadTreeNode QhNode;

void force_group_node(QuadTreeNode* N,Body* body)
{

    float v_x=N->COM_x-body->posX;
    float v_y=N->COM_y-body->posY;

    float dis_sqr= v_x*v_x +v_y*v_y;
    float dis=1.0f/sqrt(pow(dis_sqr,3));
    double force = (N->total_mass * body->mass *dis* G);

    body->forceX+=(body->posX-body->posX)*force;
    body->forceY+=(body->posY-body->posY)*force;


}




void group_node_check(QuadTreeNode* N,Body* body)
{
    if(N->bodies.size()!=0)
    {
        float dX=N->COM_x-body->posX;
        float dY=N->COM_y-body->posY;
        float dist=sqrt((dX*dX+dY*dY));
        float width=N->width;
        if( ( width) /dist< (1) || N->hasChild==false)
        {
            force_group_node(N,body);
        }
        else
        {
            group_node_check(N->child[0],body);
            group_node_check(N->child[1],body);
            group_node_check(N->child[2],body);
            group_node_check(N->child[3],body);
        }

    }
}


void force_barnes()
{
    for(long long i=0;i<bodies.size();i++)
    {
        group_node_check(&QhNode,bodies[i]);
    }
}


int main()
{
    generateParticles();

    SetView(&SimulationView, &window, view_width, view_height);
   // run the program as long as the window is open
    while (window.isOpen())
    {
        manage_view();
        central_force();
        update();

        QhNode.reset();
        QhNode.setParamaters(bodies,width_window_sim,height_window_sim);
        force_barnes();
        draw_nodes();
        reset_bodies();
    }


}












