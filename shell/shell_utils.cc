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
    command->set_background(false);

    std::string clean_cmd_line = std::regex_replace(command_line, std::regex("^ +"), "");

    std::regex whitespace_regex("\\s+");
    auto token_iter = std::sregex_token_iterator(clean_cmd_line.begin(), clean_cmd_line.end(), whitespace_regex, -1);

    auto* sub_command = command->add_sub_command();
    if (token_iter != std::sregex_token_iterator()) {
        bool set_program = true;
        while (token_iter != std::sregex_token_iterator()) {
            if (std::string(*token_iter) == "&") {
                if (set_program) {
                    std::cerr << "Error: Empty command followed by '&' is not allowed." << std::endl;
                    return false;
                }

                if (++token_iter == std::sregex_token_iterator()) {
                    command->set_background(true);
                } else {
                    std::cerr << "Error: Background command specifier '&' not found at end of command." << std::endl;
                    return false;
                }
            } else if (std::string(*token_iter) == "|") {
                if (set_program) {
                    std::cerr << "Error: Invalid pipe syntax." << std::endl;
                    return false;
                }
                sub_command = command->add_sub_command();
                set_program = true;
                token_iter++;
            } else if (set_program) {
                std::string program = std::string(*token_iter++);

                Command test_command;
                test_command.add_sub_command()->set_program(program);
                if (command->sub_command_size() > 1 && IsInteralCommand(test_command)) {
                    std::cerr << "Error: Internal commands are not allowed in piped commands." << std::endl;
                    return false;
                }
                sub_command->set_program(program);
                set_program = false;
            } else {
                sub_command->add_argument(std::string(*token_iter++));
            }
        }

        if (command->sub_command_size() > 1) {
            for (auto command_curs : command->sub_command()) {
                Command test_command;
                test_command.add_sub_command()->set_program(command_curs.program());
                if (IsInteralCommand(test_command)) {
                    std::cerr << "Error: Internal commands are not allowed in piped commands." << std::endl;
                    return false;
                }
            }
        }

        return true;
    } else {
        // Entered line is empty or whitespace.
        return false;
    }
}

bool
ShellUtils::
IsInteralCommand(const Command& command) {
    return command.sub_command(0).program() == "cd" ||
           command.sub_command(0).program() == "exit" ||
           command.sub_command(0).program() == "jobs" ||
           command.sub_command(0).program() == "fg" ||
           command.sub_command(0).program() == "bg" ||
           command.sub_command(0).program() == "shell_info";
}