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

void Edge::print_statistics()
{
    std::string  edge;
    edge.append(itos(__mSrc->getId()));
    edge.append("--->");
    edge.append(itos(__mDest->getId()));
    Log::_mCout << center(edge,12) << _mDelimiter;
    Log::_mCout << center(Log::itos(__mWeight),20) << _mDelimiter;
    Log::_mCout << *(dynamic_cast<EdgeSimualtionProperty*>(this));
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
size_t Node::get_out_edges_capacity()
{
    size_t capacity = 0;
    for (int i=0; i < __mOutEdges.size(); ++i) {
        capacity += __mOutEdges[i]->get_weight();
    }
    return capacity;
}

// Get the summation of capacity of all incoming edges.
size_t Node::get_in_edges_capacity()
{
    size_t capacity = 0;
    for (int i=0; i < __mInEdges.size(); ++i) {
        capacity += __mInEdges[i]->get_current_capacity();
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

void Node::print_statistics()
{
    Log::_mCout << Log::center(Log::itos(__mId), 10) << _mDelimiter; 
    Log::_mCout << *(dynamic_cast<NodeSimualtionProperty*>(this));

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
    Edge* edge = src->get_edge_with_node(*dest);
    if(edge == NULL){
        edge = new Edge(weight, src, dest);
        // Updating edges information in nodes
        src->add_outgoing_edge(edge);
        dest->add_incoming_edge(edge);
    }
    else{
        std::cout << "\nInfo: Edge already exists.";
        std::cout << " Updating weight to given value\n";
        edge->set_weight(weight);
    }
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

std::ofstream& operator<<(std::ofstream& fOut,
        const EdgeSimualtionProperty& edge)
{
    fOut << Log::center(Log::itos(edge._mCurrentCapacity),22) << Log::_mDelimiter;
    fOut << Log::center(Log::itos(edge._mNoOfPktsTransferred),22) << Log::_mDelimiter;
    fOut << "\n";
    return fOut;
}

EdgeCapacity EdgeSimualtionProperty::get_current_capacity()
{
    return _mCurrentCapacity;
}

void EdgeSimualtionProperty::set_current_capacity(EdgeCapacity capacity)
{
    _mCurrentCapacity = capacity;
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

size_t EdgeSimualtionProperty::get_no_of_pkts_transferred()
{
    return _mNoOfPktsTransferred;
}

void EdgeSimualtionProperty::set_no_of_pkts_transferred(size_t value)
{
    _mNoOfPktsTransferred = value;
}

// Increment/Decrement capacity by given count
void EdgeSimualtionProperty::increment_no_of_pkts_transferred()
{
    _mNoOfPktsTransferred += 1;
}

void EdgeSimualtionProperty::decrement_no_of_pkts_transferred()
{
    if (_mNoOfPktsTransferred > 0){
        _mNoOfPktsTransferred -= 1;
    }
    else{
        _mNoOfPktsTransferred = 0;
    } 
}

/**********************************************************************
 *               NodeSimualtionProperty class methods                 *
 **********************************************************************/
std::ofstream& operator<<(std::ofstream& fOut,
        const NodeSimualtionProperty& node)
{
    fOut << Log::center(Log::itos(node._mTotalNoOfPktRecv),23) << Log::_mDelimiter;
    fOut << Log::center(Log::itos(node._mTotalNoOfPktSent),19) << Log::_mDelimiter;
    fOut << Log::center(Log::itos(node._mNoOfPktToBeSent),24) << Log::_mDelimiter;
    fOut << "\n";
    return fOut;
}

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
// Static variable initilization
std::ofstream Log::_mCout;
size_t Log::_mTotalWidth = 82;
char Log::_mDelimiter = '|';

bool Log::open(std::string& file)
{
    if(file.size()){
        _mCout.open(file.c_str(), std::ofstream::out);
        if(!_mCout.fail()){
            return true;
        }
    }
    return false;
}

void Log::close()
{
    _mCout.close();
}

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
    _mCout << str ;
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
void Log::print_node_statistics_heading()
{
    std::string fill(_mTotalWidth, '-');
    std::string space((_mTotalWidth-30)/2, ' ');
    _mCout << "\n";
    _mCout << space << "NODE STATISTICS " << space << "\n";
    _mCout << fill << "\n";
    // Print heading information
    _mCout << center("NODE #", 10) << "|";
    _mCout << center("TOTAL PKTs RECEIVED", 23) << "|";
    _mCout << center("TOTAL PKTs SENT", 19) << "|";
    _mCout << center("TOTAL PKTs REMAINING", 24) << "|\n";
    _mCout << fill << "\n";
}

void Log::print_edge_statistics_heading()
{
    std::string fill(_mTotalWidth, '-');
    std::string space((_mTotalWidth-30)/2, ' ');
    _mCout << "\n";
    _mCout << space << "EDGE STATISTICS " << space << "\n";
    _mCout << fill << "\n";
    // Print heading information
    _mCout << center("Edge", 12) << "|";
    _mCout << center("ACTUAL CAPACITY", 20) << "|";
    _mCout << center("CURRENT CAPACITY", 22) << "|";
    _mCout << center("# PKTs TRANSFERRED", 22) << "|\n";
    _mCout << fill << "\n";
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

    // Open file for logging the statistics
    std::string file("flow_");
    file.append(Log::itos(src));
    file.append("_to_");
    file.append(Log::itos(dest));
    file.append("_");

    //append timestamp
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    file.append(buf);
    file.append(".log");

    if(!Log::open(file)) {  
        std::cout << "\nError: Failed to create log file for flow\n";
        return false;
    }

    // Clear the filtered node list of previous flow
    __mNodeSet.clear();
    __mNodeList.clear();
    // Update all nodes that involves in the current flow
    __update_node_set(src, dest);

    if(!__mNodeSet.size()){
        std::cout << "\nInfo : No path to reach destination " << __mDest;
        std::cout << " from source " << __mSrc << "\n\n";
        Log::close();
        std::remove(file.c_str());
        return false;
    }

    // Set the number of pkts to be sent to infinity
    // (999999 max no) for source node
    src_node->set_no_pkts_to_be_sent(INFINITY);

    return true;
}

// Private version
void Simulation::__update_node_set(NodeID src, NodeID dest)
{
    std::vector<Path> nPaths = __mGraph->get_paths(src, dest);

    // Print all paths from src to dest
    Log::print("List of paths the data will be transfering:\n");
    Log::print("-------------------------------------------\n");

    // Get all paths b/w src and dest from graph and update
    // node set with unique node ids (to process the data flow)
    for (int i = 0; i < nPaths.size(); ++i) {
        std::string str;
        for (int j = 0; j < nPaths[i].size(); ++j) {
            NodeID id = nPaths[i][j];
            str.append(Log::itos(id));
            str.append(" ----> ");
            Node * node = __mGraph->get_node_from_id(id);
            size_t out_capacity = node->get_out_edges_capacity(); 
            if(std::find(__mNodeList.begin(), __mNodeList.end(), id) == __mNodeList.end()){
                __mNodeList.push_back(id);
            }
            __mNodeSet.insert(std::pair<NodeID,EdgeCapacity>(id, out_capacity));

        }
        Log::print(str);
        Log::print("\n");
    }
}

NodeID Simulation::get_src()
{
    return __mSrc;
}

NodeID Simulation::get_dest()
{
    return __mDest;
}

void Simulation::print_node_statistics()
{
    Node* node;
    for (int i = 0; i <__mNodeList.size() ; ++i) {
        node = __mGraph->get_node_from_id(__mNodeList[i]);
        if(node == NULL){
            continue;
        }
        node->print_statistics();
    }

}

// Start the data flow b/w src and dest for given number of cycle
void Simulation::start(NodeID src, NodeID dest, size_t no_of_cylce)
{
    if(!update_src_and_dest(src, dest)){
        return;
    }
    for (int i = 0; i < no_of_cylce; ++i) {
        Log::print("\n\nCycle : ");
        Log::print(Log::itos(i));
        Log::print("\n============\n");
        __start_arbitration();
        Log::print_edge_statistics_heading();
        __consume_data();
        Log::print_node_statistics_heading();
        print_node_statistics(); 
    }
    size_t pkt_sent = __mGraph->get_node_from_id(__mSrc)->get_total_pkts_sent();
    size_t pkt_recv = __mGraph->get_node_from_id(__mDest)->get_total_pkts_received();
    Log::print("\n\nFlow statistics:\n");
    Log::print("================\n\n");
    Log::print("Number of data sent from src\t:\t");
    Log::print(Log::itos(pkt_sent));
    Log::print("\n");
    Log::print("Number of data received at dest\t:\t");
    Log::print(Log::itos(pkt_recv));
    Log::print("\n");
    Log::print("Data flow rate from src to dest\t:\t");
    if (pkt_sent){
        Log::print(Log::ftos(((float)pkt_recv/(float)pkt_sent) * 100.00));
        Log::print("%\n\n");
    }
    else{
        Log::print("0.0%\n\n");
    }
    Log::close(); 
}

std::map<NodeID, EdgeCapacity>& Simulation::get_node_set()
{
    return __mNodeSet;
}

// Main function to distribute the data b/w all nodes
void Simulation::__distribute_data(Node* node, size_t no_pkt_tbs)
{
    size_t out_capacity = __mNodeSet[node->getId()];

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
    for(int i = 0; i < nListOut.size(); ++i) {
        // Get node for current out node id
        Node* out_node = __mGraph->get_node_from_id(nListOut[i]);
        if (out_node == NULL) {
            continue;
        }
        // If the current out node is src or it is not
        // node set list, then subtracts that node capacity
        // from out capacity
        if ((nListOut[i] == __mSrc)  || 
                (!__mNodeSet.count(nListOut[i]))) {
            Edge* edge = node->get_edge_with_node(*out_node);
            if(edge != NULL){
                out_capacity -= edge->get_weight(); 
            }
        }
        else {
            // push the out node if it is in the node set
            // and it is not source
            NodeListOut.push_back(out_node);
        }
    }

    for(int k = 0; k < nListIn.size(); ++k) {
        // Get node for current out node id
        Node* in_node = __mGraph->get_node_from_id(nListIn[k]);
        if (in_node == NULL) {
            continue;
        }
        // If the current out node is src or it is not
        // node set list, then subtracts that node capacity
        // from out capacity
        if ((nListIn[k] != __mDest)  || 
                (__mNodeSet.count(nListIn[k]))) {
            // push the out node if it is in the node set
            // and it is not source
            NodeListIn.push_back(in_node);
        }
    }

    /*
       This will do the following
       1. distribute the total number of pkts to be sent across
       all outgoing nodes one-by-one
       2. Update total number of pkts sent in the current node
       3. Update number of pkts transferred in the outgoing edge
       4. In parallel, increase the current capcity of incoming node
       by 1, so that current node can receive data for next cycle

       this iwll be repeated until all out nodes capacity over or
       total number of pkts to be sent is zero
     */
    size_t nInListSize = NodeListIn.size();
    bool pkt_transferred = false;
    bool all_out_node_processed;

    while (out_capacity && no_pkt_tbs)
    {
        all_out_node_processed = true;
        for (int i = 0, j=0; i < NodeListOut.size(); ++i) {
            pkt_transferred = false;
            Edge* InEdge = NULL;
            Edge* OutEdge  = NodeListOut[i]->get_edge_with_node(*node); 
            if(OutEdge == NULL){
                continue;
            }
            if(nInListSize){
                InEdge = NodeListIn[j]->get_edge_with_node(*node);
            }

            // Ignore capacity check if the out node is destination
            if (NodeListOut[i]->getId() == __mDest) {
                OutEdge->increment_no_of_pkts_transferred();
                pkt_transferred = true;
                all_out_node_processed = false;
            }
            else {
                if(OutEdge->get_current_capacity()) {
                    OutEdge->increment_no_of_pkts_transferred();
                    OutEdge->decrement_current_capacity();
                    pkt_transferred = true;
                    all_out_node_processed = false;
                }
            }
            if ((!no_pkt_tbs) || (!out_capacity)){
                break;
            }

            // Increase corresponding in edge capacity
            if(pkt_transferred) {
                node->increment_total_pkts_sent();
                --out_capacity;
                --no_pkt_tbs;
                if( (InEdge) && 
                        (InEdge->get_weight() > InEdge->get_current_capacity())){
                    InEdge->increment_current_capacity();
                    ++j;
                    j = (j%nInListSize);
                }
            }
        }
        if(all_out_node_processed){
            break;
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
    Node* node;
    for (int i = __mNodeList.size()-1; i >= 0; --i) {
        node = __mGraph->get_node_from_id(__mNodeList[i]);
        if(node == NULL){
            continue;
        }
        size_t no_pkt_tbs = node->get_no_pkts_to_be_sent();
        // Destination node will be excluded as it only receives the data
        if((no_pkt_tbs > 0)  && (__mNodeList[i] != __mDest)) {
            __distribute_data(node, no_pkt_tbs);
        }
    }
}

// Consume the data (phase-2)
void Simulation::__consume_data()
{
    Node* node;
    NodeIDList nListIn;
    // Node version of list
    std::vector<Node*> NodeListIn;

    // Print the statistics of all node
    for (int i = 0; i < __mNodeList.size(); ++i) {
        node = __mGraph->get_node_from_id(__mNodeList[i]);
        if(node == NULL){
            continue;
        }
        // Source node will be excluded as it only sends the data
        if(__mNodeList[i] != __mSrc) {
            nListIn.clear();
            NodeListIn.clear();

            nListIn = node->get_adjency_list(IN);

            // Filter out the in nodes that not involved in the paths
            for(int j = 0; j < nListIn.size(); ++j) {

                Node* in_node = __mGraph->get_node_from_id(nListIn[j]);
                if (in_node == NULL) {
                    continue;
                }
                // If the in node is not in the node set
                // or it is destination
                if ((nListIn[j] != __mDest)  && (__mNodeSet.count(nListIn[j]))) {
                    NodeListIn.push_back(in_node);
                }
            }

            size_t inListSize = NodeListIn.size();
            /*
               It will do the followings:
               1. update the no of pkt received of current node by checking all
               its incoming edge
               2. update the no of the pkt to be sent for current node
             */
            size_t no_of_pkt_received = 0;
            size_t pkts_count  = 0;
            for (int k = 0; k < NodeListIn.size(); ++k) {
                Edge* InEdge = NULL;
                InEdge = NodeListIn[k]->get_edge_with_node(*node);
                if(InEdge == NULL) {
                    continue;
                }

                no_of_pkt_received += InEdge->get_no_of_pkts_transferred();
                InEdge->print_statistics();
                InEdge->set_no_of_pkts_transferred(0);
            }
            // Update the no of pkts to be transferred
            if(__mNodeList[i] != __mDest){
                pkts_count = node->get_no_pkts_to_be_sent();
                pkts_count += no_of_pkt_received;
                node->set_no_pkts_to_be_sent(pkts_count);
            }

            // Update total no of pkts received
            pkts_count = node->get_total_pkts_received();
            pkts_count += no_of_pkt_received;
            node->set_total_pkts_received(pkts_count);
        }
    }
}

