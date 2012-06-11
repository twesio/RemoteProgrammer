#ifndef PROGRAMMER_HPP_
#define PROGRAMMER_HPP_
#include <wx/wx.h>
#include <wx/socket.h>
#include "MainFrame.hpp"
#include "Registry.hpp"

class MainFrame;

class Programmer {
public:
	Programmer(MainFrame& mainFrame);
	~Programmer();

	bool program(wxSocketBase *sock);
	wxString& getHex();
private:
	MainFrame* mainFrame;
	Registry *registry;
	wxString *hex;
};

#endif
