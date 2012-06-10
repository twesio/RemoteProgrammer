#ifndef PROGRAMMER_HPP_
#define PROGRAMMER_HPP_
#include <wx/wx.h>
#include <wx/socket.h>
#include "MainFrame.hpp"

class MainFrame;

class Programmer {
public:
	Programmer(MainFrame& mainFrame);
	~Programmer();

	wxString getIcd3cmdPath();
	void setIcd3cmdPath(const wxString& path);
	bool program(wxSocketBase *sock);
	wxString& getHex();

	static const wxString ICD3CMD_PATH_DEFAULT;
	static const wxString ICD3CMD_REG_KEY;
	static const wxString ICD3CMD_REG_SUBKEY_NAME;
private:
	MainFrame* mainFrame;
	wxString *hex;
};

#endif
