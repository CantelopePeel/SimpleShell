//
// Created by Liam Heeger on 8/30/18.
//

#include "shell_utils.h"

#include <regex>
#include <iostream>

using namespace shell;

bool
ShellUtils::
ParseCommand(const std::string& command_line, Command* command) {
    command->set_original_command(command_line);

    std::regex whitespace_regex("\\s+");
    auto token_iter = std::sregex_token_iterator(command_line.begin(), command_line.end(), whitespace_regex, -1);

    auto* sub_command = command->add_sub_command();
    if (token_iter != std::sregex_token_iterator()) {
        bool set_program = true;
        while (token_iter != std::sregex_token_iterator()) {
            // TODO handle errors.
            if (std::string(*token_iter) == "|") {
                sub_command = command->add_sub_command();
                set_program = true;
                token_iter++;
            } else if (set_program) {
                sub_command->set_program(std::string(*token_iter++));
                set_program = false;
            } else {
                sub_command->add_argument(std::string(*token_iter++));
            }
        }

        return true;
    } else {
        // TODO add error message.
        return false;
    }
}

bool
ShellUtils::
IsInteralCommand(const Command& command) {
    return command.sub_command(0).program() == "cd" ||
           command.sub_command(0).program() == "exit" ||
           command.sub_command(0).program() == "jobs";
}