#include "ingress/app.hpp"
#include "ingress/frame.hpp"

IMPLEMENT_APP(IngressApp)

IngressApp::IngressApp() = default;
IngressApp::~IngressApp() = default;

bool IngressApp::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    m_frame = new IngressFrame( (wxFrame *)NULL );
    m_frame->Show();

    return true;
}