#include <iostream>
#include <string>
#include "parser.h"
#include "executor.h"
#include "job_control.h"

using namespace std;

int main() {
    string line;
    cout << "Welcome to My Custom Shell!\n";

    while (true) {
        cout << "myshell> ";
        if (!getline(cin, line)) break;

        if (line == "exit") break;
        if (line == "jobs") { list_jobs(); continue; }

      
        if (line.rfind("fg", 0) == 0) {
            int jobid = stoi(line.substr(3));
            bring_job_to_foreground(jobid);
            continue;
        }

        if (line.rfind("bg", 0) == 0) {
            int jobid = stoi(line.substr(3));
            resume_job_in_background(jobid);
            continue;
        }

       
        Pipeline p = parse_pipeline(line);
        if (p.cmds.empty()) continue;

        execute_pipeline(p.cmds, p.infile, p.outfile, p.background);
    }

    cout << "Goodbye!\n";
    return 0;
}
