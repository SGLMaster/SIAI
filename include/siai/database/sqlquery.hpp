#pragma once

#include "dbquery.hpp"

struct SqlQueryData
{
    std::string table;
    std::vector<std::string> cols;
    std::vector<std::string> values;
};

struct SqlMultipleQueryData
{
    std::string table;
    std::vector<std::string> cols;
    std::vector<std::vector<std::string>> values;
};

class SqlQuery : public DbQuery
{
protected:
    const std::string& m_table;
    const std::vector<std::string>& m_columns;
    const std::vector<std::string>& m_values;

    SqlQuery() = delete;
    SqlQuery(const SqlQueryData& data);
    virtual ~SqlQuery();
};

class SqlMultipleQuery : public DbQuery
{
protected:
    const std::string& m_table;
    const std::vector<std::string>& m_columns;
    const std::vector<std::vector<std::string>>& m_values;

    SqlMultipleQuery() = delete;
    SqlMultipleQuery(const SqlMultipleQueryData& data);
    virtual ~SqlMultipleQuery();
};

class SqlWhereCondition : public SqlQuery
{
public:
    SqlWhereCondition() = delete;
    SqlWhereCondition(const SqlQueryData& data);
    virtual ~SqlWhereCondition();

    virtual std::string generateString() const override;

private:
    void appendConditionsForColsAndValues(std::string& queryString) const;
};

class SqlInsertQuery : public SqlQuery
{
public:
    SqlInsertQuery() = delete;
    SqlInsertQuery(const SqlQueryData& data);
    virtual ~SqlInsertQuery();

    virtual std::string generateString() const override;

private:
    std::string getStringForColsOrValues(const std::string& startingString, const std::string& quoteChar,
                                         const std::vector<std::string>& valuesToInsertOnString) const;
};

class SqlMultipleInsertQuery : public SqlMultipleQuery
{
public:
	SqlMultipleInsertQuery() = delete;
	SqlMultipleInsertQuery(const SqlMultipleQueryData& data);
    virtual ~SqlMultipleInsertQuery();

    virtual std::string generateString() const override;

private:
    void appendStringForColumns(std::string& queryString) const;
    void appendStringForValues(std::string& queryString) const;
};

class SqlUpdateQuery : public SqlQuery
{
private:
    std::string m_whereCondition;

public:
    SqlUpdateQuery() = delete;
    SqlUpdateQuery(const SqlQueryData& data, const std::string& whereCondition="");
    virtual ~SqlUpdateQuery();

    virtual std::string generateString() const override;

private:
    void appendColsAndNewValues(std::string& queryString) const;
};

class SqlDeleteRowQuery : public SqlQuery
{
private:
	std::string m_tableName;
    std::string m_whereCondition;

public:
    SqlDeleteRowQuery() = delete;
    SqlDeleteRowQuery(const std::string& tableName, const std::string& whereCondition);
    virtual ~SqlDeleteRowQuery();

    virtual std::string generateString() const override;
};

class SqlCreateTableQuery : public SqlQuery
{
private:
    std::string m_primaryKeyName;

public:
    SqlCreateTableQuery() = delete;
    SqlCreateTableQuery(const SqlQueryData& data, const std::string& primaryKeyName);
    virtual ~SqlCreateTableQuery();

    virtual std::string generateString() const override;

private:
    void appendStringForColsPlusPrimaryKey(std::string& queryString) const;
};
