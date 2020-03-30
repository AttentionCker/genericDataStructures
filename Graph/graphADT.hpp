#
#
#include<utility>


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
        ~Graph(){}          //temp graph destructor definition...remember to change as per need later if need arises

        // Basic Operations:
        int add_Vertex(TYPE DataIn);
        int delete_Vertex(KTYPE Key);
        int add_Arc(KTYPE fromKey, KTYPE toKey);
        int delete_Arc(KTYPE fromKey, KTYPE toKey);
        int find_Vertex(KTYPE Key, TYPE& DataOut);
        int Vertex_count(void){return count;};

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

    Vertex<TYPE>    *pWalkVertex = nullptr, *pPrevVertex = nullptr;
    pWalkVertex = first;

    while(pWalkVertex && (pWalkVertex->data).key < DataIn.key)        //fixed a bug(probably) : from Vertex->data->key to (Vertex->data).key  
    {
        pPrevVertex = pWalkVertex;
        pWalkVertex = pWalkVertex->pNextVertex;
    }    

    if(!pPrevVertex)
    {
        first = newVertex;
    }
    else
    {
        pPrevVertex->pNextVertex = newVertex;
    }
    
    newVertex->pNextVertex = pWalkVertex;

    return 1;
}


template <class TYPE, class KTYPE>
int Graph<TYPE, KTYPE>::delete_Vertex(KTYPE Key)
{
    if(!first)
    { 
        return -1;      //graph empty
    }

    Vertex<TYPE>    *pWalkVertex = first;
    Vertex<TYPE>    *pPrevVertex = nullptr;

    while(pWalkVertex && (pWalkVertex->data).key < Key)
    {
        pPrevVertex = pWalkVertex;
        pWalkVertex = pWalkVertex->pNextVertex;
    }    

    if(!pWalkVertex || (pWalkVertex->data).key != Key)
    {
        return -2;  //Element not found in the graph
    }

    if(pWalkVertex->indegree == 0 && pWalkVertex->outdegree == 0)
    {
        if(!pPrevVertex)
        {
            first = first->pNextVertex;
        }
        else
        {
            pPrevVertex->pNextVertex = pWalkVertex->pNextVertex;
        }

        delete pWalkVertex;
        count--;
        return 1;
        
    }    
    else
    {
        return -3;      //can't delete as of now
    }
    

}


template<class TYPE, class KTYPE>
int Graph<TYPE, KTYPE>::add_Arc(KTYPE fromKey, KTYPE toKey)
{
    //return values used: 
    // -1   loop not allowed in my graph
    // -2   no key match
    // -3   arc already exists
    //  1   success


    if(fromKey == toKey)
    {
        return -1; //loop not allowed in my graph
    }

    //temporarily swaping from and to keys to make program efficient
    bool keySwap;
    if(fromKey < toKey)         
    {
        keySwap = false;
    }
    else
    {
        std::swap(fromKey,toKey);
        keySwap = true;
    } 

    //finding the element pointer for keys (fromKey corresponds to vertex prior to toKey ----pfromVertex starts from first)
    Vertex<TYPE>    *pfromVertex = first, *ptoVertex=nullptr;

    while(pfromVertex && pfromVertex->data.key < fromKey)
    {
        pfromVertex = pfromVertex->pNextVertex;
    }
    if(!pfromVertex || pfromVertex->data.key != fromKey)
    {
        return -2;  //no key match
    }

    //as toKey>fromKey ptoVertex descends(is after) pfromVertex
    ptoVertex = pfromVertex;

    while(ptoVertex && pfromVertex->data.key < toKey)
    {
        ptoVertex = ptoVertex->pNextVertex;
    }
    if(!ptoVertex || pfromVertex->data.key != toKey)
    {
        return -2;  //key match not found
    }

    //key matches found and vertices found

    if(keySwap)
    {
        std::swap(fromKey, toKey);
        std::swap(pfromVertex, ptoVertex);
    }//ptrs to "from" and "to" vertices found in the graph

    
    //creating new arc:
    Arc<TYPE>   *newArc     =   new Arc<TYPE>;
    newArc->pDestination    =   ptoVertex;
    newArc->pNextArc        =   nullptr;
    pfromVertex->outdegree++;
    ptoVertex->indegree++;

    if(!pfromVertex->pArc)
    {
        pfromVertex->pArc = newArc;
        return 1;                       //successfully created first arc of pfromVertex
    }

    //else find place to insert the arc in the pfromVertex's adacency list
    Arc<TYPE>   *pWalkArc = pfromVertex->pArc, *pPrevArc = nullptr;

    while(pWalkArc && pWalkArc->pDestination->data.key < toKey)
    {
        pPrevArc = pWalkArc;
        pWalkArc = pWalkArc->pNextArc;
    }

    if(pWalkArc->pDestination->data.key == toKey)
    {
        return -3;  //arc already exists
    }

    if(!pPrevArc)
    {
        pfromVertex->pArc = newArc;
    }
    else
    {
        pPrevArc->pNextArc = newArc;
    }
    
    newArc->pNextArc = pWalkArc;

    return 1;   //successfully added ARC

}




template <class TYPE, class KTYPE>
int Graph<TYPE, KTYPE>::find_Vertex(KTYPE Key, TYPE &DataOut)
{   
    // return values:
    // -1   No match for key
    //  1   Successfully located vertex and data loaded in DataOut 

    if(!first)
    {
        return -1;  //key not found as no vertex is present in the graph
    }


    Vertex<TYPE>    *pWalkVertex = first;
    
    while(pWalkVertex && pWalkVertex->data.key < Key)
    {
        pWalkVertex = pWalkVertex->pNextVertex;
    }

    if(pWalkVertex->data.key == Key)        //vertex found
    {
        DataOut = pWalkVertex->data;
        return 1;                           //successfully located the vertex and data loaded in DataOut
    }
    else
    {
        return -1;                          //no key match
    }

}