#pragma once

#include "map/cmd/cmdstream.hpp"

#include <deque>

class MapCommand;

class CommandStreamImp : public CommandStream
{
private:
    std::deque<std::unique_ptr<MapCommand>> m_commands;

    std::deque<std::unique_ptr<MapCommand>>::iterator m_iterator;

public:
    CommandStreamImp();
    virtual ~CommandStreamImp();

    virtual void executeAndLog(Entities::Container& entities, DbConnector& connector,
    		const std::string& command) override;
    virtual void undo(Entities::Container& entities, DbConnector& connector) override;
    virtual void redo(Entities::Container& entities) override;

private:
    void tryToExecuteAndLog(Entities::Container& entities, DbConnector& connector, std::deque<std::string>& arguments);
    void doExecuteAndLog(Entities::Container& entities, DbConnector& connector, std::deque<std::string>& arguments);
};
