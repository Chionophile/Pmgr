
// includes
#include <iostream>
#include <filesystem>
#include "util.h"
#include "config.h"
#include "commands.h"
#include "dbi/sqlite_interface.h"

using namespace std;

// main method
int main(int argc, char* argv[])
{
    // get environment variables
    string home_dir;
    string config_dir;
    string config_path;
    try {
        home_dir = getenv("HOME");
        config_dir = home_dir + "/.config/passmgr";
        config_path = config_dir + "/passmgr.config";
    }
    catch (const exception& e){
        cout << e.what() << endl;
        return 0;
    }

    // make the directory
    system(("mkdir -p " + config_dir).c_str());

    // make or read from the configurations file
    Config config(config_dir);
    config.configure();

    // init the database
    try
    {
        dbi::create_tables(config.peek_database_path());
    }
    catch(const exception& e)
    {
        cerr << e.what() << '\n';
    }
    
    
    // mainloop with buffer
    string buffer;
    string token;
    while(true) {

        // print the next prompt
        cout << "pmgr> ";

        // get the next line
        getline(cin,buffer);

        // read the first token
        token = breakoff(buffer);

        // if the token is exit or quit
        if(token == "quit" || token == "exit")
        {
            break;
        }

        // try catch for command errors
        try
        {
            // if the token is "addorg"
            if(token == "addorg")
            {
                // get the name of the organization
                string org_name = breakoff(buffer);

                // add the organization
                addorg(config.peek_database_path(), org_name);

                continue;
            }
        }
        catch(exception& e)
        {
            cout << e.what() << endl;
            continue;
        }
        
    }


    // exit code zero
    cout << "Done!\n";
    return 0;
}