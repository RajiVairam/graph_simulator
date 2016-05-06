# include "graph.hpp"

/**********************************************************************
 *                     Edge class methods                             *
 **********************************************************************/
Node* Edge::get_source_node()
{
    return __mSrc;
}

Node* Edge::get_destination_node()
{
    return __mDest;
}

void Edge::set_weight(unsigned int new_weight)
{
    __mWeight = new_weight;
}

unsigned int Edge::get_weight()
{
    return __mWeight;
}

// Return true, if the given node is source of
// current edge
bool Edge::is_node_src(Node* src)
{
    if ((src) && (src->getId() == __mSrc->getId())) {
        return true;
    }
    return false;
}

// Return true, if the given node is destination of
// current edge
bool Edge::is_node_dest(Node* dest)
{
    if ((dest) && (dest->getId() == __mDest->getId())) {
        return true;
    }
    return false;
}

// Return true, if the given node is source/destination of
// current edge
bool Edge::is_having_node(Node *node)
{
    if (is_node_src(node) || is_node_dest(node)) {
        return true;
    }
    return false;
}

/**********************************************************************
 *                     Node class methods                               *
 **********************************************************************/
NodeID& Node::getId()
{
    return __mId;
}

void Node::set_visited(bool mVisited_flag=true)
{
    __mVisited = mVisited_flag;
}

bool Node::is_visited()
{
    return __mVisited;
}

void Node::add_incoming_edge(Edge* in_edge)
{
    __mInEdges.push_back(in_edge);
}

void Node::add_outgoing_edge(Edge* out_edge)
{
    __mOutEdges.push_back(out_edge);
}

// Remove the edges that links to given node
void Node::remove_edge_with_node(Node& anode)
{
    for (int i= 0; i < __mInEdges.size(); ++i) {
        if (__mInEdges[i]->is_node_src(&anode)) {
            __mInEdges.erase(__mInEdges.begin()+i);
            break;
        }
    }
    for (int i= 0; i < __mOutEdges.size(); ++i) {
        if (__mOutEdges[i]->is_node_dest(&anode)) {
            __mOutEdges.erase(__mOutEdges.begin()+i);
            break;
        }
    }
}

// Get adjency list of current node  based on filter
// Can be OUT, IN, ALL
NodeIDList Node::get_adjency_list(ADJNODETYPE nType=ALL)
{
    NodeIDList adj_list;
    if (nType == IN || nType == ALL){
        for (int i=0; i < __mInEdges.size(); ++i) {
            adj_list.push_back(__mInEdges[i]->get_source_node()->getId());
        }
    }

    if (nType == OUT || nType == ALL){
        for (int i=0; i < __mOutEdges.size(); ++i) {
            adj_list.push_back(__mOutEdges[i]->get_destination_node()->getId());
        }
    }

    return adj_list;
}

// Remove all edges
void Node::unlink_all_edges()
{
    __mInEdges.clear();
    __mOutEdges.clear();
}

// Get the summation of capacity of all outgoing edges.
// If filter given, only the node in the list
// will be considered for calculation
size_t Node::get_out_edges_capacity(
                            std::set<NodeID>const& filterset=std::set<NodeID>())
{
    size_t capacity = 0;
    for (int i=0; i < __mOutEdges.size(); ++i) {
        if(filterset.count(__mOutEdges[i]->get_destination_node()->getId())) {
            capacity += __mOutEdges[i]->get_current_capacity();
        }
    }
        return capacity;
}

// Get the summation of capacity of all incoming edges.
// If filter given, only the node in the list
// will be considered for calculation
size_t Node::get_in_edges_capacity( std::set<NodeID> const & filterset = std::set<NodeID>())
{
    size_t capacity = 0;
    for (int i=0; i < __mInEdges.size(); ++i) {
        if(filterset.count(__mInEdges[i]->get_source_node()->getId())) {
            capacity += __mInEdges[i]->get_current_capacity();
        }
    }
        return capacity;
}

//Get the edge that links the current node with given node
Edge* Node::get_edge_with_node(Node &node)
{
    for (int i= 0; i < __mInEdges.size(); ++i) {
        if (__mInEdges[i]->is_node_src(&node)) {
            return (__mInEdges[i]);
        }
    }
    for (int i= 0; i < __mOutEdges.size(); ++i) {
        if (__mOutEdges[i]->is_node_dest(&node)) {
            return (__mOutEdges[i]);
        }
    }

    return NULL;
}

/**********************************************************************
 *                     Graph class methods                              *
 **********************************************************************/

// Add new edge in the current graph. If source or destination
// nodes are not exist then it will create node first and create
// edge and update necessary informations
void Graph::add_edge(NodeID src_id, NodeID dest_id, EdgeWeight weight)
{
    Node* src = get_node_from_id(src_id);
    Node* dest = get_node_from_id(dest_id);
    // Add source and destination node if not exists
    if(src == NULL){
        add_node(src_id);
        src = get_node_from_id(src_id);
    }
    if(dest == NULL){
        add_node(dest_id);
        dest = get_node_from_id(dest_id);
    }

    // Create edage
    Edge* edge = new Edge(weight, src, dest);

    // Updating edges information in nodes
    src->add_outgoing_edge(edge);
    dest->add_incoming_edge(edge);
}

// Add new node. If maximum node is created already
// then it won't create
// NOTE: The maximun edges constraint is used yet.
void Graph::add_node(NodeID node_id)
{
    Node* node = new Node(node_id);
    if(__mNodes.size() < __mMaxNodes){
        __mNodes.insert(std::pair<NodeID, Node*>(node_id, node));
    }
    else{
        std::cout << "\nError :Maximum nodes are added already\n";
    }
}

// Remove edge b/w given src and dest
void Graph::delete_edge(Node& src_node, Node& dest_node)
{
    src_node.remove_edge_with_node(dest_node);
    dest_node.remove_edge_with_node(src_node);
}

// Remove edge (id version)
void Graph::delete_edge(unsigned int src_id, unsigned int dest_id)
{
    // Get source and destination node
    Node *dest = NULL;
    Node *src = get_node_from_id(src_id);
    if (src) {
        dest = get_node_from_id(dest_id);
        if(dest){
            this->delete_edge(*src, *dest);
            return;
        }
    }
    std::cout << "\nError: No valid edge b/w from the src and dest";
    return;
}

// Remove edge (Egde& version)
void Graph::delete_edge(Edge& edge)
{
    this->delete_edge(*(edge.get_source_node()),
            *(edge.get_destination_node()));
}

// Delete node from graph that results in removing all edges that
// the given node links with other node
void Graph::delete_node(Node& node)
{
    NodeIDList adj_list = node.get_adjency_list();
    for (int i = 0; i < adj_list.size(); ++i) {
        NodeListIter nIter = __mNodes.find(adj_list[i]);
        if (nIter != __mNodes.end()) {
            (nIter->second)->remove_edge_with_node(node);
        }
    }
    node.unlink_all_edges();
    // Remove from the Nodes list
    __mNodes.erase(node.getId());
}

// Delete node (id version)
void Graph::delete_node(unsigned int node_id)
{
    Node *node = get_node_from_id(node_id);
    if (node){
        this->delete_node(*node);
    }
    else{
        std::cout << "\nError: Node not exists in graph";
    }
}

// Reset visited bit for all nodes
void Graph::reset_traversal()
{
    NodeListIter nIter = __mNodes.begin();
    for(; nIter != __mNodes.end(); ++nIter) {
        (nIter->second)->set_visited(false);
    }
}

// Get all paths from src and destination using recursive
void Graph::__form_path(Node& src, Node& dest, std::vector<Path>& nPathList,
        Path& nPath)
{
    src.set_visited(true);
    nPath.push_back(src.getId());

    if (src.getId() == dest.getId()) {
          nPathList.push_back(nPath);
    }
    else {
        NodeIDList outNodes = src.get_adjency_list(OUT);
        for (int i = 0; i < outNodes.size(); ++i) {
            Node* node = get_node_from_id(outNodes[i]);
            if ( (node) && (!(node->is_visited())) ) {
                __form_path(*node, dest, nPathList, nPath);
            }
        }
    }
    src.set_visited(false);
    nPath.pop_back();
}

// Get all paths to filter all nodes that not involved in the paths
std::vector<Path> Graph::get_paths(Node& src_node, Node& dest_node)
{
    std::vector<Path> nPathList;
    Path nPath;
    this->__form_path(src_node, dest_node, nPathList, nPath);
    return nPathList;
}

// Get Path (id version)
std::vector<Path> Graph::get_paths(NodeID src_id, NodeID dest_id)
{
    Node *dest = NULL;
    Node *src = get_node_from_id(src_id);
    if (src) {
        dest = get_node_from_id(dest_id);
        if(dest) {
            return (this->get_paths(*src, *dest));
        }
    }
    return std::vector<Path>();
}

// Return node object from node id
Node* Graph::get_node_from_id(NodeID node_id)
{
    NodeListIter nIter = __mNodes.find(node_id);
    if(nIter != __mNodes.end()) {
        return (nIter->second);
    }
    else{
        return NULL;
    }
}

/**********************************************************************
 *               EdgeSimualtionProperty class methods                 *
 **********************************************************************/

EdgeCapacity EdgeSimualtionProperty::get_current_capacity()
{
    return _mCurrentCapacity;
}

// Increment/Decrement capacity by given count
void EdgeSimualtionProperty::increment_current_capacity()
{
    _mCurrentCapacity += 1;
}

void EdgeSimualtionProperty::decrement_current_capacity()
{
   if (_mCurrentCapacity > 0){
       _mCurrentCapacity -= 1;
   }
   else{
      _mCurrentCapacity = 0;
   }
}

void EdgeSimualtionProperty::set_current_capacity(EdgeCapacity capacity)
{
    _mCurrentCapacity = capacity;
}

/**********************************************************************
 *               NodeSimualtionProperty class methods                 *
 **********************************************************************/

size_t NodeSimualtionProperty::get_no_pkts_to_be_sent()
{
    return _mNoOfPktToBeSent;
}

void NodeSimualtionProperty::set_no_pkts_to_be_sent(size_t count)
{
    _mNoOfPktToBeSent = count;
}
// Increment/Decrement number of pkts to be sent
// by given count
void NodeSimualtionProperty::increment_no_pkts_to_be_sent()
{
    _mNoOfPktToBeSent += 1;
}

void NodeSimualtionProperty::decrement_no_pkts_to_be_sent()
{
   if (_mNoOfPktToBeSent > 0){
       _mNoOfPktToBeSent -= 1;
   }
   else{
      _mNoOfPktToBeSent = 0;
   }
}

size_t NodeSimualtionProperty::get_total_pkts_sent()
{
    return _mTotalNoOfPktSent;
}

void NodeSimualtionProperty::set_total_pkts_sent(size_t count)
{
    _mTotalNoOfPktSent = count;
}

// Increment total number of pkts sent so far
// by given count
void NodeSimualtionProperty::increment_total_pkts_sent()
{
    _mTotalNoOfPktSent += 1;
}


size_t NodeSimualtionProperty::get_total_pkts_received()
{
    return _mTotalNoOfPktRecv;
}

void NodeSimualtionProperty::set_total_pkts_received(size_t count)
{
    _mTotalNoOfPktRecv = count;
}
// Increment total number of pkts sent so far
// by given count
void NodeSimualtionProperty::increment_total_pkts_received()
{
    _mTotalNoOfPktRecv += 1;
}

/**********************************************************************
 *                     Log class methods                              *
 **********************************************************************/

std::string Log::ftos(float n)
{
    std::ostringstream s;
    s << n;
    return s.str();
}

std::string Log::itos(size_t n)
{
    std::ostringstream s;
    s << n;
    return s.str();
}

void Log::print(const std::string& str)
{
    __mCout << str << "\n";
}

// Print the given string in the center of given width
// Will be used to print table
std::string Log::center(const std::string str, int width)
{
    std::stringstream ss, spaces;
    int padding = width - str.size();

    for(int i=0; i<padding/2; ++i){
        spaces << " ";
    }

    ss << spaces.str() << str << spaces.str();

    if(padding>0 && padding%2!=0){
        ss << " ";
    }
    return ss.str();
}

// Print the heading of the table
// It will be printed for every cycle
void Log::print_heading(int cycle_no)
{
    std::string fill(__mTotalWidth, '-');
    std::string space((__mTotalWidth-30)/2, ' ');
    // Print heading information
    __mCout << fill << "\n\n\n";
    __mCout << space << "NODES STATISTICS - CYCLE " << cycle_no << space << "\n";
    __mCout << fill << "\n";
    __mCout << center("NODE #", 10) << "|";
    __mCout << center("TOTAL PKTs SENT", 19) << "|";
    __mCout << center("TOTAL PKTs RECEIVED", 23) << "|";
    __mCout << center("TOTAL PKTs REMAINING", 24) << "|";
    __mCout << center("PKT DELIVERY PERCENT", 25) << "|\n";
    __mCout << fill << "\n";
}

// Print the statistics of node
void Log::print_statistics(Node* node)
{
    // Printing statistics
    __mCout << center(itos(node->getId()), 10) << "|";
    __mCout << center(itos(node->get_total_pkts_sent()), 19) << "|";
    __mCout << center(itos(node->get_total_pkts_received()), 23) << "|";
    __mCout << center(itos(node->get_no_pkts_to_be_sent()), 24) << "|";
    size_t pkt_recv = node->get_total_pkts_received();
    if (pkt_recv) {
                float ratio = ((float)node->get_total_pkts_sent()/(float)pkt_recv) * 100.00;
        __mCout << center(ftos(ratio), 25) << "|\n";
    }
    else {
        __mCout << center("NA", 24) << " |\n";
    }

}

/**********************************************************************
 *                     Simulation class methods                       *
 **********************************************************************/
// Update source and destination for current flow
bool Simulation::update_src_and_dest(NodeID src, NodeID dest)
{
    Node* src_node = __mGraph->get_node_from_id(src);
    Node* dest_node = __mGraph->get_node_from_id(dest);

    __mSrc = src;
    __mDest = dest;

    // Set the number of pkts to be sent to infinity
    // (999999 max no) for source node
    src_node->set_no_pkts_to_be_sent(INFINITY);

    // Clear the filtered node list of previous flow
    __mNodeSet.clear();
    // Update all nodes that involves in the current flow
    __update_node_set(src, dest);

    if(!__mNodeSet.size()){
        std::cout << "\nInfo : No path to reach destionation " << __mDest;
        std::cout << " from source " << __mSrc << "\n\n";
        return false;
    }
    return true;
}

// Private version
void Simulation::__update_node_set(NodeID src, NodeID dest)
{
    std::vector<Path> nPaths = __mGraph->get_paths(src, dest);

    // Print all paths from src to dest
    __mLog.print("List of paths the data will be transfering:\n");
    __mLog.print("-------------------------------------------\n");

    // Get all paths b/w src and dest from graph and update
    // node set with unique node ids (to process the data flow)
    for (int i = 0; i < nPaths.size(); ++i) {
                std::string str;
        for (int j = 0; j < nPaths[i].size(); ++j) {
            str.append(__mLog.itos(nPaths[i][j]));
            str.append(" ----> ");
            __mNodeSet.insert(nPaths[i][j]);
        }
        __mLog.print(str);
    }
    __mLog.print("\n");
}

NodeID Simulation::get_src()
{
    return __mSrc;
}

NodeID Simulation::get_dest()
{
    return __mDest;
}

// Start the data flow b/w src and dest for given number of cycle
void Simulation::start(NodeID src, NodeID dest, size_t no_of_cylce)
{
    if(!update_src_and_dest(src, dest)){
       return;
    }
    for (int i = 0; i < no_of_cylce; ++i) {
        __start_arbitration();
        __mLog.print_heading(i);
        __consume_data();
    }
}

std::set<NodeID>& Simulation::get_node_set()
{
    return __mNodeSet;
}

// Main function to distribute the data b/w all nodes
void Simulation::__distribute_data(Node* node, size_t no_pkt_tbs)
{
    // Get the outgoing capcity of all edges that linked with
    // given node
    size_t out_capacity = node->get_out_edges_capacity(get_node_set());
    // Get all outgoing adjency list
    NodeIDList nListOut = node->get_adjency_list(OUT);
    NodeIDList nListIn;
    size_t inListSize = 0;
    // Node version of list
    std::vector<Node*> NodeListOut, NodeListIn;

    // Exclude the incoming edges for src node
    if (node->getId() != __mSrc){
        nListIn = node->get_adjency_list(IN);
        inListSize = nListIn.size();
    }

    // Get node version of all in and out adjency list
    for(int i = 0; i < nListOut.size(); ++i){
        if(__mNodeSet.count(nListOut[i])) {
            NodeListOut.push_back(__mGraph->get_node_from_id(nListOut[i]));
        }
    }

    for(int i = 0; i < inListSize; ++i){
        if(__mNodeSet.count(nListIn[i])) {
            NodeListIn.push_back(__mGraph->get_node_from_id(nListIn[i]));
        }
    }

    /*
       This will do the following
       1. distribute the total number of pkts to be sent across
          all outgoing nodes one-by-one
       2. Update the below parameters in the outgoing node
          * total number of pkt received by 1
          * total number of pkt to be sent by 1
          * decrease its current capacity by
       3. Update the below parameters in current node
          * total number of pkt sent
       4. In parallel, increase the current capcity of incoming node
          by 1, so that current node can receive data for next cycle

       this iwll be repeated until all out nodes capacity over or
       total number of pkts to be sent is zero
     */
    while (out_capacity && no_pkt_tbs)
    {
        for (int i = 0, j=0; i < NodeListOut.size(); ++i) {

            Edge* InEdge = NULL;
            Edge* OutEdge = NodeListOut[i]->get_edge_with_node(*node);
            if (!NodeListIn.empty()){
                inListSize = NodeListIn.size();
                InEdge = NodeListIn[j]->get_edge_with_node(*node);
            }

            if (OutEdge->get_current_capacity()) {
                // Mark the out node so that in the current node
                // wont' be processed
                NodeListOut[i]->set_visited(true);
                // Should not chnage the current capacity and pkt to
                // be sent for destination
                if(NodeListOut[i]->getId() != __mDest){
                    OutEdge->decrement_current_capacity();
                    NodeListOut[i]->increment_no_pkts_to_be_sent();
                }
                NodeListOut[i]->increment_total_pkts_received();
                node->increment_total_pkts_sent();
                // Matching no of in nodes with out nodes
                if (InEdge){
                    InEdge->increment_current_capacity();
                    ++j;
                    j = j%inListSize;
                }
                --out_capacity;
                --no_pkt_tbs;
                if ((!no_pkt_tbs) || (!out_capacity)){
                   break;
                }

            }
        }
    }
    // Update the no of pkt to be sent if any remaining
    // so that will be processed in next cycle.
    // This is not applicable for source as it has infinte pkt to be sent
    if (node->getId() != __mSrc) {
        node->set_no_pkts_to_be_sent(no_pkt_tbs);
    }
}

// Start arbitration b/w all nodes (phase -1)
void Simulation::__start_arbitration()
{
    std::set<NodeID>::iterator sIter;
    Node* node;
    for (sIter = __mNodeSet.begin(); sIter != __mNodeSet.end(); ++sIter) {
        node = __mGraph->get_node_from_id(*sIter);
        size_t no_pkt_tbs = node->get_no_pkts_to_be_sent();
        // Destination node will be excluded as it only receives the data
        if((no_pkt_tbs > 0) && (!(node->is_visited())) && (*sIter != __mDest)) {
            __distribute_data(node, no_pkt_tbs);
        }
        node->set_visited(false);
    }
}

// Consume the data (phase-2)
void Simulation::__consume_data()
{
    std::set<NodeID>::iterator sIter;
    Node* node;
    // Print the statistics of all node
    for (sIter = __mNodeSet.begin(); sIter != __mNodeSet.end(); ++sIter)
    {
        node = __mGraph->get_node_from_id(*sIter);
        __mLog.print_statistics(node);
    }
}
