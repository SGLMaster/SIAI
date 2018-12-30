#include "map/cmd/cmdstreamimp.hpp"
#include "map/cmd/command.hpp"

#include "map/exception.hpp"
#include "log.hpp"

#include "util/string.hpp"

CommandStreamImp::CommandStreamImp() = default;
CommandStreamImp::~CommandStreamImp() = default;

void CommandStreamImp::executeAndLog(Entities::Container& entities, const std::string& command)
{
    std::deque<std::string> arguments;
    Util::String::splitIntoContainer(command, arguments, ' ');

    std::string commandName = arguments.front();
    arguments.pop_front();

    try
    {
        auto mapCommand = MapCommand::create(commandName, arguments);
        mapCommand->execute(entities);

        if(m_commands.size() > 0 && m_iterator != m_commands.end())
        {
            int lastCommandIndex = std::distance(m_commands.begin(), m_iterator);
            int newCommandStreamSize = lastCommandIndex;

            m_commands.resize(newCommandStreamSize);
        }

        m_commands.push_back(std::move(mapCommand));

        m_iterator = m_commands.end();
    }
    catch(CommandException& e)
    {
        Log::warning(e.what());
    }
}

void CommandStreamImp::undo(Entities::Container& entities)
{
    if(m_iterator != m_commands.begin() && m_commands.size() > 0)
    {
        --m_iterator;

        auto& lastCommand = *m_iterator;
        lastCommand->undo(entities);
    }
}

void CommandStreamImp::redo(Entities::Container& entities)
{
    if(m_iterator != m_commands.end() && m_commands.size() > 0)
    {
        auto& command = *m_iterator;
        command->execute(entities);

        ++m_iterator;
    }
}
