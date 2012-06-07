#include <wx/wx.h>
#include "hpp/Application.hpp"
#include "hpp/MainFrame.hpp"

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(MainFrame::MENU_SELECT_PATH, MainFrame::OnSelectPath)
	EVT_MENU(MainFrame::MENU_START, MainFrame::OnStart)
	EVT_MENU(MainFrame::MENU_STOP, MainFrame::OnStop)
	EVT_MENU(MainFrame::MENU_QUIT, MainFrame::OnQuit)
	EVT_MENU(MainFrame::MENU_ABOUT, MainFrame::OnAbout)
	EVT_SOCKET(MainFrame::SERVER_ID, MainFrame::OnServerEvent)
	EVT_SOCKET(MainFrame::SOCKET_ID, MainFrame::OnSocketEvent)
END_EVENT_TABLE()

IMPLEMENT_APP(Application)
