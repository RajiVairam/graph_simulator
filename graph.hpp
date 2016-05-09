# ifndef __GRAPH_HPP_
# define __GRAPH_HPP_

# include <iostream>
# include <fstream>
# include <sstream>
# include <iomanip>
# include <vector>
# include <set>
# include <map>
# include <string>
# include <time.h>
# include <algorithm>

# define INFINITY (unsigned int)-1

typedef unsigned int EdgeWeight;
typedef unsigned int EdgeCapacity;
typedef unsigned int NodeID;

// Node type (in node, out node, or both)
typedef enum {OUT, IN, ALL} ADJNODETYPE;

// Base class to have log file and format attributes
class Log
{
    protected:
        static std::ofstream _mCout;  // out stream
        static size_t _mTotalWidth;   // Maximum width
        static char _mDelimiter;
        bool _mPrinted;                 //check whether printed or not

    public:
        Log() {_mPrinted=false;}

        ~Log(){ }
        static bool open(std::string& file);
        static void close();
        static std::string center(const std::string str, int width);
        static std::string ftos(float n);
        static std::string itos(size_t n);
        static void print(const std::string& str);
        static void print_node_statistics_heading();
        static void print_edge_statistics_heading();
        void set_printed(bool flag);
        bool get_printed();
};

// Class for edge simulation properties
// Base class for Edge
class EdgeSimualtionProperty : public virtual Log
{
    protected:
        EdgeCapacity _mCurrentCapacity;  //Current capacity (weight - no of pkt recved)
        size_t _mNoOfPktsTransferred ;   // No of pkts transferred in current cycle

    public:
        EdgeSimualtionProperty(EdgeCapacity capacity)
            :
                Log(),
                _mCurrentCapacity(capacity),
                _mNoOfPktsTransferred(0)
    { }
        virtual ~EdgeSimualtionProperty() {}

        EdgeCapacity get_current_capacity();
        void set_current_capacity(EdgeCapacity capacity);
        void increment_current_capacity(); 
        void decrement_current_capacity(); 

        size_t get_no_of_pkts_transferred();
        void set_no_of_pkts_transferred(size_t value);
        void increment_no_of_pkts_transferred(); 
        void decrement_no_of_pkts_transferred(); 

        size_t get_no_of_pkts_in_queue();
        void set_no_of_pkts_in_queue(size_t value);
        void increment_no_of_pkts_in_queue(); 
        void decrement_no_of_pkts_in_queue(); 
        friend std::ofstream& operator<<(std::ofstream&,
                const EdgeSimualtionProperty&); 
};

// Class for node simulation properties
// Base class for Node
class NodeSimualtionProperty : public virtual Log
{
    protected:   // Node statistics
        size_t _mNoOfPktToBeSent;
        //size_t _mNoOfPktSent;
        //size_t _mNoOfPktReceived;
        size_t _mTotalNoOfPktRecv;
        size_t _mTotalNoOfPktSent;

    public:
        NodeSimualtionProperty(): Log()
    {
        _mNoOfPktToBeSent = 0;
        _mTotalNoOfPktSent = 0;
        _mTotalNoOfPktRecv = 0;
    }
        virtual ~NodeSimualtionProperty(){}

        size_t get_no_pkts_to_be_sent();
        void set_no_pkts_to_be_sent(size_t count);
        void increment_no_pkts_to_be_sent();
        void decrement_no_pkts_to_be_sent();

        size_t get_total_pkts_sent();
        void set_total_pkts_sent(size_t count);
        void increment_total_pkts_sent();

        size_t get_total_pkts_received();
        void set_total_pkts_received(size_t count);
        void increment_total_pkts_received();
        friend std::ofstream& operator<<(std::ofstream&,
                const NodeSimualtionProperty&);
};

// Forward declaration.
class Node;

class Edge : virtual public EdgeSimualtionProperty
{
    private:
        EdgeWeight __mWeight;      // Weight
        Node* __mSrc;              // Source
        Node* __mDest;             // Destination

    public:
        Edge(EdgeWeight weight, Node* src, Node* dest)
            :
                EdgeSimualtionProperty(weight),
                __mWeight(weight),
                __mSrc(src),
                __mDest(dest)
    { }
        ~Edge() {}

        Node* get_source_node();
        Node* get_destination_node();
        void set_weight(EdgeWeight new_weight);
        EdgeWeight get_weight();
        bool is_node_src(Node* src);
        bool is_node_dest(Node* dest);
        bool is_having_node(Node *node);
        void print_statistics();
};

typedef std::vector<NodeID> NodeIDList;
typedef std::vector<Edge*> EdgeList;
typedef std::vector<Edge*>::iterator EdgeIter;

class Node : virtual public NodeSimualtionProperty
{
    private:
        NodeID __mId;           // Node id
        EdgeList __mInEdges;    // Incoming edges list
        EdgeList __mOutEdges;   // Outgoing edges list
        bool __mVisited;        // for traversal

    public:
        Node(NodeID node_id)
            :
                NodeSimualtionProperty(),
                __mId(node_id),
                __mVisited(false)
    { }
        ~Node(){}

        NodeID& getId();

        void set_visited(bool visited_flag);
        bool is_visited();

        void add_incoming_edge(Edge* in_edge);
        void add_outgoing_edge(Edge* out_edge);

        void remove_edge_with_node(Node& anode);
        void unlink_all_edges();

        NodeIDList get_adjency_list(ADJNODETYPE nType);  //Get adj list based on filter

        Edge* get_edge_with_node(Node& node);
        // Summation of outgoing and incoming edges capacity
        size_t get_out_edges_capacity();
        size_t get_in_edges_capacity();
        void print_statistics();
};

typedef std::map<NodeID, Node*> NodeList;           // <Node_id, Node> pair
typedef std::map<NodeID, Node*>::iterator NodeListIter;
typedef std::vector<NodeID> Path;

class Graph
{
    private:
        NodeList __mNodes;                 // map of node id with node
        size_t __mMaxNodes;                // Max. No of Nodes
        size_t __mMaxEdges;                // Max. No of Edges

    public:
        Graph(size_t maxnode=0,
                size_t maxedge=0)
            :
                __mMaxNodes(maxnode),
                __mMaxEdges(maxedge)
    { }
        ~Graph(){}

        void add_edge(NodeID src_id, NodeID dest_id, EdgeWeight weight);
        void add_node(NodeID node_id);

        void delete_edge(Node& src_node, Node& dest_node);
        void delete_edge(NodeID src_id, NodeID dest_id);
        void delete_edge(Edge& edge);

        void delete_node(Node& node);
        void delete_node(NodeID node_id);

        Node* get_node_from_id(NodeID node_id);  // Get node from id

        std::vector<Path> get_paths(NodeID src_id, NodeID dest_id);
        std::vector<Path> get_paths(Node& src_node, Node& dest_node);

        void reset_traversal();

    private:
        void __form_path(Node& src, Node& dest,
                std::vector<Path>& nPathList,
                Path& nPath);
};

// Class for simulation.
// taking the created graph as argument
// can handle one flow (one src tp one dest) at a time
class Simulation
{
    private:
        Graph*  __mGraph;   // Created graph
        NodeID  __mSrc;     // source of flow
        NodeID  __mDest;    // destination of flow

    public:
        std::map<NodeID, EdgeCapacity> __mNodeSet;  // Filter of node ids thats involves
        // in the flow path
        std::vector<NodeID> __mNodeList;                 // Need to iterate in path order

    public:
        Simulation(Graph* graph)
            :
                __mGraph(graph)
    {
    }
        ~Simulation(){}

        bool update_src_and_dest(NodeID src, NodeID dest);
        NodeID get_src();
        NodeID get_dest();
        std::map<NodeID, EdgeCapacity>& get_node_set();
        // start simulation
        void start(NodeID src, NodeID dest, size_t no_of_cylce);
        void print_node_statistics();

    private:
        void __update_node_set(NodeID src, NodeID dest);
        void __start_arbitration();
        // data distribution (phase-1)
        void __distribute_data(Node* node, size_t no_pkt_tbs);
        // printing statistics (phase-2)
        void __consume_data();
};

#endif

