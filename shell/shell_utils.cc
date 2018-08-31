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
    std::regex whitespace_regex("\\s+");
    auto token_iter = std::sregex_token_iterator(command_line.begin(), command_line.end(), whitespace_regex, -1);

    if (token_iter != std::sregex_token_iterator()) {
        std::cout << *token_iter << std::endl;
        command->set_program(std::string(*token_iter));
        while (++token_iter != std::sregex_token_iterator()) {
            // TODO handle ampersand
            std::cout << *token_iter << std::endl;
            //command->add_argument(*token_iter);
        }

        //command->argument(command->argument_size() - 1);
        return true;
    } else {
        // TODO add error message.
        return false;
    }
}

bool
ShellUtils::
IsInteralCommand(const Command& command) {
    return command.program() == "cd" ||
           command.program() == "exit" ||
           command.program() == "jobs";
}