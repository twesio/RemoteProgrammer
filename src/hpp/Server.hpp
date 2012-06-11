#ifndef SERVER_HPP_
#define SERVER_HPP_
#include <wx/wx.h>
#include <wx/socket.h>
#include "MainFrame.hpp"
#include "Programmer.hpp"
#include "Registry.hpp"

class MainFrame;
class Programmer;

class Server {
public:
	Server(MainFrame& mainFrame, Programmer& programmer);
	~Server();

	void start();
	void stop();
	bool isStarted();
	bool isBusy();
	void setBusy(const bool busy);
	void handleServerEvent(wxSocketEvent& event);
	void handleSocketEvent(wxSocketEvent& event);
private:
	void addToLog(const wxString& msg);
	void setStarted(bool isStarted);
	bool _isStarted;
	bool _isBusy;
	MainFrame* mainFrame;
	Programmer* programmer;
	Registry* registry;
	wxSocketServer *socketServer;
};

#endif
