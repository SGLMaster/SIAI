#pragma once

#include "dbquery.hpp"

struct SQLQueryData
{
    std::string table;
    std::vector<std::string> cols;
    std::vector<std::string> values;
};

struct SQLMultipleQueryData
{
    std::string table;
    std::vector<std::string> cols;
    std::vector<std::vector<std::string>> values;
};

class SQLQuery : public DbQuery
{
protected:
    const std::string& m_table;
    const std::vector<std::string>& m_columns;
    const std::vector<std::string>& m_values;

    SQLQuery() = delete;
    SQLQuery(const SQLQueryData& data);
    virtual ~SQLQuery();
};

class SQLMultipleQuery : public DbQuery
{
protected:
    const std::string& m_table;
    const std::vector<std::string>& m_columns;
    const std::vector<std::vector<std::string>>& m_values;

    SQLMultipleQuery() = delete;
    SQLMultipleQuery(const SQLMultipleQueryData& data);
    virtual ~SQLMultipleQuery();
};

class SQLWhereCondition : public SQLQuery
{
public:
    SQLWhereCondition() = delete;
    SQLWhereCondition(const SQLQueryData& data);
    virtual ~SQLWhereCondition();

    virtual std::string generateString() const override;

private:
    void appendConditionsForColsAndValues(std::string& queryString) const;
};

class SQLInsertQuery : public SQLQuery
{
public:
    SQLInsertQuery() = delete;
    SQLInsertQuery(const SQLQueryData& data);
    virtual ~SQLInsertQuery();

    virtual std::string generateString() const override;

private:
    std::string getStringForColsOrValues(const std::string& startingString, const std::string& quoteChar,
                                         const std::vector<std::string>& valuesToInsertOnString) const;
};

class SQLMultipleInsertQuery : public SQLMultipleQuery
{
public:
	SQLMultipleInsertQuery() = delete;
	SQLMultipleInsertQuery(const SQLMultipleQueryData& data);
    virtual ~SQLMultipleInsertQuery();

    virtual std::string generateString() const override;

private:
    void appendStringForColumns(std::string& queryString) const;
    void appendStringForValues(std::string& queryString) const;
};

class SQLUpdateQuery : public SQLQuery
{
private:
    std::string m_whereCondition;

public:
    SQLUpdateQuery() = delete;
    SQLUpdateQuery(const SQLQueryData& data, const std::string& whereCondition="");
    virtual ~SQLUpdateQuery();

    virtual std::string generateString() const override;

private:
    void appendColsAndNewValues(std::string& queryString) const;
};

class SQLCreateTableQuery : public SQLQuery
{
private:
    std::string m_primaryKeyName;

public:
    SQLCreateTableQuery() = delete;
    SQLCreateTableQuery(const SQLQueryData& data, const std::string& primaryKeyName);
    virtual ~SQLCreateTableQuery();

    virtual std::string generateString() const override;

private:
    void appendStringForColsPlusPrimaryKey(std::string& queryString) const;
};
