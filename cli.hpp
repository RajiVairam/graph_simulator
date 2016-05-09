#ifndef __CLI_HPP_
#define __CLI_HPP_

# include <iostream>
# include <sstream>
# include <iomanip>
# include <vector>
# include <map>
# include <string>
# include <string.h>
# include <algorithm>

/*
 * Base class for all command line options
 */
class Cmd {

    protected:
        std::string _mName;      // Command Name
        std::string _mDesc;      // Command Description
        std::string _mUsage;     // string used to represent usage

    public:
        Cmd(const std::string& name,
                const std::string& desc)
        {
            _mName = name;
            _mDesc = desc;
            _mUsage = std::string("");
        }
        virtual ~Cmd(){}

        // pure virtual function to get usage string
        virtual std::string& get_usage()=0;
        // Return name of the command
        std::string& get_name();
        // Formatting the usgae output
        std::string align();
};

/*
 * Class of flag kind of option. No value is required
 *    E.g. -help
 */
class Flag : public Cmd
{
    public:
        Flag(const std::string& name,
                const std::string& desc)
            :
                Cmd(name, desc)
    {

    }

        ~Flag(){};
        // Drived class implementation
        std::string& get_usage();
};

/*
 *  Class of option that may/may not require values
 *   E.g.  -add_node [NODE_ID]
 */
class Option : public Cmd
{
    private:
        std::string __mShortDesc;  // Short description for value (e.g. [NODE_ID])
        bool __mRequiredValue;     // Value required or not

    public:
        Option(const std::string& name,
                const std::string& desc,
                const std::string& short_desc,
                bool required = true)
            :
                Cmd(name, desc)
    {
        __mShortDesc = short_desc;
        __mRequiredValue = required;
    }

        ~Option(){};

        std::string& get_usage();
        bool is_required();
};

/*
 *  Class for title that will have sub option
 *   E.g.  add_edge -src_node [NODE_ID] -dest_node [NODE_ID]
 */
class Title : virtual public Cmd
{
    private:
        std::vector<Option*> __mSubOptions;  // To store suboptions object

    public:
        Title(const std::string& name,
                const std::string& desc)
            :
                Cmd(name, desc)
    {

    }

        ~Title(){};

        std::string& get_usage();
        size_t get_sub_option_count();

        // Method to add suboption
        void add_sub_option(const std::string& name, const std::string& desc,
                const std::string& short_desc, bool required);
};

typedef std::vector<std::string> StringList;
typedef std::map<std::string, std::string> StringMap;

/*
 *  Singleton class to have all commands
 */
class Cli
{
    private:
        std::vector<Cmd*> __mFlagList;                    // List of flags added
        std::vector<Cmd*> __mOptionList;                  // List of option list
        std::vector<Cmd*> __mTitleList;                   // List of title list
        std::string __mUsage;                             // Usage string
        static Cli* __mPrivateInstance;                   // private instance
        std::map<std::string, bool> __mCmdNames;          // map of all options vs its
        // required flag field
        StringMap  __mValues;                             // Map of names and its value
        // will be updated after parsing
        // Hiding constructors
    private:
        Cli() { }
        Cli(Cli const&){}
        Cli& operator=(Cli const&){}
        ~Cli(){}

    public:
        // To get instance
        static Cli* Instance();
        // To add flag
        void add_flag(const std::string& name, const std::string& desc);
        // To add options
        void add_option(const std::string& name, const std::string& desc,
                const std::string& short_desc, bool required);
        // To add title command
        Title* add_title(const std::string&, const std::string&);

        void add_title_sub_option(Title *, const std::string& name,
                const std::string& desc,
                const std::string& short_desc,
                bool required);

        // Overloaded version of get cmd value
        const char* get_cmd_value(char** arg_start, char** arg_end,
                const std::string & option);
        const char* get_cmd_value(StringList& sList,  const std::string& option);
        // To check whether given option is in user input
        bool cmd_exists(char** arg_start, char** arg_end,
                const std::string& option);
        // To get the usage string
        std::string& usage();
        // To validate sub options of title command
        bool validate_title_sub_options();
        // To get value of command
        const char* get_value(const std::string& option);
        // Reset parser results
        void reset_values();
        // parsing user inputs
        bool parse_cmds(std::string&);
};
// Update command static information
void update_available_option(Cli*);

#endif
