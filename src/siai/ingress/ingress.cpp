///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include <wx/wxprec.h>

#include "ingress/ingress.h"

///////////////////////////////////////////////////////////////////////////
using namespace Forms;

IngressFrame::IngressFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 500,600 ), wxSize( 500,600 ) );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );

	m_staticCode = new wxStaticText( this, wxID_ANY, _("Codigo"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticCode->Wrap( -1 );
	m_staticCode->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );

	bSizer2->Add( m_staticCode, 2, wxALL, 5 );

	m_staticName = new wxStaticText( this, wxID_ANY, _("Nombre"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticName->Wrap( -1 );
	m_staticName->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );

	bSizer2->Add( m_staticName, 2, wxALL, 5 );

	m_staticRackId = new wxStaticText( this, wxID_ANY, _("Estante"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticRackId->Wrap( -1 );
	m_staticRackId->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );

	bSizer2->Add( m_staticRackId, 2, wxALL, 5 );

	m_staticWeight = new wxStaticText( this, wxID_ANY, _("Peso"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticWeight->Wrap( -1 );
	m_staticWeight->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );

	bSizer2->Add( m_staticWeight, 2, wxALL, 5 );


	bSizer1->Add( bSizer2, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );

	m_textCode = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_textCode, 0, wxALL, 5 );

	m_textName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_textName, 0, wxALL, 5 );

	m_textRack = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_textRack, 0, wxALL, 5 );

	m_textWeight = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_textWeight, 0, wxALL, 5 );


	bSizer1->Add( bSizer4, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );


	bSizer5->Add( 0, 0, 1, wxEXPAND, 5 );

	m_buttonAddTask = new wxButton( this, wxID_ANY, _("Agregar Tarea"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_buttonAddTask, 0, wxALL, 5 );


	bSizer1->Add( bSizer5, 1, wxEXPAND, 5 );

	m_toolBar = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL );
	m_toolUpdateTasks = m_toolBar->AddTool( wxID_ANY, _("Actualizar Tareas..."), wxBitmap( wxT("resources/tools/sync.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, _("Actualizar Tareas..."), _("Actualizar Tareas..."), NULL );

	m_toolBar->Realize();

	bSizer1->Add( m_toolBar, 0, wxEXPAND, 5 );

	wxArrayString m_checkListTasksChoices;
	m_checkListTasks = new wxCheckListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_checkListTasksChoices, 0 );
	m_checkListTasks->SetMinSize( wxSize( -1,400 ) );
	m_checkListTasks->SetMaxSize( wxSize( -1,400 ) );

	bSizer1->Add( m_checkListTasks, 10, wxALL|wxEXPAND, 5 );

	m_buttonDone = new wxButton( this, wxID_ANY, _("Listo"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_buttonDone, 0, wxALL, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();
	m_menubar1 = new wxMenuBar( 0 );
	m_menuConnection = new wxMenu();
	wxMenuItem* m_menuItemConnect;
	m_menuItemConnect = new wxMenuItem( m_menuConnection, wxID_ANY, wxString( _("Conectar") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuConnection->Append( m_menuItemConnect );

	wxMenuItem* m_menuItemDisconnect;
	m_menuItemDisconnect = new wxMenuItem( m_menuConnection, wxID_ANY, wxString( _("Desconectar") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuConnection->Append( m_menuItemDisconnect );
	m_menuItemDisconnect->Enable( false );

	m_menubar1->Append( m_menuConnection, _("Conexion") );

	this->SetMenuBar( m_menubar1 );


	this->Centre( wxBOTH );

	// Connect Events
	m_buttonAddTask->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IngressFrame::OnClickAddTask ), NULL, this );
	this->Connect( m_toolUpdateTasks->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( IngressFrame::OnToolUpdateTasks ) );
	m_buttonDone->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IngressFrame::OnClickDone ), NULL, this );
	m_menuConnection->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IngressFrame::OnSelectionConnect ), this, m_menuItemConnect->GetId());
	m_menuConnection->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( IngressFrame::OnSelectionDisconnect ), this, m_menuItemDisconnect->GetId());
}

IngressFrame::~IngressFrame()
{
	// Disconnect Events
	m_buttonAddTask->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IngressFrame::OnClickAddTask ), NULL, this );
	this->Disconnect( m_toolUpdateTasks->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( IngressFrame::OnToolUpdateTasks ) );
	m_buttonDone->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IngressFrame::OnClickDone ), NULL, this );

}
