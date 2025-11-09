#include "job_control.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <windows.h>

struct Job {
    int id;
    DWORD pid;
    HANDLE hProcess;
    std::string cmd;
    bool done;
};

static std::vector<Job> jobs;
static int next_id = 1;

void add_job_handle(HANDLE hProcess, DWORD pid, const std::string &cmd) {
    Job j;
    j.id = next_id++;
    j.pid = pid;
    j.hProcess = hProcess;
    j.cmd = cmd;
    j.done = false;
    jobs.push_back(j);
    std::cout << "[bg] PID: " << pid << "\n";
}

void list_jobs() {
    if (jobs.empty()) {
        std::cout << "No background jobs.\n";
        return;
    }
    for (const auto &j : jobs) {
        std::cout << "[" << j.id << "] PID: " << j.pid << " "
                  << (j.done ? "Done " : "Running ")
                  << j.cmd << "\n";
    }
}

void bring_job_to_foreground(int jobid) {
    for (auto it = jobs.begin(); it != jobs.end(); ++it) {
        if (it->id == jobid) {
            std::cout << "Bringing job [" << it->id << "] (" << it->cmd << ") to foreground...\n";
            if (it->hProcess == NULL) {
                std::cout << "fg: No process handle available for PID " << it->pid << "\n";
                return;
            }
            DWORD wait = WaitForSingleObject(it->hProcess, INFINITE);
            if (wait == WAIT_OBJECT_0) {
                it->done = true;
                std::cout << "Process already completed (marking as done)\n";
            } else {
                std::cout << "fg: Wait failed or returned " << wait << "\n";
            }
            CloseHandle(it->hProcess);
            it->hProcess = NULL;
            return;
        }
    }
    std::cout << "fg: Job not found.\n";
}

void resume_job_in_background(int jobid) {
    for (auto &j : jobs) {
        if (j.id == jobid) {
            if (j.done) {
                std::cout << "bg: Job already finished.\n";
            } else {
                std::cout << "Resuming job [" << j.id << "] in background.\n";
            }
            return;
        }
    }
    std::cout << "bg: Job ID not found.\n";
}
