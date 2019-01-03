#include "sqlquery.h"

SQLQuery::SQLQuery(const SQLQueryData& data):
            m_table(data.table), m_columns(data.cols), m_values(data.values) {}

SQLQuery::~SQLQuery() {}

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
    //We delete the comma after the last element cause we don't want it, it is a side effect of the loop
    queryString.replace(queryString.end()-1, queryString.end(), "");
    queryString += ")";

    return queryString;
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
    //We delete the comma after the last element cause we don't want it, it is a side effect of the loop
    queryString.replace(queryString.end()-1, queryString.end(), "");
}
