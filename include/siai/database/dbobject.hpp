#pragma once

class DbConnector;

class DbObject
{
public:
	virtual void saveToDatabase(DbConnector& connector)  const = 0;
	virtual void loadFromDatabase(DbConnector& connector) = 0;

protected:
	DbObject() = default;
	~DbObject() = default;
};
