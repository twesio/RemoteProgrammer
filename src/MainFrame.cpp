#include "hpp/MainFrame.hpp"
#include <iostream>
#include "images/icon.xpm"

MainFrame::MainFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(1000, 300)) {
	this->programmer = new Programmer(*this);
	this->server = new Server(*this, *(this->programmer));
	this->registry = new Registry();

	// application icon
	this->SetIcon(wxIcon(icon_xpm));

	// menu bar
	wxMenu *fileMenu = new wxMenu;
	fileMenu->Append(MainFrame::MENU_START, _("St&art server\tCtrl-E"), _("Start listening for clients"));
	fileMenu->Append(MainFrame::MENU_STOP, _("St&op server\tCtrl-R"), _("Stop listening for clients"));
	fileMenu->AppendSeparator();
	fileMenu->Append(MainFrame::MENU_QUIT, _("E&xit\tCtrl-Q"), _("Quit this program"));

	wxMenu *settingsMenu = new wxMenu;
	settingsMenu->Append(MainFrame::MENU_CHANGE_ICDCMD_PATH, _("Change ICD3CMD &Path\tCtrl-O"), _("Opens a file dialog.."));
	settingsMenu->Append(MainFrame::MENU_CHANGE_ICDCMD_OPTIONS, _("Change ICD3CMD &Options\tCtrl-T"), _("Opens a file dialog.."));

	wxMenu *helpMenu = new wxMenu;
	helpMenu->Append(MainFrame::MENU_ABOUT, _("&About\tF1"), _("Show about dialog"));

	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, _("&File"));
	menuBar->Append(settingsMenu, _("&Settings"));
	menuBar->Append(helpMenu, _("&Help"));

	// and attach this menu bar to the frame
	SetMenuBar(menuBar);

	// status bar
	CreateStatusBar(1);

	// Logging box
	textCtrl = new wxTextCtrl(
		this, wxID_ANY, _T(""),
		wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxBORDER_SIMPLE
	);

	this->addToLog(_("Application started"));
	server->start();
}

MainFrame::~MainFrame() {
}

void MainFrame::addToLog(const wxString& msg) {
	this->textCtrl->AppendText(
		wxString::Format(_("[%s] %s\n"), wxDateTime::Now().FormatISOTime().c_str(), msg.c_str())
	);

	this->textCtrl->SetFocus();
	this->textCtrl->ShowNativeCaret(false);
}

// event handlers
void MainFrame::OnChangeIcdCmdPath(wxCommandEvent& WXUNUSED(event)) {
	wxFileDialog* dialog = new wxFileDialog(this, "Select ICD3CMD.EXE ..", wxEmptyString, wxEmptyString, _("*.exe"));
	if(dialog->ShowModal() == wxID_OK) {
		this->registry->setIcdCmdPath(dialog->GetPath());

		this->addToLog(wxString::Format(_("ICD3CMD path changed to %s"), this->registry->getIcdCmdPath().c_str()));
		this->addToLog(_("Select [File -> Start server] from the menu to run the server"));
	}
}

void MainFrame::OnChangeIcdCmdOptions(wxCommandEvent& WXUNUSED(event)) {
	wxString intro = wxString::Format(_(
		"Note:\n"
		"There is a placeholder $PATH which will be substituted with the path of the file which contains the transferred hex data.\n"
		"The default options are: %s\n\n"
		"Command line options:"
	), Registry::ICD3CMD_OPTIONS_DEFAULT.c_str());

	wxString options = wxGetTextFromUser(intro, _("Set ICD3CMD options"), this->registry->getIcdCmdOptions(), this);

	if(options.Length() == 0) {
		this->addToLog(_("Edit of ICD3CMD Options cancelled."));
		return;
	}

	this->registry->setIcdCmdOptions(options);
}

void MainFrame::OnStart(wxCommandEvent& WXUNUSED(event)) {
	server->start();
}

void MainFrame::OnStop(wxCommandEvent& WXUNUSED(event)) {
	server->stop();
}

void MainFrame::OnQuit(wxCommandEvent& WXUNUSED(event)) {
	// true forces the frame to close
	Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event)) {
	wxMessageBox(MainFrame::ABOUT_MESSAGE, _("About"), wxOK | wxICON_INFORMATION, this);
}

void MainFrame::OnServerEvent(wxSocketEvent& event) {
	server->handleServerEvent(event);
}

void MainFrame::OnSocketEvent(wxSocketEvent& event) {
	server->handleSocketEvent(event);
}

const wxString MainFrame::ABOUT_MESSAGE(_(
	"RemoteProgrammer 0.1\n"
	"Copyright (C) 2012 snd\n\n"
	"This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY."
));
