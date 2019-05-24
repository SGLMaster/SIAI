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

    virtual void executeAndLog(Entities::Stock& entities, DbConnector& connector,
    		const std::string& command) override;
    virtual void undo(Entities::Stock& entities, DbConnector& connector) override;
    virtual void redo(Entities::Stock& entities, DbConnector& connector) override;

    virtual void clear() override;

private:
    void tryToExecuteAndLog(Entities::Stock& entities, DbConnector& connector, std::deque<std::string>& arguments);
    void doExecuteAndLog(Entities::Stock& entities, DbConnector& connector, std::deque<std::string>& arguments);
};
