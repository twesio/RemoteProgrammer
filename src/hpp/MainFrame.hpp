#ifndef MAINFRAME_HPP_
#define MAINFRAME_HPP_
#include <wx/wx.h>
#include <wx/socket.h>
#include "Server.hpp"
#include "Programmer.hpp"
#include "Registry.hpp"

class Server;

class MainFrame: public wxFrame {
public:
	MainFrame(const wxString& title);
	~MainFrame();
	void addToLog(const wxString& msg);

	void OnChangeIcdCmdPath(wxCommandEvent& event);
	void OnChangeIcdCmdOptions(wxCommandEvent& event);
	void OnStart(wxCommandEvent& event);
	void OnStop(wxCommandEvent& event);
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnServerEvent(wxSocketEvent& event);
	void OnSocketEvent(wxSocketEvent& event);

	enum {
		MENU_SELECT_PATH, //deprecated
		MENU_START,
		MENU_STOP,
		MENU_CHANGE_ICDCMD_PATH,
		MENU_CHANGE_ICDCMD_OPTIONS,
		MENU_QUIT = wxID_EXIT,
		MENU_ABOUT = wxID_ABOUT,

		SERVER_ID = 100,
		SOCKET_ID
	};
private:
	wxTextCtrl *textCtrl;
	Programmer* programmer;
	Server* server;
	Registry *registry;

	static const wxString ABOUT_MESSAGE;
	DECLARE_EVENT_TABLE()
};

#endif
