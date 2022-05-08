#pragma once

#include <gtest/gtest.h>
#include <string>

#include "../src/messages-manager.hh"

TEST(json_parsing, simple_parse)
{
    std::string path = "/home/aminou/Epita/Project/refacto/Cpad/message-ressources/error-messages.json";
    auto json = MessagesManager::instance().read_json(path);
    MessagesManager::instance().from_json(json);

    for (const auto& elem: MessagesManager::instance().get_error_messages())
        std::cout << elem.second << "\n";
    for (const auto& elem: MessagesManager::instance().get_execution_messages())
        std::cout << elem.second << "\n";
    
}