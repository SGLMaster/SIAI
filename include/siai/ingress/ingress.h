///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/toolbar.h>
#include <wx/checklst.h>
#include <wx/menu.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

namespace Forms
{
	///////////////////////////////////////////////////////////////////////////////
	/// Class IngressFrame
	///////////////////////////////////////////////////////////////////////////////
	class IngressFrame : public wxFrame
	{
		private:

		protected:
			wxStaticText* m_staticCode;
			wxStaticText* m_staticName;
			wxStaticText* m_staticRackId;
			wxStaticText* m_staticWeight;
			wxTextCtrl* m_textCode;
			wxTextCtrl* m_textName;
			wxTextCtrl* m_textRack;
			wxTextCtrl* m_textWeight;
			wxButton* m_buttonAdd;
			wxToolBar* m_toolBar;
			wxToolBarToolBase* m_toolUpdateTasks;
			wxCheckListBox* m_checkListTasks;
			wxMenuBar* m_menubar1;
			wxMenu* m_menuConnection;

			// Virtual event handlers, overide them in your derived class
			virtual void OnToolUpdateTasks( wxCommandEvent& event ) = 0;
			virtual void OnSelectionConnect( wxCommandEvent& event ) = 0;
			virtual void OnSelectionDisconnect( wxCommandEvent& event ) = 0;


		public:

			IngressFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Asistente de Ingreso SIAI"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,272 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

			~IngressFrame();

	};

} // namespace Forms

