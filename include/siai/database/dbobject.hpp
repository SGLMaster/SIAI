#pragma once

class DbConnector;

#include <string>

class DbObject
{
public:
	virtual void saveToDatabase(DbConnector& connector, const std::string& mapName) const = 0;
	virtual void loadFromDatabase(DbConnector& connector) = 0;

protected:
	DbObject() = default;
	~DbObject() = default;
};
