#include "hpp/Programmer.hpp"
#include <wx/msw/registry.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/process.h>

const wxString Programmer::ICD3CMD_PATH_DEFAULT("C:\\Program Files (x86)\\Microchip\\MPLAB IDE\\Programmer Utilities\\ICD3\\ICD3CMD.EXE");
const wxString Programmer::ICD3CMD_REG_KEY("Software\\RemoteProgrammer");
const wxString Programmer::ICD3CMD_REG_SUBKEY_NAME("ICD3CMD_PATH");

Programmer::Programmer(MainFrame& mainFrame): mainFrame(&mainFrame) {
	this->hex = new wxString();

	// Initialize path to ICD3CMD in registry
	wxRegKey rpKey(wxRegKey::HKLM, Programmer::ICD3CMD_REG_KEY);
	if(!rpKey.Exists()) {
		rpKey.Create(false);
	}
	if(!rpKey.HasValue(Programmer::ICD3CMD_REG_SUBKEY_NAME)) {
		rpKey.SetValue(Programmer::ICD3CMD_REG_SUBKEY_NAME, Programmer::ICD3CMD_PATH_DEFAULT);
	}
}

Programmer::~Programmer() {

}

wxString Programmer::getIcd3cmdPath() {
	wxRegKey rpKey(wxRegKey::HKLM, Programmer::ICD3CMD_REG_KEY);
	wxString path("");
	rpKey.QueryValue(Programmer::ICD3CMD_REG_SUBKEY_NAME, path);

	return path;
}

void Programmer::setIcd3cmdPath(const wxString& path) {
	wxRegKey rpKey(wxRegKey::HKLM, Programmer::ICD3CMD_REG_KEY);
	rpKey.SetValue(Programmer::ICD3CMD_REG_SUBKEY_NAME, path);

	this->mainFrame->addToLog(wxString::Format(_("ICD3CMD path changed to %s"), path.c_str()));
	this->mainFrame->addToLog(_("Select [File -> Start server] from the menu to run the server"));
}

bool Programmer::program(wxSocketBase *sock) {
	if(!this->hex->Contains(_(":00000001FF"))) {
		return false;
	}

	this->mainFrame->SetStatusText(_("Programming"));
	this->mainFrame->addToLog(_("Detected EOF Record"));
	this->hex->Shrink();

	// Write recieved data into a temporary file
	wxString fileName = wxFileName::CreateTempFileName(_T("rp"));
	wxFile *file = new wxFile();
	file->Open(fileName, wxFile::write);
	file->Write(this->hex->c_str(), this->hex->Length());
	file->Flush();
	file->Close();
	delete file;

	// Flash hex into the device
	wxString params = wxString::Format(_T("-P32MX795F512H -L -M -F\"%s\""), fileName.c_str());
	wxString cmd = wxString::Format(_T("\"%s\" %s"), this->getIcd3cmdPath().c_str(), params.c_str());
	this->mainFrame->addToLog(wxString::Format(_("Hex file has %d bytes"), this->hex->Length()));
	this->mainFrame->addToLog(wxString::Format(_("Invoking %s"), cmd.c_str()));

	// Tell the client which command will be used
	wxString msg = wxString::Format(_("Invoking ICD3CMD %s\n"), params.c_str());
	sock->Write(msg, msg.Length());

	wxProcess* process = wxProcess::Open(cmd);
	if(!process) {
		this->mainFrame->addToLog(_("Error opening ICD3CMD"));
		return false;
	}

	wxInputStream *in = process->GetInputStream();
	bool firstRead = true;
	while(!in->Eof()) {
		wxString response;
		wxChar buffer[256];

		in->Read(buffer, sizeof(buffer));
		response.Append(buffer, in->LastRead());

		// Beautify the icd3cmd output
		if(firstRead) {
			response.Trim(false);
			firstRead = false;
		} else if(in->Eof()) {
			response.Trim().Append("\n");
		}

		response.Shrink();
		sock->Write(response, response.length());
	}

	// Cleanup
	if(remove(fileName.c_str()) == 0) {
		this->mainFrame->addToLog(_("Successfully removed temporary hex file"));
	} else {
		this->mainFrame->addToLog(_("Removal of temporary hex file failed"));
	}

	// Disconnects the client
	return true;
}

wxString& Programmer::getHex() {
	return *this->hex;
}
