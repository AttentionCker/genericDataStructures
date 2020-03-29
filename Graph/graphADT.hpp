#
#


//declaring prototypes templates of STRUCTS to be used by the graph
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



//declaring the graph data type and function prototypes of various function implementations used by the graph
template <class TYPE, class KTYPE>
class graph
{
    Vertex<TYPE>    *first;
    unsigned int    count;

    public:

        graph();
        ~graph();

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