#include "parser.h"
#include <sstream>
#include <vector>
#include <string>

std::vector<std::string> tokenize(const std::string &line) {
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string tok;

    while (iss >> tok)
        tokens.push_back(tok);

    return tokens;
}

Pipeline parse_pipeline(const std::string &line) {
    Pipeline p;
    auto tokens = tokenize(line);

    // Check for background job '&'
    if (!tokens.empty() && tokens.back() == "&") {
        p.background = true;
        tokens.pop_back();
    }

    std::vector<std::string> current;
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i] == "|") {
            if (!current.empty()) {
                p.cmds.push_back(current);
                current.clear();
            }
        } else if (tokens[i] == "<" && i + 1 < tokens.size()) {
            p.infile = tokens[++i];
        } else if (tokens[i] == ">" && i + 1 < tokens.size()) {
            p.outfile = tokens[++i];
        } else {
            current.push_back(tokens[i]);
        }
    }
    if (!current.empty())
        p.cmds.push_back(current);

    return p;
}
