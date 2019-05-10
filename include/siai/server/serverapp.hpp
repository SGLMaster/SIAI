#pragma once

using DbConnectorPtr = std::unique_ptr<DbConnector>;
using MapPtr = std::unique_ptr<SIAIMap>;

class wxCmdLineParser;

class ServerApp : public wxApp
{
private:
    virtual bool OnInit() override;
    virtual int OnRun() override;
    virtual int OnExit() override;

    virtual void OnInitCmdLine(wxCmdLineParser& pParser) override;
    virtual bool OnCmdLineParsed(wxCmdLineParser& pParser) override;

    void noValidOptionMessage() const;
};