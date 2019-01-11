#include "map/cmd/cmdstreamimp.hpp"
#include "map/cmd/command.hpp"

#include "map/exception.hpp"
#include "log.hpp"

#include "util/string.hpp"

CommandStreamImp::CommandStreamImp() = default;
CommandStreamImp::~CommandStreamImp() = default;

void CommandStreamImp::executeAndLog(Entities::Container& entities, DbConnector& connector, const std::string& command)
{
	std::deque<std::string> arguments;
	    Util::String::splitIntoContainer(command, arguments, ' ');

	tryToExecuteAndLog(entities, connector, arguments);
}

void CommandStreamImp::undo(Entities::Container& entities, DbConnector& connector)
{
    if(m_iterator != m_commands.begin() && m_commands.size() > 0)
    {
        --m_iterator;

        auto& lastCommand = *m_iterator;
        lastCommand->undo(entities, connector);
    }
}

void CommandStreamImp::redo(Entities::Container& entities, DbConnector& connector)
{
    if(m_iterator != m_commands.end() && m_commands.size() > 0)
    {
        auto& command = *m_iterator;
        command->execute(entities, connector);

        ++m_iterator;
    }
}

void CommandStreamImp::tryToExecuteAndLog(Entities::Container& entities, DbConnector& connector,
		std::deque<std::string>& arguments)
{
	try
	{
		doExecuteAndLog(entities, connector, arguments);
	}
	catch(const CommandException& e)
	{
		Log::warning(e.what());
	}
	catch (const EntityException& e)
	{
		Log::warning(e.what());
	}
}

void CommandStreamImp::doExecuteAndLog(Entities::Container& entities, DbConnector& connector,
		std::deque<std::string>& arguments)
{
	std::string commandName = arguments.front();
	arguments.pop_front();

	auto mapCommand = MapCommand::create(commandName, arguments);
	mapCommand->execute(entities, connector);

	if(m_commands.size() > 0 && m_iterator != m_commands.end())
	{
		int lastCommandIndex = std::distance(m_commands.begin(), m_iterator);
		int newCommandStreamSize = lastCommandIndex;

		m_commands.resize(newCommandStreamSize);
	}

	m_commands.push_back(std::move(mapCommand));

	m_iterator = m_commands.end();
}
