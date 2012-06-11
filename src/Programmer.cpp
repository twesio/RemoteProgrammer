#include "hpp/Programmer.hpp"
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/process.h>

Programmer::Programmer(MainFrame& mainFrame): mainFrame(&mainFrame) {
	this->registry = new Registry();
	this->hex = new wxString();
}

Programmer::~Programmer() {

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
	wxString params = this->registry->getIcdCmdOptions();
	params.Replace(Registry::ICD3CMD_OPTIONS_PLACEHOLDER_PATH, fileName, true);

	wxString cmd = wxString::Format(_T("\"%s\" %s"), this->registry->getIcdCmdPath().c_str(), params.c_str());

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
