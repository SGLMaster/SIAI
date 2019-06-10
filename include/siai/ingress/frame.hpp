#pragma once

#include "ingress/ingress.h"

#include "database/database.hpp"

#include <memory>
#include <vector>

struct IngressTask
{
    int id;
    std::string code;
    std::string name;
    int rackId;
    int weight;
};

class IngressFrame : public Forms::IngressFrame
{
private:
    std::string m_originalFrameTitle;

    std::string m_mapName;

    std::vector<IngressTask> m_tasksList;

    DbConnectionOptions m_dbConnectionOptions;

    std::unique_ptr<DbConnector> m_dbConnector;

public:
    IngressFrame(wxWindow* parent);
    virtual ~IngressFrame();

private:
    virtual void OnSelectionConnect(wxCommandEvent& event) override;
    virtual void OnSelectionDisconnect(wxCommandEvent& event) override;

    virtual void OnClickAddTask(wxCommandEvent& event) override;

    virtual void OnToolUpdateTasks(wxCommandEvent& event) override;

    void tryToConnectDb();
    bool isDbConnected();

    void loadTasksFromDb();
    void fillCheckList(std::vector<DbRow>& tasks);

    void addTask();

    void tryQueryAndStore(const DbQuery& query, std::vector<DbRow>& vector);
    void tryQueryWithoutResults(const DbQuery& query);

    void updateFrame();
    void updateFrameTitle();
};