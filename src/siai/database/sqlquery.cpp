#include "database/sqlquery.hpp"

namespace DbUtil
{
	inline void removeTrailingComma(std::string& stringWithTrailingComma)
	{
		stringWithTrailingComma.replace(stringWithTrailingComma.end()-1, stringWithTrailingComma.end(), "");
	}
}

SqlQuery::SqlQuery(const SqlQueryData& data):
            m_table(data.table), m_columns(data.cols), m_values(data.values) {}

SqlQuery::~SqlQuery() {}

SqlMultipleQuery::SqlMultipleQuery(const SqlMultipleQueryData& data) : m_table(data.table),
		m_columns(data.cols), m_values(data.values) {}

SqlMultipleQuery::~SqlMultipleQuery() = default;

SqlShowTablesQuery::SqlShowTablesQuery() : SqlQuery(SqlQueryData()) {}

SqlShowTablesQuery::~SqlShowTablesQuery() = default;

std::string SqlShowTablesQuery::generateString() const
{
    return "SHOW TABLES";
}

SqlSelectQuery::SqlSelectQuery(const SqlQueryData& data, const std::string& whereCondition):
                SqlQuery(data), m_whereCondition(whereCondition) {}

SqlSelectQuery::~SqlSelectQuery() = default;

std::string SqlSelectQuery::generateString() const
{
    std::string query;

    query += "SELECT ";

    appendCols(query);

    query += " FROM `" + m_table + "`";

    if(!m_whereCondition.empty())
        query += " " + m_whereCondition;

    return query;
}

void SqlSelectQuery::appendCols(std::string& queryString) const
{
    for(unsigned int i = 0; i < m_columns.size(); ++i)
    {
        queryString += "`" + m_columns[i] + "`,";
    }
    //We delete the comma after the last element cause we don't want it, it is a side effect of the loop
    queryString.replace(queryString.end()-1, queryString.end(), "");
}

SqlWhereCondition::SqlWhereCondition(const SqlQueryData& data):
                    SqlQuery(data) {}

SqlWhereCondition::~SqlWhereCondition(){}

std::string SqlWhereCondition::generateString() const
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

void SqlWhereCondition::appendConditionsForColsAndValues(std::string& queryString) const
{
    for(unsigned int i = 0; i < m_columns.size(); ++i)
    {
        queryString += "`" + m_columns[i] + "` = '" + m_values[i] + "' AND ";
    }

    //We remove the " AND " text after the last condition
    queryString.replace(queryString.end()-5, queryString.end(), "");
}

SqlInsertQuery::SqlInsertQuery(const SqlQueryData& data):
                SqlQuery(data) {}

SqlInsertQuery::~SqlInsertQuery() = default;

std::string SqlInsertQuery::generateString() const
{
    std::string query;

    query += "INSERT INTO `" + m_table + "` ";
    query += getStringForColsOrValues("", "`", m_columns);
    query += getStringForColsOrValues(" VALUES ", "'", m_values);

    return query;
}

std::string SqlInsertQuery::getStringForColsOrValues(const std::string& startingString, const std::string& quoteChar,
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

SqlMultipleInsertQuery::SqlMultipleInsertQuery(const SqlMultipleQueryData& data) : SqlMultipleQuery(data) {}

SqlMultipleInsertQuery::~SqlMultipleInsertQuery() = default;

std::string SqlMultipleInsertQuery::generateString() const
{
    std::string query;

    query += "INSERT INTO `" + m_table + "` ";
    appendStringForColumns(query);
    query += " VALUES ";
    appendStringForValues(query);

    return query;
}

void SqlMultipleInsertQuery::appendStringForColumns(std::string& queryString) const
{
	queryString += "(";
	for(const auto& value : m_columns)
	{
		queryString = queryString + "`" + value + "`" +",";
	}
	DbUtil::removeTrailingComma(queryString);
	queryString += ")";
}

void SqlMultipleInsertQuery::appendStringForValues(std::string& queryString) const
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

SqlUpdateQuery::SqlUpdateQuery(const SqlQueryData& data, const std::string& whereCondition):
                SqlQuery(data), m_whereCondition(whereCondition) {}

SqlUpdateQuery::~SqlUpdateQuery() = default;

std::string SqlUpdateQuery::generateString() const
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

void SqlUpdateQuery::appendColsAndNewValues(std::string& queryString) const
{
    for(unsigned int i = 0; i < m_columns.size(); ++i)
    {
        queryString += "`" + m_columns[i] + "` = '" + m_values[i] + "',";
    }
    DbUtil::removeTrailingComma(queryString);
}

SqlDeleteRowQuery::SqlDeleteRowQuery(const std::string& tableName, const std::string& whereCondition) :
		SqlQuery(SqlQueryData{"", {""}, {""}}), m_tableName{tableName}, m_whereCondition{whereCondition} {}

SqlDeleteRowQuery::~SqlDeleteRowQuery() = default;

std::string SqlDeleteRowQuery::generateString() const
{
	std::string query;

	query += "DELETE FROM `" + m_tableName + "` " + m_whereCondition;

	return query;
}

SqlCreateTableQuery::SqlCreateTableQuery(const SqlQueryData& data, const std::string& primaryKeyName) : SqlQuery(data),
		m_primaryKeyName{primaryKeyName} {}

SqlCreateTableQuery::~SqlCreateTableQuery() = default;

std::string SqlCreateTableQuery::generateString() const
{
    std::string query;

    query += "CREATE TABLE `" + m_table + "` ";
    appendStringForColsPlusPrimaryKey(query);

    return query;
}

void SqlCreateTableQuery::appendStringForColsPlusPrimaryKey(std::string& queryString) const
{
    queryString += "(";

    for(unsigned int i = 0; i < m_columns.size(); ++i)
    {
        queryString += "`" + m_columns[i] + "` " + m_values[i] +",";
    }

    queryString += " PRIMARY KEY (`" + m_primaryKeyName + "`) )";
}
