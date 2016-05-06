class QuadTree
{
public:
    int MAX_OBJECTS=10;
    int MAX_LEVELS=5;

    int level;
    vector<Body*> objects;
    //Rectangle bounds;
    QuadTree[] nodes;
    //constructor
    QuadTree(int plevel,Rectangle pBounds)
    {
        level=plevel;
        //bounds=pbounds;
        nodes=new QuadTree[4];

    }

    //Clears Quadtree
    void clear()
    {

        objects.clear();
        for(i=0;i<nodes.length();i++)
        {
            if(nodes[i]!=NULL)
                nodes[i].clear();
            nodes[i]=NULL;

        }
    }

    //Split Node into 4 subnodes;
    void split()
    {
        int subWidth=(int)(bounds.width)/2;
        int subHeight=(int)(bounds.height)/2;

        nodes[0]=new QuadTree(level+1,)

    }
};
