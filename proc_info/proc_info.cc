#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    // Processor Type:
    std::ifstream cpu_ifs("/proc/cpuinfo");
    std::cout << "Processor Type(s): " << std::endl;
    int proc_counter = 0;
    for (std::string line; std::getline(cpu_ifs, line);) {
        if (line.find("model name\t: ") != std::string::npos) {
            // TODO fix substituting.
            std::string cpu_info = line.substr(std::string("model name\t: ").size(), line.size());
            std::cout << "CPU " << proc_counter++ << ": " << cpu_info << std::endl;
        }
    }

    // Kernel Version
    std::string version;
    std::ifstream version_ifs("/proc/version");
    std::getline(version_ifs, version);

    std::cout << "Kernel Version: " << version << std::endl;

    version_ifs.close();

    // Total Memory:
    std::string memory;
    std::ifstream memory_ifs("/proc/meminfo");
    for (std::string line; std::getline(memory_ifs, line);) {
        if (line.find("MemTotal: ") != std::string::npos) {
            // TODO fix substituting. 
            memory = line.substr(std::string("MemTotal: ").size() + 5, line.size());
            break;
        }
    }
    std::cout << "Total Memory:" << memory << std::endl;

    // Uptime
    std::string uptime;
    std::ifstream uptime_ifs("/proc/uptime");
    uptime_ifs >> uptime;

    std::cout << "Uptime: " << uptime << " seconds" << std::endl;

    uptime_ifs.close();

    return 0;
}
