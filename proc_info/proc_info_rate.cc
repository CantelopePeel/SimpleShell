#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>

int nth_field(std::string in, int n) {
    std::istringstream iss(in);
    std::string curs = "";
    
    for (int i = 0; i < n; i++) {
        iss >> curs;
    }
    
    return std::stoi(curs);
}

void print_proc_info() {
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
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        print_proc_info();
        return 0;
    }

    double read_rate = std::stod(argv[1]);
    double printout_rate = std::stod(argv[2]);

    int counter = 0;
    double avg_user = 0, avg_system = 0, avg_idle = 0, avg_total = 0, avg_mem_free = 0, avg_mem_total = 0, avg_ctx_switches = 0, avg_proc_creations = 0;
    int ctxs = 0, ctxs_prior = 0;
    int sectors_read_prior = 0, sectors_write_prior = 0;
    int sectors_read = 0, sectors_write = 0;
    int procs = 0, procs_prior = 0;
    for (;;) {
        if (counter % (int) read_rate == 0) {
            std::ifstream cpu_ifs("/proc/cpuinfo");
            int proc_counter = 0;
            for (std::string line; std::getline(cpu_ifs, line);) {
                if (line.find("processor\t: ") != std::string::npos) {
                    proc_counter++; 
                }
            }
            
            std::string stat;
            std::ifstream stat_ifs("/proc/stat");

            double user, nice, system, idle, total;
            stat_ifs >> stat >> user >> nice >> system >> idle;
            total = user + nice + system + idle;

            avg_user += user / printout_rate;
            avg_system += system / printout_rate;
            avg_idle += idle / printout_rate;
            avg_total += total / printout_rate;

            stat_ifs.close();
            
            double memory_free, memory_total;
            std::ifstream memory_ifs("/proc/meminfo");
            for (std::string line; std::getline(memory_ifs, line);) {
                if (line.find("MemFree: ") != std::string::npos) {
                    memory_free = std::stoi(line.substr(std::string("MemFree: ").size(), line.size() - 12));
                }
                if (line.find("MemTotal: ") != std::string::npos) {
                    memory_total = std::stoi(line.substr(std::string("MemTotal: ").size(), line.size() - 12));
                }
            }
                    
            avg_mem_free += memory_free / printout_rate;
            avg_mem_total += memory_total / printout_rate;
        }

        if (counter % (int) printout_rate == 0) {
            sectors_read = 0;
            sectors_write = 0;
            std::ifstream disk_ifs("/proc/diskstats");
            for (std::string line; std::getline(disk_ifs, line);) {
                sectors_read += nth_field(line, 6);    
                sectors_write += nth_field(line, 10);
            }
            
            std::ifstream ctxt_ifs("/proc/stat");
            for (std::string line; std::getline(ctxt_ifs, line);) {
                if (line.find("ctxt") != std::string::npos) {
                    ctxs = nth_field(line, 2);
                }
            }
            
            std::ifstream procs_ifs("/proc/stat");
            for (std::string line; std::getline(procs_ifs, line);) {
                if (line.find("processes") != std::string::npos) {
                    procs = nth_field(line, 2);
                }
            }
            
            if (counter != 0) {
                std::printf("%%CPU(s): %.3f, %.3f, %.3f\n", 100.0 * avg_user / avg_total, 100.0 * avg_system / avg_total, 100.0 * avg_idle / avg_total);
                std::printf("Mem: %.0f KB, %.3f%% \n", avg_mem_free, 100.0 * avg_mem_free / avg_mem_total);
                std::printf("Sectors/s: %.3f sectors/s, %.3f sectors/s\n", (double) (sectors_read - sectors_read_prior) / printout_rate, (double) (sectors_write - sectors_write_prior) / printout_rate);   

                std::printf("Context switches/s: %.3f \n", (double) (ctxs - ctxs_prior) / printout_rate);   
                std::printf("Process creations/s: %.3f \n", (double) (procs - procs_prior) / printout_rate);
            }

            ctxs_prior = ctxs;
            procs_prior = procs;
            sectors_read_prior = sectors_read;
            sectors_write_prior = sectors_write;

            avg_user = 0;
            avg_system = 0;
            avg_idle = 0; 
            avg_total = 0; 
            avg_mem_free = 0;
            avg_mem_total = 0;
            avg_ctx_switches = 0;
            avg_proc_creations = 0;
        } 
        counter++;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}
