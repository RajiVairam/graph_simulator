# include "cli.hpp"

/**********************************************************************
 *                     Cmd class methods                             *
 **********************************************************************/
// To print the usgae string in formatted way
std::string Cmd::align()
{
    int padding = 0;

    if (_mUsage.size() > 80) {
        padding = 10;
    }
    else{
        padding = 90 - _mUsage.size();
    }
    std::string spaces(padding, ' ');
    return spaces;
}

// Returns command name.
std::string& Cmd::get_name()
{
    return _mName;
}

/**********************************************************************
 *                     Flag class methods                             *
 **********************************************************************/
std::string& Flag::get_usage()
{
    if (_mUsage.empty()) {
        _mUsage.append(_mName);
        _mUsage.append(align());
        _mUsage.append(_mDesc);
    }
    return _mUsage;
}

/**********************************************************************
 *                     Option class methods                           *
 **********************************************************************/
std::string& Option::get_usage()
{
    if (_mUsage.empty()) {
        _mUsage.append(_mName);
        _mUsage.append(" <");
        _mUsage.append(__mShortDesc);
        _mUsage.append(">");

        if (_mDesc.size()){
            _mUsage.append(align());
            _mUsage.append(_mDesc);
        }
    }
    return _mUsage;
}

bool Option::is_required()
{
    return __mRequiredValue;
}

/**********************************************************************
 *                     Title class methods                           *
 **********************************************************************/
std::string& Title::get_usage()
{
    if (_mUsage.empty()) {
        _mUsage.append(_mName);

        for (int i =0; i < __mSubOptions.size(); ++i) {
            _mUsage.append(" ");
            _mUsage.append(__mSubOptions[i]->get_usage());
        }

        _mUsage.append(align());
        _mUsage.append(_mDesc);
    }
    return _mUsage;
}

size_t Title::get_sub_option_count()
{
    return __mSubOptions.size();
}

void Title::add_sub_option(const std::string& name, const std::string& desc,
        const std::string& short_desc, bool required)
{
    // Make the describtion as empty to denote it is sub option
    desc.empty();

    // Create and insert the sub option
    Option *sub_option = new Option(name, desc, short_desc, required);
    __mSubOptions.push_back(sub_option);
}

/**********************************************************************
 *                     Cli class methods                           *
 **********************************************************************/
Cli* Cli::__mPrivateInstance = NULL;

// Get instance
Cli* Cli::Instance()
{
    if (__mPrivateInstance == NULL) {
        __mPrivateInstance = new Cli();
    }
    return __mPrivateInstance;
}

void Cli::add_flag(const std::string& name, const std::string& desc)
{
    // Create and add it in flag list
    Cmd *cmd = new Flag(name, desc);
    __mFlagList.push_back(cmd);
    __mCmdNames.insert(std::pair<std::string, bool>(name, false));
}

void Cli::add_option(const std::string& name, const std::string& desc,
        const std::string& short_desc, bool required)
{
    Cmd *cmd = new Option(name, desc, short_desc, required);
    __mOptionList.push_back(cmd);
    __mCmdNames.insert(std::pair<std::string, bool>(name, required));
}

Title* Cli::add_title(const std::string& name, const std::string& desc)
{
    // Create and return for adding suboption
    Cmd *cmd = new Title(name, desc);
    __mTitleList.push_back(cmd);
    __mCmdNames.insert(std::pair<std::string, bool>(name, false));
    return (dynamic_cast<Title*>(cmd));
}

void Cli::add_title_sub_option(Title* tptr, const std::string& name,
        const std::string& desc,
        const std::string& short_desc, bool required)
{
    // Insert in a title and sub option map
    std::string new_name (tptr->get_name());
    new_name.append(" ");
    new_name.append(name);
    __mCmdNames.insert(std::pair<std::string, bool>(new_name, required));
    // Call function to add sub option in title class.
    tptr->add_sub_option(name, desc, short_desc, required);
}

// Char** version
const char* Cli::get_cmd_value(char** arg_start, char** arg_end,
        const std::string & option)
{
    // Get option's value by extracing the next
    //  available string in input
    char ** ptr = std::find(arg_start, arg_end, option);
    if (ptr != arg_end && (++ptr) != arg_end)
    {
        return *ptr;
    }
    return NULL;
}

// std::sting version
const char* Cli::get_cmd_value(StringList& sList, const std::string& option)
{
    StringList::iterator val = std::find(sList.begin(), sList.end(), option);
    if (val != sList.end() && (++val) != sList.end())
    {
        return val->c_str();
    }
    return NULL;
}

bool Cli::cmd_exists(char** arg_start, char** arg_end,
        const std::string& option)
{
    // Check and return whether the current option
    // is in input or not
    return (std::find(arg_start, arg_end, option) != arg_end);
}

// Usge string by calling each command's usge string
std::string& Cli::usage()
{
    if (__mUsage.empty()) {
        __mUsage.append("\n");
        // For flag
        for (unsigned i = 0; i < __mFlagList.size(); ++i) {
            __mUsage.append("  ");
            __mUsage.append(__mFlagList[i]->get_usage());
            __mUsage.append("\n\n");
        }
        // For command
        for (unsigned i = 0; i < __mOptionList.size(); ++i) {
            __mUsage.append("  ");
            __mUsage.append(__mOptionList[i]->get_usage());
            __mUsage.append("\n\n");
        }
        // For Title (includes suboption)
        for (unsigned i = 0; i < __mTitleList.size(); ++i) {
            __mUsage.append("  ");
            __mUsage.append(__mTitleList[i]->get_usage());
            __mUsage.append("\n\n");
        }
    }
    return __mUsage;
}

// Validates whether all sub options are given
// if corresponding title command is given
bool Cli::validate_title_sub_options()
{
    // Iterate through all title command list
    for(int i = 0 ; i < __mTitleList.size(); ++i) {
        std::string name = __mTitleList[i]->get_name();
        if(__mValues.count(name)) {
            size_t count = (dynamic_cast<Title*>(__mTitleList[i]))->get_sub_option_count();
            if  (__mValues.size() != (count +1)){
                std::cout << "\nError: Suboption is not given properly for " <<  name << "\n";
                return false;
            }
        }
    }
    return true;
}

// Get value of given option
const char* Cli::get_value(const std::string& option)
{
    std::map<std::string, std::string>::iterator it = __mValues.find(option);

    if (it != __mValues.end()){
        return (it->second).c_str();
    }
    else {
        return NULL;
    }
}

// Clear parsed results
void Cli::reset_values()
{
    __mValues.clear();
}

// Parse commands
bool Cli::parse_cmds(std::string& buf)
{
    // Spiliting the arguments
    std::istringstream instr(buf);
    StringList argv;
    std::string s;
    while(instr >> s){
        argv.push_back(s);
    }
    int argc = argv.size();

    // Check for all command names in given input
    std::map<std::string, bool>::iterator nIter = __mCmdNames.begin();

    for (; nIter != __mCmdNames.end(); ++nIter ){
        // If it is exists
        std::string name;
        size_t pos = (nIter->first).find(" ");
        if (pos != std::string::npos) {
            name = (nIter->first).substr(pos+1);
        }
        else {
            name = (nIter->first);
        }

        if (std::find(argv.begin(), argv.end(), name) != argv.end()) {
            // If the value is not required
            if(!nIter->second) {
                // Update value as empty
                __mValues.insert(std::pair<std::string, std::string>
                        (name, ""));
            }
            else { // If required get the value from input
                const char * value = get_cmd_value(argv, name);
                if (value) {
                    std::string sval(value);
                    __mValues.insert(std::pair<std::string, std::string>(name, sval));
                }
                else{ //If value is missing issue the error
                    std::cout << "\nError : Missing argument for " << name << "\n";
                    return false;
                }
            }
        }
    }
    // If some options is updated, then look for
    // sub options of all titles
    if (__mValues.size()) {
        return validate_title_sub_options();
    }
    else { // If notning is upated, then it is in valid arguments
        std::cout << "\nError : Invalid argument is given\n";
        usage();
        return false;
    }
}

/**********************************************************************
 *                     Utility Cli Functions                          *
 **********************************************************************/

// Static information to be updated
void update_available_option(Cli* cli)
{
    // First create title and create its sub options
    // Then link both
    //
    //  Example:
    //
    // create_graph -max_node [MAX_NODE] -max_edge [MAX_EDGE]  Create new graph with given max node and max edge
    //
    Title* title = cli->add_title("create_graph",
            "Create new graph with given max node and max edge");
    cli->add_title_sub_option(title, "-max_node", "", "MAX_NODE", true);
    cli->add_title_sub_option(title, "-max_edge", "", "MAX_EDGE", true);

    // Delete graph
    cli->add_flag("delete_graph", "Delete the current graph");

    // Add Node
    cli->add_option("add_node","Add a new node to the graph", "NODE_ID", true);

    // Delete Node
    cli->add_option("delete_node", "Delete the given node from the graph",
            "NODE_ID", true);
    // Add Edge
    title = cli->add_title("add_edge",
            "Add a new edage to the graph b/w src and dest");
    cli->add_title_sub_option(title, "-src_node", "", "SRC_NODE_ID", true);
    cli->add_title_sub_option(title, "-dest_node", "", "DEST_NODE_ID", true);
    cli->add_title_sub_option(title, "-weight", "", "WEIGHT", true);

    // Delete Edge
    title = cli->add_title("delete_edge", "Delete the given edge from the graph");
    cli->add_title_sub_option(title, "-src_node", "", "SRC_NODE_ID", true);
    cli->add_title_sub_option(title, "-dest_node", "", "DEST_NODE_ID", true);

    // start flow
    title = cli->add_title("start_flow",
            "Start data flow b/w src and dest for given duration (in sec)");
    cli->add_title_sub_option(title, "-src_node", "", "SRC_NODE_ID", true);
    cli->add_title_sub_option(title, "-dest_node", "", "DEST_NODE_ID", true);
    cli->add_title_sub_option(title, "-time", "", "DURATION", true);

    // More commands can be added
}
