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
    Log::_mCout << center(edge,20) << _mDelimiter;
    Log::_mCout << center(Log::itos(__mWeight),18) << _mDelimiter;
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
void Node::remove_edge_with_node(Node& anode, ADJNODETYPE type=ALL)
{
    if((type == IN)||(type == ALL)) {
        for (int i= 0; i < __mInEdges.size(); ++i) {
            if (__mInEdges[i]->is_node_src(&anode)) {
                __mInEdges.erase(__mInEdges.begin()+i);
                break;
            }
        }
    }
    if((type == OUT)||(type == ALL)) {
        for (int i= 0; i < __mOutEdges.size(); ++i) {
            if (__mOutEdges[i]->is_node_dest(&anode)) {
                __mOutEdges.erase(__mOutEdges.begin()+i);
                break;
            }
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
        capacity += __mInEdges[i]->get_weight();
    }
    return capacity;
}

//Get the edge that links the current node with given node
Edge* Node::get_edge_with_node(Node &node, ADJNODETYPE type=ALL)
{
    if((type == IN)||(type == ALL)) {
        for (int i= 0; i < __mInEdges.size(); ++i) {
            if (__mInEdges[i]->is_node_src(&node)) {
                return (__mInEdges[i]);
            }
        }
    }
    if((type == OUT)||(type == ALL)) {
        for (int i= 0; i < __mOutEdges.size(); ++i) {
            if (__mOutEdges[i]->is_node_dest(&node)) {
                return (__mOutEdges[i]);
            }
        }
    }

    return NULL;
}

// print the node statistics
void Node::print_statistics()
{
    Log::_mCout << Log::center(Log::itos(__mId), 10) << _mDelimiter;
    Log::_mCout << *(dynamic_cast<NodeSimualtionProperty*>(this));

}

// Get the out edges list
EdgeList Node::get_out_edge_list()
{
    return __mOutEdges;
}

// Get the in edges list
EdgeList Node::get_in_edge_list()
{
    return __mInEdges;
}

void Node::reset_edge_counters()
{
    for (int i=0; i < __mInEdges.size(); ++i) {
        __mInEdges[i]->set_no_of_pkts_transferred(0);
        __mInEdges[i]->set_total_pkts_transferred(0);
    }

    for (int i=0; i < __mOutEdges.size(); ++i) {
        __mOutEdges[i]->set_no_of_pkts_transferred(0);
        __mOutEdges[i]->set_total_pkts_transferred(0);
    }

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
        if(!add_node(src_id)){
            return;
        }
        src = get_node_from_id(src_id);
    }
    if(dest == NULL){
        if(!add_node(dest_id)){
            return;
        }
        dest = get_node_from_id(dest_id);
    }

    // Create edage
    Edge* edge = src->get_edge_with_node(*dest, OUT);
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
// NOTE: The maximun edges constraint yet to be added
bool Graph::add_node(NodeID node_id)
{
    if(__mNodes.size() < __mMaxNodes){
        Node* node = new Node(node_id);
        __mNodes.insert(std::pair<NodeID, Node*>(node_id, node));
        return true;
    }
    else{
        std::cout << "\nError: Maximum nodes are added already\n";
        return false;
    }
}

// Remove edge b/w given src and dest
void Graph::delete_edge(Node& src_node, Node& dest_node)
{
    src_node.remove_edge_with_node(dest_node, OUT);
    dest_node.remove_edge_with_node(src_node, IN);
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
            (nIter->second)->remove_edge_with_node(node, ALL);
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
    fOut << Log::center(Log::itos(edge._mNoOfPktsTransferred),20) << Log::_mDelimiter;
    fOut << Log::center(Log::itos(edge._mTotalPktsTransferred),20) << Log::_mDelimiter;
    fOut << "\n";
    return fOut;
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

size_t EdgeSimualtionProperty::get_total_pkts_transferred()
{
    return _mTotalPktsTransferred;
}

void EdgeSimualtionProperty::set_total_pkts_transferred(size_t value)
{
    _mTotalPktsTransferred = value;
}

// Increment/Decrement capacity by given count
void EdgeSimualtionProperty::increment_total_pkts_transferred()
{
    _mTotalPktsTransferred += 1;
}

/**********************************************************************
 *               NodeSimualtionProperty class methods                 *
 **********************************************************************/
std::ofstream& operator<<(std::ofstream& fOut,
        const NodeSimualtionProperty& node)
{
    fOut << Log::center(Log::itos(node._mTotalNoOfPktRecv),23) << Log::_mDelimiter;
    fOut << Log::center(Log::itos(node._mTotalNoOfPktSent),19) << Log::_mDelimiter;
    if(node._mNoOfPktToBeSent == (unsigned int)(-1)){
        fOut << Log::center("Infinity", 26) << Log::_mDelimiter;
    }
    else{
        fOut << Log::center(Log::itos(node._mNoOfPktToBeSent),26) << Log::_mDelimiter;
    }
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
    _mCout << center("TOTAL PKTs REMAINING", 26) << "|\n";
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
    _mCout << center("Edge", 20) << "|";
    _mCout << center("WEIGHT", 18) << "|";
    _mCout << center("# PKTs TXed", 20) << "|";
    _mCout << center("TOTAL PKTs TXed", 20) << "|\n";
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

    // Update all nodes that involves in the current flow
    __update_node_set(src, dest);

    // Reset total pkt transferred and received for nodes
    reset_counters();

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
            __mNodeSet.insert(id);
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

// Print all node statistics
void Simulation::print_node_statistics()
{
    Node* node;
    std::set<NodeID>::iterator sIter;
    for (sIter = __mNodeSet.begin(); sIter != __mNodeSet.end(); ++sIter) {
        node = __mGraph->get_node_from_id(*sIter);
        if(node == NULL){
            continue;
        }
        node->print_statistics();
    }

}

void Simulation::reset_counters()
{
    std::set<NodeID>::iterator sIter;
    for (sIter = __mNodeSet.begin(); sIter != __mNodeSet.end(); ++sIter) {
        Node* node = __mGraph->get_node_from_id(*sIter);
        if(node == NULL) {
            continue;
        }
        node->set_total_pkts_received(0);
        node->set_total_pkts_sent(0);
        node->set_no_pkts_to_be_sent(0);
        node->reset_edge_counters();
    }
}

// Start the data flow b/w src and dest for given number of cycle
void Simulation::start(NodeID src, NodeID dest, size_t no_of_cylce)
{
    if(!update_src_and_dest(src, dest)){
        return;
    }

    for (int i = 0; i < no_of_cylce; ++i) {
        //Print the header info
        Log::print("\n\nCycle : ");
        Log::print(Log::itos(i+1));
        Log::print("\n============\n");

        // Start to send the data b/w nodes (phase-1)
        __start_arbitration();

        // Print the edge statistics heading
        Log::print_edge_statistics_heading();

        // Consume the data from nodes(phase-2)
        __start_consumption();

        // Print node statistics heading
        Log::print_node_statistics_heading();

        // Print all node statistics
        print_node_statistics();
    }

    // Print the flow rate b/w src and dest at end of flow
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

    // Close the log
    Log::close();
}

// Start arbitration b/w all nodes
void Simulation::__start_arbitration()
{
    Node* node;
    std::set<NodeID>::iterator sIter;

    // Process each nodes one by one
    for (sIter = __mNodeSet.begin(); sIter != __mNodeSet.end(); ++sIter) {
        node = __mGraph->get_node_from_id(*sIter);

        if(node == NULL){
            continue;
        }

        // Get the number of pkts to be sent. If it is zero
        // no need to distribute data
        size_t no_pkt_tbs = node->get_no_pkts_to_be_sent();

        // Destination node will be excluded as it only receives the data
        if((no_pkt_tbs > 0)  && (*sIter != __mDest)) {
            __distribute_data(node, no_pkt_tbs);
        }
    }
}

// Main function to distribute the data b/w all nodes
void Simulation::__distribute_data(Node* node, size_t no_pkt_tbs)
{
    EdgeList out_list = node->get_out_edge_list();

    // Filtered-out the nodes that are not involving in the
    // data flow and stores remaining
    EdgeList filtered_out_list;
    size_t out_capacity = 0;
    size_t actual_pkt_tbs = no_pkt_tbs;

    for(int i = 0; i < out_list.size(); ++i) {
        if(__mNodeSet.count(out_list[i]->get_destination_node()->getId())) {
            out_capacity += out_list[i]->get_weight();
            out_list[i]->set_no_of_pkts_transferred(0);
            filtered_out_list.push_back(out_list[i]);
        }
    }

    size_t out_edge_cout = filtered_out_list.size();

    while(no_pkt_tbs && out_capacity) {
        // Divide pockets and send equally across all edges
        // divide the total pockets to send across all
        // out edges
        size_t pkt_divide = no_pkt_tbs/out_edge_cout;
        for(int k = 0; k < out_edge_cout; ++k) {

            // if the no of pkts to be sent is less the number
            // of edge count, then send 1 pkt to all nodes until
            // pkt is available (some edge won't get pkt in this case)
            if(pkt_divide == 0) {
                pkt_divide = 1;
            }

            size_t weight = filtered_out_list[k]->get_weight();
            size_t pkt_transferred = filtered_out_list[k]->get_no_of_pkts_transferred();
            size_t current_weight = weight - pkt_transferred;

            if(current_weight > pkt_divide){
                pkt_transferred += pkt_divide;
                filtered_out_list[k]->set_no_of_pkts_transferred(pkt_transferred);

                // Update total pkts transferred
                size_t total_pkts_transferred =
                    filtered_out_list[k]->get_total_pkts_transferred();
                total_pkts_transferred += pkt_divide;
                filtered_out_list[k]->set_total_pkts_transferred(total_pkts_transferred);

                // Decrment the no of pkts to be sent and out capacity
                no_pkt_tbs -= pkt_divide;
                out_capacity -= pkt_divide;
            }
            else {
                pkt_transferred += current_weight;
                filtered_out_list[k]->set_no_of_pkts_transferred(pkt_transferred);

                // Update total pkts transferred
                size_t total_pkts_transferred =
                    filtered_out_list[k]->get_total_pkts_transferred();
                total_pkts_transferred += current_weight;
                filtered_out_list[k]->set_total_pkts_transferred(total_pkts_transferred);

                // Decrment the no of pkts to be sent and out capacity
                no_pkt_tbs -= current_weight;
                out_capacity -=  current_weight;
            }

            if((!no_pkt_tbs) || (!out_capacity)) {
                break;
            }
        }
    }

    // Update total number of pkts sent count
    size_t total_pkt_sent = node->get_total_pkts_sent();
    total_pkt_sent += (actual_pkt_tbs - no_pkt_tbs);
    node->set_total_pkts_sent(total_pkt_sent);

    // Update the no of pkt to be sent if any remaining
    // so that will be processed in next cycle.
    // This is not applicable for source as it has infinte pkt to be sent
    if (node->getId() != __mSrc) {
        node->set_no_pkts_to_be_sent(no_pkt_tbs);
    }
}

// Consume the data (phase-2)
void Simulation::__start_consumption()
{
    Node* node;
    std::set<NodeID>::iterator sIter;

    // Process each nodes one by one
    for (sIter = __mNodeSet.begin(); sIter != __mNodeSet.end(); ++sIter) {
        node = __mGraph->get_node_from_id(*sIter);
        if(node == NULL){
            continue;
        }
        // Destination node will be excluded as it only receives the data
        if((*sIter != __mSrc)) {
            __consume_data(node);
        }
    }
}

void Simulation::__consume_data(Node* node)
{
    EdgeList in_list = node->get_in_edge_list();

    // Filtered-out the nodes that are not involving in the
    // data flow and stores remaining
    EdgeList filtered_in_list;

    for(int i = 0; i < in_list.size(); ++i) {
        if(__mNodeSet.count(in_list[i]->get_destination_node()->getId())) {
            filtered_in_list.push_back(in_list[i]);
        }
    }

    size_t pkt_received = 0;
    for(int k = 0; k < filtered_in_list.size(); ++k) {
        pkt_received += filtered_in_list[k]->get_no_of_pkts_transferred();
        filtered_in_list[k]->print_statistics();
        filtered_in_list[k]->set_no_of_pkts_transferred(0);
    }
    // Update total number of pkts received count
    size_t total_pkt_received = node->get_total_pkts_received();
    total_pkt_received += pkt_received;
    node->set_total_pkts_received(total_pkt_received);

    // Update the no of pkt to be sent
    if (node->getId() != __mDest) {
        size_t no_pkt_tbs = node->get_no_pkts_to_be_sent();
        no_pkt_tbs += pkt_received;
        node->set_no_pkts_to_be_sent(no_pkt_tbs);
    }
}

