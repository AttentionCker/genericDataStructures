#
#


//declaring prototypes templates of STRUCTS to be used by the Graph
template <class TYPE>
struct Vertex;

template<class TYPE>
struct Arc;


//defining the STRUCTS:

//VERTEX(NODE)
template <class TYPE>
struct Vertex
{
    Vertex<TYPE>    *pNextVertex;
    TYPE            data;
    unsigned int    indegree;
    unsigned int    outdegree;
    short           processed;
    Arc<TYPE>       *pArc;
};

//ARC(EDGE)
template <class TYPE>
struct Arc
{
    Vertex<TYPE>    *pDestination;
    Arc<TYPE>       *pNextArc;
};



//declaring the Graph data type and function prototypes of various function implementations used by the Graph
template <class TYPE, class KTYPE>
class Graph
{
    Vertex<TYPE>    *first;
    unsigned int    count;

    public:

        Graph();
        ~Graph();

        // Basic Operations:
        int add_Vertex(TYPE DataIn);
        int delete_Vertex(KTYPE Key);
        int add_Arc(KTYPE fromKey, KTYPE toKey);
        int delete_Arc(KTYPE fromKey, KTYPE toKey);
        int find_Vertex(KTYPE Key, TYPE& DataOut);
        int Vertex_count(void);

        //Traversals:
        int DepthFirstTravel(void);
        int BreadthFirstTravel(void);

};

//function definitions of the class Graph:
template <class TYPE, class KTYPE>
Graph<TYPE, KTYPE>::Graph()
{
    count = 0;
    first = nullptr;
}


template <class TYPE, class KTYPE>
int Graph<TYPE, KTYPE>::add_Vertex(TYPE DataIn)
{
    Vertex<TYPE> *newVertex = nullptr;   

    newVertex = new Vertex<TYPE>;

    if(!newVertex)       // if newVertext = nullptr (nullptr is convertible to bool value)
        return 0;       //0 -> to indicate memory overflow or general failure

    count++;
    newVertex->data         =   DataIn;
    newVertex->indegree     =   0;
    newVertex->outdegree    =   0;
    newVertex->processed    =   0; 
    newVertex->pArc         =   nullptr;
    newVertex->pNextVertex  =   nullptr;

    if(first == nullptr)    //or if(!first)
    {
        first = newVertex;
        return 1;
    }

    Vertex<TYPE>    *pWalk_Vertex = nullptr, *pPrev_Vertex = nullptr;
    pWalk_Vertex = first;

    while(pWalk_Vertex && (pWalk_Vertex->data).key < DataIn.key)        //fixed a bug(probably) : from Vertex->data->key to (Vertex->data).key  
    {
        pPrev_Vertex = pWalk_Vertex;
        pWalk_Vertex = pWalk_Vertex->pNextVertex;
    }    

    if(!pPrev_Vertex)
    {
        first = newVertex;
    }
    else
    {
        pPrev_Vertex->pNextVertex = newVertex;
    }
    
    newVertex->pNextVertex = pWalk_Vertex;

    return 1;
}


