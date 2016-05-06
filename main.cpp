# include "cli.hpp"
# include "graph.hpp"

// Global pointer for graph and simulation
static Graph* gGraph = NULL;
static Simulation* gSimulation = NULL;

void parse_cmd_and_execute(Cli* cli, std::string& buf)
{
    // Returns, if parsing failed due to invalid options
    if(!cli->parse_cmds(buf)){
        return;
    }

    // Command to create graph
    if(cli->get_value("create_graph"))    {
        int max_node = atoi(cli->get_value("-max_node"));
        int max_edge = atoi(cli->get_value("-max_edge"));

        if (gGraph){
            std::cout << "Already graph created\n";
            return;
        }
        gGraph = new Graph(max_node, max_edge);
        return;
    }
    // All other command is available if the graph is created
    if(gGraph){
        if (cli->get_value("add_node")){
            NodeID id = atoi(cli->get_value("add_node"));
            gGraph->add_node(id);
            return;
        }

        if (cli->get_value("delete_node")){
            NodeID id = atoi(cli->get_value("delete_node"));
            gGraph->delete_node(id);
            return;
        }

        if (cli->get_value("add_edge")){
            NodeID src = atoi(cli->get_value("-src_node"));
            NodeID dest = atoi(cli->get_value("-dest_node"));
            EdgeWeight weight = atoi(cli->get_value("-weight"));
            gGraph->add_edge(src, dest, weight);
            return;
        }

        if (cli->get_value("delete_edge")){
            NodeID src = atoi(cli->get_value("-src_node"));
            NodeID dest = atoi(cli->get_value("-dest_node"));
            gGraph->delete_edge(src, dest);
            return;
        }

        if (cli->get_value("delete_graph")){
            delete gGraph;
            gGraph = NULL;
            return;
        }
        // Simulate option
        // NOTE: The option time is now used as number of cycles
        // in the data flow
        if (cli->get_value("start_flow")){
            NodeID src = atoi(cli->get_value("-src_node"));
            NodeID dest = atoi(cli->get_value("-dest_node"));
            size_t time = atoi(cli->get_value("-time"));
            // Create simulation object if it is not there.
            if(gSimulation == NULL)    {
                gSimulation = new Simulation(gGraph);
            }
            gSimulation->start(src, dest, time);
            return;        }
    }
    else{
        std::cout << "\nError : The graph is not created. Create graph first\n";
        return;
    }
}

void start_batch_mode(Cli* cli, const char* file)
{
    // Open file and read commands line by line
    std::ifstream infile(file);
    if(infile.fail()) {
        std::cout << "\nError: Failed to open file " << file << "\n";
        return;
    }
    std::string str;
    while (std::getline(infile, str)) {
        if(str.size()) {
            cli->reset_values();
            parse_cmd_and_execute(cli, str);
        }
    }
}

// Start interactive mode
void start_interactive(Cli* cli)
{
    std::string str;
    // Process until user enters exit.
    while ( std::cin ) {
        // Print the prompt
        std::cout << "[n/w simulator]# ";
        std::getline(std::cin, str);
        // Exit from loop if input is exit
        if (!strcmp(str.c_str(), "exit")){
            break;
        }
        else if(!strcmp(str.c_str(), "help")){
            // Print the all command available.
            // This help is different from the one
            // in the main
            std::cout << cli->usage();
        }
        else if(!strcmp(str.c_str(), "?")){
            std::cout << cli->usage();
        }
        else if(!str.size()){
            // Skip empty line
            continue;
        }
        else{
            // Parse and execute for each commands
            parse_cmd_and_execute(cli, str);
            cli->reset_values();
        }
    }
}

/*
 * This gives two options to user
 * 1. interactive mode
 * 2. batch mode
 *
 * The available commands can be used from any one
 * of the above mode
 *
 */
void print_usage(Cli* cli, char* name)
{
    std::cout << "\n\n  Usage : " << name << " [OPTIONS] [FILE_NAME]\n\n";
    std::cout << "  -interactive                     Run in interactive mode\n\n";
    std::cout << "  -batch -file <FILE_NAME>         Run in batch mode (Commands from file)\n\n";
    std::cout << "  -help                            print this message\n\n\n";
    std::cout << "Available Commands:\n";
    std::cout << "-------------------\n";
    std::cout << cli->usage();
    exit(-1);
}

int main(int argc, char** argv)
{
    Cli *cli = Cli::Instance();

    // Update cli static information
    update_available_option(cli);

    // If interactive mode is given
    if (cli->cmd_exists(argv, (argv+argc), "-interactive")) {
        start_interactive(cli);
    }
    else if (cli->cmd_exists(argv, (argv+argc), "-batch")) {
        // Batch mode get file that contains all commands
        const char * file = cli->get_cmd_value(argv, (argv+argc), "-file");
        if(file) {
            start_batch_mode(cli, file);
        }
        else{
            print_usage(cli, argv[0]);
        }
    }
    else {
        print_usage(cli, argv[0]);
    }
    return 0;
}
