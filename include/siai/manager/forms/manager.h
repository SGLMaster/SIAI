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
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/slider.h>
#include <wx/toolbar.h>
#include <wx/scrolwin.h>
#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/choice.h>

///////////////////////////////////////////////////////////////////////////

namespace Forms
{
	///////////////////////////////////////////////////////////////////////////////
	/// Class ManagerFrame
	///////////////////////////////////////////////////////////////////////////////
	class ManagerFrame : public wxFrame
	{
		private:

		protected:
			wxToolBar* m_toolBar1;
			wxToolBarToolBase* m_toolSelect;
			wxToolBarToolBase* m_toolRegularCell;
			wxToolBarToolBase* m_toolBlockedCell;
			wxToolBarToolBase* m_toolParkingCell;
			wxToolBarToolBase* m_toolAddAgv;
			wxToolBarToolBase* m_toolTurnLeft;
			wxToolBarToolBase* m_toolTurnRight;
			wxToolBarToolBase* m_toolUndo;
			wxToolBarToolBase* m_toolRedo;
			wxToolBarToolBase* m_toolConnectDatabase;
			wxStaticText* m_staticTextZoom;
			wxSlider* m_sliderZoom;
			wxScrolledWindow* m_scrolledMapPanel;
			wxMenuBar* m_menubar;
			wxMenu* m_menuMap;
			wxMenu* m_menuDatabase;
			wxStatusBar* m_statusBar;

			// Virtual event handlers, overide them in your derived class
			virtual void OnToolSelect( wxCommandEvent& event ) = 0;
			virtual void OnToolRegularCell( wxCommandEvent& event ) = 0;
			virtual void OnToolBlockedCell( wxCommandEvent& event ) = 0;
			virtual void OnToolParkingCell( wxCommandEvent& event ) = 0;
			virtual void OnToolAddAgv( wxCommandEvent& event ) = 0;
			virtual void OnToolTurnLeft( wxCommandEvent& event ) = 0;
			virtual void OnToolTurnRight( wxCommandEvent& event ) = 0;
			virtual void OnToolUndo( wxCommandEvent& event ) = 0;
			virtual void OnToolRedo( wxCommandEvent& event ) = 0;
			virtual void OnToolConnectDatabase( wxCommandEvent& event ) = 0;
			virtual void OnSliderZoom( wxCommandEvent& event ) = 0;
			virtual void OnEnterMapPanel( wxMouseEvent& event ) = 0;
			virtual void OnLeaveMapPanel( wxMouseEvent& event ) = 0;
			virtual void OnLeftClickMapPanel( wxMouseEvent& event ) = 0;
			virtual void OnPaintMapPanel( wxPaintEvent& event ) = 0;
			virtual void OnSelectionNewMap( wxCommandEvent& event ) = 0;
			virtual void OnSelectionLoadMap( wxCommandEvent& event ) = 0;
			virtual void OnSelectionDbSettings( wxCommandEvent& event ) = 0;


		public:

			ManagerFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Administrador de Almacén SIAI"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

			~ManagerFrame();

	};

	///////////////////////////////////////////////////////////////////////////////
	/// Class NewMapDialog
	///////////////////////////////////////////////////////////////////////////////
	class NewMapDialog : public wxDialog
	{
		private:

		protected:
			wxStaticText* m_staticMapName;
			wxTextCtrl* m_textCtrlMapName;
			wxStaticText* m_staticTextNumberOfCols;
			wxTextCtrl* m_textCtrlNumberOfCols;
			wxStaticText* m_staticTextNumberOfRows;
			wxTextCtrl* m_textCtrlNumberOfRows;
			wxButton* m_buttonAccept;
			wxButton* m_buttonCancel;

			// Virtual event handlers, overide them in your derived class
			virtual void OnClose( wxCloseEvent& event ) = 0;
			virtual void OnButtonClickAccept( wxCommandEvent& event ) = 0;
			virtual void OnButtonClickCancel( wxCommandEvent& event ) = 0;


		public:

			NewMapDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Nuevo Mapa..."), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP );
			~NewMapDialog();

	};

	///////////////////////////////////////////////////////////////////////////////
	/// Class DbSettingsDialog
	///////////////////////////////////////////////////////////////////////////////
	class DbSettingsDialog : public wxDialog
	{
		private:

		protected:
			wxStaticText* m_staticHost;
			wxTextCtrl* m_textCtrlHost;
			wxStaticText* m_staticPort;
			wxTextCtrl* m_textCtrlPort;
			wxStaticText* m_staticUser;
			wxTextCtrl* m_textCtrlUser;
			wxStaticText* m_staticPassword;
			wxTextCtrl* m_textCtrlPassword;
			wxButton* m_buttonAccept;
			wxButton* m_button4;

			// Virtual event handlers, overide them in your derived class
			virtual void OnClose( wxCloseEvent& event ) = 0;
			virtual void OnButtonClickAccept( wxCommandEvent& event ) = 0;
			virtual void OnButtonClickCancel( wxCommandEvent& event ) = 0;


		public:

			DbSettingsDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Configurar Base de Datos"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP );
			~DbSettingsDialog();

	};

	///////////////////////////////////////////////////////////////////////////////
	/// Class LoadMapDialog
	///////////////////////////////////////////////////////////////////////////////
	class LoadMapDialog : public wxDialog
	{
		private:

		protected:
			wxStaticText* m_staticMapName;
			wxChoice* m_choiceMapName;
			wxButton* m_buttonAccept;
			wxButton* m_buttonCancel;

			// Virtual event handlers, overide them in your derived class
			virtual void OnClose( wxCloseEvent& event ) = 0;
			virtual void OnAccept( wxCommandEvent& event ) = 0;
			virtual void OnCancel( wxCommandEvent& event ) = 0;


		public:

			LoadMapDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Cargar Mapa..."), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP );
			~LoadMapDialog();

	};

} // namespace Forms

