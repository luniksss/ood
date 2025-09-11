#include "DrawingController.h"

int DrawingController::StartDrawing()
{
    std::string commandLine;
    getline(m_in, commandLine);
    std::istringstream input(commandLine);

    std::string command;
    input >> command;

    auto it = m_commands.find(command);
    if (it != m_commands.end())
    {
        it->second(input);
        return 0;
    }

    return 1;
}