#pragma once

#include "ingress/ingress.h"

#include "database/database.hpp"

#include <memory>
#include <vector>

class IngressFrame : public Forms::IngressFrame
{
private:
    std::string m_originalFrameTitle;

    std::string m_mapName;

    DbConnectionOptions m_dbConnectionOptions;

    std::unique_ptr<DbConnector> m_dbConnector;

public:
    IngressFrame(wxWindow* parent);
    virtual ~IngressFrame();

private:
    virtual void OnSelectionConnect(wxCommandEvent& event) override;
    virtual void OnSelectionDisconnect(wxCommandEvent& event) override;

    virtual void OnToolUpdateTasks(wxCommandEvent& event) override;

    void tryToConnectDb();
    bool isDbConnected();

    void loadTasksFromDb();
    void tryQueryAndStore(const DbQuery& query, std::vector<DbRow>& vector);
    void fillCheckList(std::vector<DbRow>& tasks);

    void updateFrame();
    void updateFrameTitle();
};