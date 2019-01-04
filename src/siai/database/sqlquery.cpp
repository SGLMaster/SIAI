#include "database/sqlquery.hpp"

namespace DbUtil
{
	inline void removeTrailingComma(std::string& stringWithTrailingComma)
	{
		stringWithTrailingComma.replace(stringWithTrailingComma.end()-1, stringWithTrailingComma.end(), "");
	}
}

SQLQuery::SQLQuery(const SQLQueryData& data):
            m_table(data.table), m_columns(data.cols), m_values(data.values) {}

SQLQuery::~SQLQuery() {}

SQLMultipleQuery::SQLMultipleQuery(const SQLMultipleQueryData& data) : m_table(data.table),
		m_columns(data.cols), m_values(data.values) {}

SQLMultipleQuery::~SQLMultipleQuery() = default;

SQLWhereCondition::SQLWhereCondition(const SQLQueryData& data):
                    SQLQuery(data) {}

SQLWhereCondition::~SQLWhereCondition(){}

std::string SQLWhereCondition::generateString() const
{
    if(m_columns.size() != m_values.size())
        return "";

    std::string query;

    query += "WHERE ";

    query += "(";

    appendConditionsForColsAndValues(query);

    query += ")";

    return query;
}

void SQLWhereCondition::appendConditionsForColsAndValues(std::string& queryString) const
{
    for(unsigned int i = 0; i < m_columns.size(); ++i)
    {
        queryString += "`" + m_columns[i] + "` = '" + m_values[i] + "' AND ";
    }

    //We remove the " AND " text after the last condition
    queryString.replace(queryString.end()-5, queryString.end(), "");
}

SQLInsertQuery::SQLInsertQuery(const SQLQueryData& data):
                SQLQuery(data) {}

SQLInsertQuery::~SQLInsertQuery() = default;

std::string SQLInsertQuery::generateString() const
{
    std::string query;

    query += "INSERT INTO `" + m_table + "` ";
    query += getStringForColsOrValues("", "`", m_columns);
    query += getStringForColsOrValues(" VALUES ", "'", m_values);

    return query;
}

std::string SQLInsertQuery::getStringForColsOrValues(const std::string& startingString, const std::string& quoteChar,
                                                     const std::vector<std::string>& valuesToInsertOnString) const
{
    std::string queryString(startingString);
    queryString += "(";
    for(auto value : valuesToInsertOnString)
    {
        queryString = queryString + quoteChar + value + quoteChar +",";
    }
    DbUtil::removeTrailingComma(queryString);
    queryString += ")";

    return queryString;
}

SQLMultipleInsertQuery::SQLMultipleInsertQuery(const SQLMultipleQueryData& data) : SQLMultipleQuery(data) {}

SQLMultipleInsertQuery::~SQLMultipleInsertQuery() = default;

std::string SQLMultipleInsertQuery::generateString() const
{
    std::string query;

    query += "INSERT INTO `" + m_table + "` ";
    appendStringForColumns(query);
    query += " VALUES ";
    appendStringForValues(query);

    return query;
}

void SQLMultipleInsertQuery::appendStringForColumns(std::string& queryString) const
{
	queryString += "(";
	for(const auto& value : m_columns)
	{
		queryString = queryString + "`" + value + "`" +",";
	}
	DbUtil::removeTrailingComma(queryString);
	queryString += ")";
}

void SQLMultipleInsertQuery::appendStringForValues(std::string& queryString) const
{
	for(const auto& column : m_values)
	{
		queryString += "(";
		for(const auto& value : column)
		{
			queryString = queryString + "'" + value + "'" +",";
		}
		DbUtil::removeTrailingComma(queryString);
		queryString += "),";
	}
	DbUtil::removeTrailingComma(queryString);
}

SQLUpdateQuery::SQLUpdateQuery(const SQLQueryData& data, const std::string& whereCondition):
                SQLQuery(data), m_whereCondition(whereCondition) {}

SQLUpdateQuery::~SQLUpdateQuery() = default;

std::string SQLUpdateQuery::generateString() const
{
    if(m_columns.size() != m_values.size())
        return "";

    std::string query;

    query = query + "UPDATE `" + m_table + "` SET ";
    appendColsAndNewValues(query);
    if(!m_whereCondition.empty())
        query += " " + m_whereCondition;

    return query;
}

void SQLUpdateQuery::appendColsAndNewValues(std::string& queryString) const
{
    for(unsigned int i = 0; i < m_columns.size(); ++i)
    {
        queryString += "`" + m_columns[i] + "` = '" + m_values[i] + "',";
    }
    DbUtil::removeTrailingComma(queryString);
}

SQLCreateTableQuery::SQLCreateTableQuery(const SQLQueryData& data, const std::string& primaryKeyName) : SQLQuery(data),
		m_primaryKeyName{primaryKeyName} {}

SQLCreateTableQuery::~SQLCreateTableQuery() = default;

std::string SQLCreateTableQuery::generateString() const
{
    std::string query;

    query += "CREATE TABLE `" + m_table + "` ";
    appendStringForColsPlusPrimaryKey(query);

    return query;
}

void SQLCreateTableQuery::appendStringForColsPlusPrimaryKey(std::string& queryString) const
{
    queryString += "(";

    for(unsigned int i = 0; i < m_columns.size(); ++i)
    {
        queryString += "`" + m_columns[i] + "` " + m_values[i] +",";
    }
    DbUtil::removeTrailingComma(queryString);

    queryString += " PRIMARY KEY (`" + m_primaryKeyName + "`) )";
}
