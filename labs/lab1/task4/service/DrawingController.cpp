#include "DrawingController.h"

int DrawingController::StartDrawing()
{
    std::string commandLine;
    while (getline(m_in, commandLine))
    {
        std::istringstream input(commandLine);
        std::string command;
        input >> command;

        if (command.empty())
        {
            continue;
        }

        auto it = m_commands.find(command);
        if (it != m_commands.end())
        {
            it->second(input);
        }
        else
        {
            return 1;
        }
    }

    return 0;
}