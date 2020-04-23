#include<stack>
#include<queue>
#include<utility>
#include<iostream>

#define INT_MAX 1000

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
    bool            bInMinTree;         //min-span-tree-flag
    bool            bInMinPathTree;     //min-path-flag
    int             iPathLength;
    Vertex<TYPE>    *pParentVertex;            
};

//ARC(EDGE)
template <class TYPE>
struct Arc
{
    Vertex<TYPE>    *pDestination;
    Arc<TYPE>       *pNextArc;
    int             weight;
    bool            bInMinTree;         //min-span-tree-flag
    bool            bInMinPathTree;
};



//declaring the Graph data type and function prototypes of various function implementations used by the Graph
template <class TYPE, class KTYPE>
class Graph
{
    Vertex<TYPE>    *first;
    unsigned int    count;
    int             iNumDisjoints;      //-1 if NumDisjoints not calculated

    public:

        Graph();
        ~Graph(){}          //temp graph destructor definition...remember to change as per need later if need arises

        // Basic Operations:
        int add_Vertex(TYPE DataIn);                //done
        int delete_Vertex(KTYPE Key);               //done --- not completely (wont delete vertex if not disjoint)
        int add_Arc(KTYPE fromKey, KTYPE toKey, int WEIGHT = 1);    //done
        int add_Arc_bothways(KTYPE fromKey, KTYPE toKey, int WEIGHT = 1);    //done
        int delete_Arc(KTYPE fromKey, KTYPE toKey); //done
        int find_Vertex(KTYPE Key, TYPE& DataOut);  //done
        int Vertex_count(void){return count;};      //done

        //Traversals:
        int DepthFirstTravel(void (*process)(TYPE ProcData));   //done
        int BreadthFirstTravel(void (*process)(TYPE ProcData)); //done
    
        //visualise the graph:
        void Visualize_Graph();                 //done
        void Visualize_Weighted_Graph();        //done
        
        //additional functionalities:
        int NumOfDisjoints();                   //done                
        int MinSpanningTree();                  //done 
        int ShortestPath(KTYPE , KTYPE);        //done
        int ShortestPathOptimized(KTYPE, KTYPE);

};

//function definitions of the class Graph:
template <class TYPE, class KTYPE>
Graph<TYPE, KTYPE>::Graph()
{
    count = 0;
    first = nullptr;
    iNumDisjoints = -1; 
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

    while(pWalkVertex && pWalkVertex->data.key < DataIn.key)         
    {
        pPrevVertex = pWalkVertex;
        pWalkVertex = pWalkVertex->pNextVertex;
    }    

    if(pWalkVertex && pWalkVertex->data.key == DataIn.key)
        return -3;                          //vertex already exists

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

    while(pWalkVertex && pWalkVertex->data.key < Key)
    {
        pPrevVertex = pWalkVertex;
        pWalkVertex = pWalkVertex->pNextVertex;
    }    

    if(!pWalkVertex || pWalkVertex->data.key != Key)
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
int Graph<TYPE, KTYPE>::add_Arc(KTYPE fromKey, KTYPE toKey, int WEIGHT)
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

    while(ptoVertex && ptoVertex->data.key < toKey)
    {
        ptoVertex = ptoVertex->pNextVertex;
    }
    if(!ptoVertex || ptoVertex->data.key != toKey)
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
    newArc->weight          =   WEIGHT;
    pfromVertex->outdegree++;
    ptoVertex->indegree++;

    if(!pfromVertex->pArc)
    {
        pfromVertex->pArc = newArc;
        return 1;                       //successfully created first arc of pfromVertex
    }

    //else find place to insert the arc in the pfromVertex's adjacency list
    Arc<TYPE>   *pWalkArc = pfromVertex->pArc, *pPrevArc = nullptr;

    while(pWalkArc && pWalkArc->pDestination->data.key < toKey)
    {
        pPrevArc = pWalkArc;
        pWalkArc = pWalkArc->pNextArc;
    }

    
    if(pWalkArc && pWalkArc->pDestination->data.key == toKey)
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
int Graph<TYPE, KTYPE>::delete_Arc(KTYPE fromKey, KTYPE toKey)
{
    // return Values:
    // -1   vertex not available
    // -2   arc not available


    if(!first)
    {
        return -1;      //no vertex in graph
    }

    Vertex<TYPE>    *pfromVertex = first;

    while(pfromVertex && pfromVertex->data.key < fromKey)
    {
        pfromVertex = pfromVertex->pNextVertex;
    }

    if(pfromVertex->data.key != fromKey)
    {
        return -1;      //no vertex match available
    }

    Arc<TYPE>   *pWalkArc = pfromVertex->pArc, *pPrevArc = nullptr;

    while(pWalkArc && pWalkArc->pDestination->data.key < toKey)
    {
        pPrevArc = pWalkArc;
        pWalkArc = pWalkArc->pNextArc;
    }

    if(!pWalkArc || pWalkArc->pDestination->data.key != toKey)
    {
        return -2;  //no arc available
    }

    if(!pPrevArc)
    {
        pfromVertex->pArc = pfromVertex->pArc->pNextArc;
    }
    else
    {
        pPrevArc->pNextArc = pWalkArc->pNextArc;
    }

    pfromVertex->outdegree--;
    pWalkArc->pDestination->indegree--;

    delete pWalkArc;

    return 1;   // located successfully & deleted the arc

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


template<class TYPE, class KTYPE>
int Graph<TYPE, KTYPE>::DepthFirstTravel(void (*process)(TYPE ProcData))
{   
    // return Values:
    //  1   success
    // -1   empty graph
   
    if(!first)
    {
        return -1;  //empty graph
    }

    Vertex<TYPE>    *pWalkVertex = first, *currentVertex = nullptr; 
    Arc<TYPE>       *pWalkArc = nullptr;

    while(pWalkVertex)
    {
        pWalkVertex->processed = 0;                     //setting processed flag for all vertices to 0
        pWalkVertex = pWalkVertex->pNextVertex;
    }

    pWalkVertex = first;
    std::stack<Vertex<TYPE>* >   VertexStack;

    while(pWalkVertex)
    {
        if(pWalkVertex->processed != 2)                 //vertex has not been processed
        {
            if(pWalkVertex->processed == 0)             //vertex not in stack
            {
                VertexStack.push(pWalkVertex);  //put in stack
                pWalkVertex->processed = 1;             //mark in stack
            }

            while(!VertexStack.empty())
            {
                currentVertex = VertexStack.top();
                VertexStack.pop();
                currentVertex->processed = 2;
                process(currentVertex->data);
                pWalkArc = currentVertex->pArc;

                while(pWalkArc)
                {
                    if(pWalkArc->pDestination->processed == 0)
                    {
                        VertexStack.push(pWalkArc->pDestination);
                        pWalkArc->pDestination->processed = 1;
                    }
                    pWalkArc = pWalkArc->pNextArc;
                }

            }
        }
        pWalkVertex = pWalkVertex->pNextVertex;

    }

    return 1;
}



template<class TYPE, class KTYPE>
int Graph<TYPE, KTYPE>::BreadthFirstTravel(void (*process)(TYPE ProcData))
{   
    // return Values:
    //  1   success
    // -1   empty graph
   
    if(!first)
    {
        return -1;  //empty graph
    }

    Vertex<TYPE>    *pWalkVertex = first, *currentVertex = nullptr; 
    Arc<TYPE>       *pWalkArc = nullptr;

    while(pWalkVertex)
    {
        pWalkVertex->processed = 0;                     //setting processed flag for all vertices to 0
        pWalkVertex = pWalkVertex->pNextVertex;
    }

    pWalkVertex = first;
    std::queue<Vertex<TYPE>* >   VertexQueue;

    while(pWalkVertex)
    {
        if(pWalkVertex->processed != 2)                 //vertex has not been processed
        {
            if(pWalkVertex->processed == 0)             //vertex not in Queue
            {
                VertexQueue.push(pWalkVertex);  //put in Queue
                pWalkVertex->processed = 1;             //mark in Queue
            }

            while(!VertexQueue.empty())
            {
                currentVertex = VertexQueue.front();
                VertexQueue.pop();
                currentVertex->processed = 2;
                process(currentVertex->data);
                pWalkArc = currentVertex->pArc;

                while(pWalkArc)
                {
                    if(pWalkArc->pDestination->processed == 0)
                    {
                        VertexQueue.push(pWalkArc->pDestination);
                        pWalkArc->pDestination->processed = 1;
                    }
                    pWalkArc = pWalkArc->pNextArc;
                }

            }
        }
        pWalkVertex = pWalkVertex->pNextVertex;

    }

    return 1;
}


template <class TYPE, class KTYPE>
void Graph<TYPE, KTYPE>::Visualize_Graph()
{   
    
    if(!first)
    {
        return;
    }
    Vertex<TYPE>    *pWalkVertex = first;
    Arc<TYPE>       *pWalkArc = nullptr;

    while(pWalkVertex)
    {
        pWalkArc = pWalkVertex->pArc;
        std::cout<<"\n("<<pWalkVertex->data.key<<", "<<pWalkVertex->data.val<<")";
        while(pWalkArc)
        {
            std::cout<<"=>("<<pWalkArc->pDestination->data.key<<", "<<pWalkArc->pDestination->data.val<<")";
            pWalkArc = pWalkArc->pNextArc;
        }
        pWalkVertex = pWalkVertex->pNextVertex;
    }
    std::cout<<"\n";
    return;
}

template <class TYPE, class KTYPE>
void Graph<TYPE, KTYPE>::Visualize_Weighted_Graph()
{   
    
    if(!first)
    {
        return;
    }
    Vertex<TYPE>    *pWalkVertex = first;
    Arc<TYPE>       *pWalkArc = nullptr;

    while(pWalkVertex)
    {
        pWalkArc = pWalkVertex->pArc;
        std::cout<<"\n("<<pWalkVertex->data.key<<", "<<pWalkVertex->data.val<<")";
        while(pWalkArc)
        {
            std::cout<<"=>("<<pWalkArc->pDestination->data.key<<", "<<pWalkArc->pDestination->data.val<<", "<<pWalkArc->weight<<")";
            pWalkArc = pWalkArc->pNextArc;
        }
        pWalkVertex = pWalkVertex->pNextVertex;
    }
    std::cout<<"\n";
    return;
}



template<class TYPE, class KTYPE>
int Graph<TYPE, KTYPE>::add_Arc_bothways(KTYPE fromKey, KTYPE toKey, int WEIGHT)
{
    int err = add_Arc(fromKey, toKey, WEIGHT);

    if(err == 1)
    {
        err = add_Arc(toKey, fromKey, WEIGHT);
    }
    else
    {
        return -1;  //failed
    }

    return err;     

}

template<class TYPE, class KTYPE>
int Graph<TYPE, KTYPE>::NumOfDisjoints()
{   
    // return Values:
    //  iNumDisjoints   number of disjoints
    // -1   empty graph disjoint has no meaning
   
    if(!first)
    {
        return -1;  //empty graph
    }

    Vertex<TYPE>    *pWalkVertex = first, *currentVertex = nullptr; 
    Arc<TYPE>       *pWalkArc = nullptr;

    while(pWalkVertex)
    {
        pWalkVertex->processed = 0;                     //setting processed flag for all vertices to 0
        pWalkVertex = pWalkVertex->pNextVertex;
    }

    pWalkVertex = first;
    iNumDisjoints = 1;      //no disjoints
    std::stack<Vertex<TYPE>* >   VertexStack;

    while(pWalkVertex)
    {
        if(pWalkVertex->processed != 2)                 //vertex has not been processed
        {
            if(pWalkVertex->processed == 0)             //vertex not in stack
            {
                VertexStack.push(pWalkVertex);  //put in stack
                pWalkVertex->processed = 1;             //mark in stack
            }

            while(!VertexStack.empty())
            {
                currentVertex = VertexStack.top();
                VertexStack.pop();
                currentVertex->processed = 2;
                pWalkArc = currentVertex->pArc;

                while(pWalkArc)
                {
                    if(pWalkArc->pDestination->processed == 0)
                    {
                        VertexStack.push(pWalkArc->pDestination);
                        pWalkArc->pDestination->processed = 1;
                    }
                    pWalkArc = pWalkArc->pNextArc;
                }

            }
        }
        

        pWalkVertex = pWalkVertex->pNextVertex;
        if(pWalkVertex && pWalkVertex->processed == 0)
        {
            iNumDisjoints++;
        }    
    }

    if(iNumDisjoints == 1)
        return 0;
    else
    return iNumDisjoints;
}


template<class TYPE,class KTYPE>
int Graph<TYPE, KTYPE>::MinSpanningTree()
{
    //return values:
    // -1   Empty Graph
    //  1   Success


    if(!count)      //equivalent to first == nullptr
    {
        return -1;  //empty graph
    }


    Vertex<TYPE>    *pWalkVertex    =   first;
    Arc<TYPE>       *pWalkArc       =   nullptr;

    while(pWalkVertex)
    {   
        pWalkVertex->bInMinTree = 0;
        pWalkArc = pWalkVertex->pArc;
        while(pWalkArc)
        {
            pWalkArc->bInMinTree = 0;
            pWalkArc = pWalkArc->pNextArc;
        }
        pWalkVertex = pWalkVertex->pNextVertex;
    }

    std::vector< Vertex<TYPE>* >  InTreeVertices;
    InTreeVertices.push_back(first);
    first->bInMinTree = true;
    int c = 1;
    while(c<count)
    {
        
        int iMinWeight;
        iMinWeight = INT_MAX;
        Arc<TYPE>   *pMinWeightArc = nullptr;

        KTYPE   tempToKey = first->data.key;

        for(auto pWalkVertex : InTreeVertices)
        {
            if(pWalkVertex->bInMinTree)
            {
                pWalkArc = pWalkVertex->pArc;

                while(pWalkArc)
                {
                    if(!pWalkArc->pDestination->bInMinTree && pWalkArc->weight <= iMinWeight)
                    {
                        pMinWeightArc = pWalkArc;
                        iMinWeight = pMinWeightArc->weight;
                        tempToKey = pWalkVertex->data.key;
                    }
                    pWalkArc = pWalkArc->pNextArc;
                }
            }
            
        }

        pMinWeightArc->bInMinTree = true;
        InTreeVertices.push_back(pMinWeightArc->pDestination);
        pMinWeightArc->pDestination->bInMinTree = true;
        c++;
 
        //adding the other direction arc:

        {
            // KTYPE toKey = pWalkVertex->data.key;
            pWalkArc = pMinWeightArc->pDestination->pArc;
            while(pWalkArc && pWalkArc->pDestination->data.key != tempToKey)
            {
                pWalkArc = pWalkArc->pNextArc;
            }

            if(pWalkArc)
            {
                pWalkArc->bInMinTree = true;
            }
        }

    }

    pWalkVertex = first;
    pWalkArc = nullptr;

    while(pWalkVertex)
    {
        pWalkArc = pWalkVertex->pArc;
        
        std::cout<<"\n("<<pWalkVertex->data.key<<", "<<pWalkVertex->data.val<<")";
        while(pWalkArc)
        {
            if(pWalkArc->bInMinTree)
                std::cout<<"=>("<<pWalkArc->pDestination->data.key<<", "<<pWalkArc->pDestination->data.val<<", "<<pWalkArc->weight<<")";
            pWalkArc = pWalkArc->pNextArc;
        }
        pWalkVertex = pWalkVertex->pNextVertex;
    }

    return 1;
}


template<class TYPE, class KTYPE>
int Graph<TYPE, KTYPE>::ShortestPath(KTYPE fromKey, KTYPE toKey)
{

    if(!first)
    {
        return -1;  // function called on empty graph
    }

    Vertex<TYPE>    *pWalkVertex    =   first;
    Vertex<TYPE>    *pfromVertex    =   nullptr;
    Vertex<TYPE>    *ptoVertex      =   nullptr;
    Arc<TYPE>       *pWalkArc       =   nullptr;

    while(pWalkVertex)
    {   
        if(pWalkVertex->data.key == fromKey)
            pfromVertex = pWalkVertex;


        if(pWalkVertex->data.key == toKey)
            ptoVertex = pWalkVertex;

        pWalkVertex->bInMinPathTree     = 0;
        pWalkVertex->iPathLength        = INT_MAX;

        pWalkArc = pWalkVertex->pArc;
        while(pWalkArc)
        {
            pWalkArc->bInMinPathTree = 0;
            pWalkArc = pWalkArc->pNextArc;
        }
        pWalkVertex = pWalkVertex->pNextVertex;
    }

    if(!pfromVertex || !ptoVertex)
    {
        return -2;      //vertices not available
    }

    
    std::vector< Vertex<TYPE>* >  InTreeVertices;
    InTreeVertices.push_back(pfromVertex);
    pfromVertex->bInMinPathTree = true;
    pfromVertex->iPathLength = 0;
    int c = 1;
   
    while(c<count)
    {
        
        int iMinPathLength = INT_MAX;
        
        Arc<TYPE>   *pMinPathArc = nullptr;

        for(auto pWalkVertex : InTreeVertices)
        {
            if(pWalkVertex->bInMinPathTree)
            {
                pWalkArc = pWalkVertex->pArc;

                while(pWalkArc)
                {
                    if(!pWalkArc->pDestination->bInMinPathTree && (pWalkVertex->iPathLength + pWalkArc->weight) <= iMinPathLength)
                    {
                        pMinPathArc = pWalkArc;
                        iMinPathLength = pMinPathArc->weight + pWalkVertex->iPathLength;                        
                    }
                    pWalkArc = pWalkArc->pNextArc;
                }
            }
            
        }

        pMinPathArc->bInMinPathTree = true;
        pMinPathArc->pDestination->bInMinPathTree = true;
        InTreeVertices.push_back(pMinPathArc->pDestination);
        pMinPathArc->pDestination->iPathLength = iMinPathLength;
        c++;

    }    
/*  //display the minPathTree 
    pWalkVertex = first;
    pWalkArc = nullptr;

    while(pWalkVertex)
    {
        pWalkArc = pWalkVertex->pArc;
        
        std::cout<<"\n("<<pWalkVertex->data.key<<", "<<pWalkVertex->data.val<<")";
        while(pWalkArc)
        {
            if(pWalkArc->bInMinPathTree)
                std::cout<<"=>("<<pWalkArc->pDestination->data.key<<", "<<pWalkArc->pDestination->data.val<<", "<<pWalkArc->weight<<")";
            pWalkArc = pWalkArc->pNextArc;
        }
        pWalkVertex = pWalkVertex->pNextVertex;
    }
*/

    std::stack< Vertex<TYPE>* >     tempStack;
    tempStack.push(pfromVertex);

    std::vector< Vertex<TYPE>* >    vectShortestPath;

    bool toVertexFound = false;

    while(!toVertexFound)
    {   pWalkVertex = tempStack.top();
        
        //if only one vertex is available in the stack pop it and push in the solution. AND then push all adjacent vertices in the stack
        if(tempStack.size() == 1)
        {
            vectShortestPath.push_back(pWalkVertex);
            tempStack.pop();
            
            pWalkArc = pWalkVertex->pArc;

            while(pWalkArc)
            {
                if(pWalkArc->bInMinPathTree)
                {
                    if(pWalkArc->pDestination->data.key == toKey)
                    {
                        vectShortestPath.push_back(pWalkArc->pDestination);
                        toVertexFound = true;
                        pWalkArc = nullptr;
                    }
                    else
                    {
                        tempStack.push(pWalkArc->pDestination);
                        pWalkArc->pDestination->processed = 1;
                        pWalkArc = pWalkArc->pNextArc;
                    }
                }
                else
                {
                    pWalkArc = pWalkArc->pNextArc;
                }
                
            }
        }
        // if more than one vertices available in stack, check for presence of toVertex from the top vertex by dfs. if toVertex present (it must be present UNIQUELY) push that vertex to solution else pop it.
        else
        {
            std::stack< Vertex<TYPE>* >     dfsStack;
            dfsStack.push(pWalkVertex);

            bool dfsToVertexPresent = false;

            while(!dfsStack.empty() && !dfsToVertexPresent)
            {
                pWalkArc = (dfsStack.top())->pArc;
                dfsStack.pop();

                while(pWalkArc)
                {
                    if(pWalkArc->bInMinPathTree)
                    {
                        if(pWalkArc->pDestination->data.key == toKey)
                        {
                            pWalkArc = nullptr;
                            pWalkVertex->processed = 2;
                            vectShortestPath.push_back(pWalkVertex);
                            dfsToVertexPresent = true;
                        }
                        else
                        {
                            dfsStack.push(pWalkArc->pDestination);
                            pWalkArc = pWalkArc->pNextArc;
                        }
                        
                    }
                    else
                    {
                        pWalkArc = pWalkArc->pNextArc;
                    }
                }
            }

            if(pWalkVertex->processed == 1)
            {
                tempStack.pop();
                pWalkVertex->processed = 2;
            }
            else
            {
                while(!tempStack.empty())
                {
                    tempStack.pop();
                }   //saving space

                pWalkArc = pWalkVertex->pArc;
                while (pWalkArc)
                {
                    if (pWalkArc->bInMinPathTree)
                    {
                        if (pWalkArc->pDestination->data.key == toKey)
                        {
                            vectShortestPath.push_back(pWalkArc->pDestination);
                            toVertexFound = true;
                            pWalkArc = nullptr;
                        }
                        else
                        {
                            tempStack.push(pWalkArc->pDestination);
                            pWalkArc->pDestination->processed = 1;
                            pWalkArc = pWalkArc->pNextArc;
                        }
                    }
                    else
                    {
                        pWalkArc = pWalkArc->pNextArc;
                    }
                }
            }
            

        }
        
    }

   //displaying the shortest path from fromVertex to toVertex and also the min path length:
    
    for(typename std::vector< Vertex<TYPE>* >::iterator i = vectShortestPath.begin(); i != --(vectShortestPath.end()); i++)
    {
        std::cout<<(*i)->data.val<<"=>";
    }
    std::cout<<(*(--(vectShortestPath.end())))->data.val<<"\nMinimum Path Length = "<<ptoVertex->iPathLength;

    return 1;

}




template<class TYPE, class KTYPE>
int Graph<TYPE, KTYPE>::ShortestPathOptimized(KTYPE fromKey, KTYPE toKey)
{

    if(!first)
    {
        return -1;  // function called on empty graph
    }

    Vertex<TYPE>    *pWalkVertex    =   first;
    Vertex<TYPE>    *pfromVertex    =   nullptr;
    Vertex<TYPE>    *ptoVertex      =   nullptr;
    Arc<TYPE>       *pWalkArc       =   nullptr;

    while(pWalkVertex)
    {   
        if(pWalkVertex->data.key == fromKey)
            pfromVertex = pWalkVertex;


        if(pWalkVertex->data.key == toKey)
            ptoVertex = pWalkVertex;

        pWalkVertex->pParentVertex      = nullptr;
        pWalkVertex->bInMinPathTree     = 0;
        pWalkVertex->iPathLength        = INT_MAX;

        pWalkArc = pWalkVertex->pArc;
        while(pWalkArc)
        {
            pWalkArc->bInMinPathTree = 0;
            pWalkArc = pWalkArc->pNextArc;
        }
        pWalkVertex = pWalkVertex->pNextVertex;
    }

    if(!pfromVertex || !ptoVertex)
    {
        return -2;      //vertices not available
    }

    
    std::vector< Vertex<TYPE>* >  InTreeVertices;
    InTreeVertices.push_back(pfromVertex);
    pfromVertex->bInMinPathTree = true;
    pfromVertex->iPathLength = 0;
    int c = 1;

    Vertex<TYPE>    *tempParentVertex;
   
    while(c<count)
    {
        
        int iMinPathLength = INT_MAX;
        
        Arc<TYPE>   *pMinPathArc = nullptr;

        for(auto pWalkVertex : InTreeVertices)
        {
               pWalkArc = pWalkVertex->pArc;

                while(pWalkArc)
                {
                    if(!pWalkArc->pDestination->bInMinPathTree && (pWalkVertex->iPathLength + pWalkArc->weight) < iMinPathLength)
                    {
                        pMinPathArc = pWalkArc;
                        iMinPathLength = pMinPathArc->weight + pWalkVertex->iPathLength; 

                        tempParentVertex = pWalkVertex;                       
                    }
                    pWalkArc = pWalkArc->pNextArc;
                }
            
            
        }

        pMinPathArc->bInMinPathTree = true;
        pMinPathArc->pDestination->bInMinPathTree = true;
        InTreeVertices.push_back(pMinPathArc->pDestination);
        pMinPathArc->pDestination->iPathLength = iMinPathLength;
        c++;

        pMinPathArc->pDestination->pParentVertex = tempParentVertex;
    }    


    std::stack< Vertex<TYPE>* >     ShortestPathStack;
        
    pWalkVertex = ptoVertex;
    while(pWalkVertex->data.key != fromKey)
    {
        ShortestPathStack.push(pWalkVertex);
        pWalkVertex = pWalkVertex->pParentVertex;
        
    }
    ShortestPathStack.push(pWalkVertex);    

    while(ShortestPathStack.size() != 1)
    {
        std::cout<<(ShortestPathStack.top())->data.val<<"=>";
        ShortestPathStack.pop();
    }
    std::cout << (ShortestPathStack.top())->data.val<<"\n";
    ShortestPathStack.pop();

    return 1;
}