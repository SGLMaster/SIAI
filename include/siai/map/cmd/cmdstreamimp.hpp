#pragma once

#include "map/cmd/cmdstream.hpp"

#include <deque>

class MapCommand;

class DbConnector;

class CommandStreamImp : public CommandStream
{
private:
    std::deque<std::unique_ptr<MapCommand>> m_commands;

    std::deque<std::unique_ptr<MapCommand>>::iterator m_iterator;

public:
    CommandStreamImp();
    virtual ~CommandStreamImp();

    virtual void executeAndLog(Entities::Container& entities, const std::string& command) override;
    virtual void saveLastCommandChangeToDb(DbConnector& connector, const std::string& mapName) override;

    virtual void undo(Entities::Container& entities) override;
    virtual void redo(Entities::Container& entities) override;

private:
    void tryToExecuteAndLog(Entities::Container& entities, std::deque<std::string>& arguments);
    void doExecuteAndLog(Entities::Container& entities, std::deque<std::string>& arguments);
};
