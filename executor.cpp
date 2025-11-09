#include "executor.h"
#include "job_control.h"
#include <iostream>
#include <vector>
#include <string>
#include <process.h>  
#include <cstdlib>     
#include <direct.h> 
#include <windows.h>
#include <sstream>


static std::string join_tokens(const std::vector<std::string> &tokens) {
    std::ostringstream oss;
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (i) oss << " ";
        oss << tokens[i];
    }
    return oss.str();
}


static bool launch_cmd_process(const std::string &cmdline, DWORD creationFlags,
                               PROCESS_INFORMATION &outPi) {
    STARTUPINFOA si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&outPi, sizeof(outPi));

    
    std::string fullCmd = "cmd.exe /C " + cmdline;

    BOOL ok = CreateProcessA(
        "C:\\Windows\\System32\\cmd.exe",       // application name
        (LPSTR)fullCmd.c_str(),                 // command line
        NULL, NULL,
        FALSE,
        creationFlags,
        NULL, NULL,
        &si,
        &outPi
    );

    if (!ok) {
        std::cerr << "myshell: CreateProcessA failed (GetLastError=" << GetLastError() << ")\n";
        return false;
    }
    return true;
}


int execute_command(const std::vector<std::string> &tokens, bool background) {
    if (tokens.empty()) return 0;

    // builtin: cd
    if (tokens[0] == "cd") {
        if (tokens.size() > 1) {
            if (_chdir(tokens[1].c_str()) != 0) {
                perror("cd");
                return -1;
            }
        } else {
            const char *home = getenv("USERPROFILE");
            if (home) _chdir(home);
        }
        return 0;
    }

    // Build command line (let cmd.exe handle builtins and path)
    std::string cmdline = join_tokens(tokens);

    PROCESS_INFORMATION pi;
    DWORD flags = background ? CREATE_NEW_CONSOLE : 0;

    if (!launch_cmd_process(cmdline, flags, pi)) {
        return -1;
    }

    if (background) {
        // Duplicate the process handle so job_control owns a stable handle
        HANDLE dup = NULL;
        if (!DuplicateHandle(GetCurrentProcess(), pi.hProcess,
                             GetCurrentProcess(), &dup, 0, FALSE,
                             DUPLICATE_SAME_ACCESS)) {
         
            std::cerr << "myshell: DuplicateHandle failed (" << GetLastError() << ")\n";
            dup = pi.hProcess; 
        }

        // Register job with job_control
        add_job_handle(dup, pi.dwProcessId, cmdline);

        
        CloseHandle(pi.hThread);
        if (dup != pi.hProcess) CloseHandle(pi.hProcess);
        return 0;
    } else {
        // Foreground: wait until process finishes
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        return 0;
    }
}

// Execute pipeline / multiple commands with redirection handled by cmd.exe
int execute_pipeline(const std::vector<std::vector<std::string>> &cmds,
                     const std::string &infile,
                     const std::string &outfile,
                     bool background) {
    if (cmds.empty()) return 0;

    // Build command line connecting commands with pipes
    std::ostringstream oss;
    for (size_t i = 0; i < cmds.size(); ++i) {
        if (i) oss << " | ";
        oss << join_tokens(cmds[i]);
    }
    if (!infile.empty()) oss << " < " << infile;
    if (!outfile.empty()) oss << " > " << outfile;
    std::string cmdline = oss.str();

    PROCESS_INFORMATION pi;
    DWORD flags = background ? CREATE_NEW_CONSOLE : 0;

    if (!launch_cmd_process(cmdline, flags, pi)) {
        return -1;
    }

    if (background) {
        // Duplicate handle and register job
        HANDLE dup = NULL;
        if (!DuplicateHandle(GetCurrentProcess(), pi.hProcess,
                             GetCurrentProcess(), &dup, 0, FALSE,
                             DUPLICATE_SAME_ACCESS)) {
            std::cerr << "myshell: DuplicateHandle failed (" << GetLastError() << ")\n";
            dup = pi.hProcess;
        }
        add_job_handle(dup, pi.dwProcessId, cmdline);
        CloseHandle(pi.hThread);
        if (dup != pi.hProcess) CloseHandle(pi.hProcess);
        return 0;
    } else {
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        return 0;
    }
}
