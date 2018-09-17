//
// Created by Liam Heeger on 8/30/18.
//

#include "shell_info_command.h"
#include "shell/proto/shell_info.pb.h"

#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <ios>

using namespace shell;

ShellInfoCommand::ShellInfoCommand(const Command& command) : InternalCommand(command) {}

void
ShellInfoCommand::
Run(ShellInfo* shell_info, std::string* output) {
    std::string shell_info_output;
    shell_info->SerializeToString(&shell_info_output);

    // Convert to hex:
    std::stringstream info_hex_stream;

    for (std::string::size_type i = 0; i < shell_info_output.length(); i++) {
        info_hex_stream << std::hex << std::setfill('0') << std::setw(2) << (int) shell_info_output[i];
    }

    std::cout << "SHELL|" << shell_info_output << "|SHELL" << std::endl;
}
