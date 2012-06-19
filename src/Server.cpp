#include "hpp/Server.hpp"
#include <wx/file.h>

Server::Server(MainFrame& mainFrame, Programmer& programmer): mainFrame(&mainFrame), programmer(&programmer), registry() {
	this->setStarted(false);
	this->setBusy(false);
}

Server::~Server() {
	delete socketServer;
}

void Server::start() {
	if(this->isStarted()) {
		this->addToLog(_("Error: Server is already running"));
		return;
	}

	this->addToLog(_("Starting server.."));

	// Try to find the ICD3CMD executable
	if(wxFile::Exists(this->registry->getIcdCmdPath())) {
		this->addToLog(_("Found ICD3CMD"));
	} else {
		this->addToLog(wxString::Format(_("Unable to find ICD3CMD (%s)"), this->registry->getIcdCmdPath().c_str()));
		this->addToLog(_("Select [Settings -> Change ICD3CMD Path] from the menu to configure the path"));

		return;
	}

	// Create socket
	wxIPV4address addr;
	addr.Service(3000); // Todo: Allow customization

	this->socketServer = new wxSocketServer(addr);

	if(this->socketServer->Ok()) {
		this->addToLog(wxString::Format(_("Server listening on %s:%d"), addr.IPAddress().c_str(), 3000));
	} else {
		this->addToLog(_("Error opening socket"));
		return;
	}

	this->socketServer->SetEventHandler(*mainFrame, MainFrame::SERVER_ID);
	this->socketServer->SetNotify(wxSOCKET_CONNECTION_FLAG);
	this->socketServer->Notify(true);

	this->programmer->getHex().Clear();
	this->setStarted(true);
	this->setBusy(false);
}

void Server::stop() {
	if(!this->isStarted()) {
		this->addToLog(_("Error: Server is not running"));
		return;
	}

	this->addToLog(_("Stopping server.."));
	this->socketServer->Destroy();
	this->setStarted(false);

	// Todo: Disconnect all clients
}

bool Server::isStarted() {
	return _isStarted;
}

bool Server::isBusy() {
	return this->_isBusy;
}

void Server::setBusy(const bool busy) {
	this->_isBusy = busy;

	if(busy) {
		this->programmer->getHex().Clear();
		mainFrame->SetStatusText(_("Client connected"));
	} else {
		mainFrame->SetStatusText(_("Ready"));
	}
}

void Server::handleServerEvent(wxSocketEvent& event) {
	wxSocketBase *sock;

	// Accept the new connection if there is one in the pending connections queue
	// Accept(false) is used for non-blocking accept (although if we got here, there
	// should always be a pending connection)
	sock = socketServer->Accept(false);
	if(sock) {
		wxIPV4address addr;
		sock->GetPeer(addr);

		if(this->isBusy()) {
			sock->Destroy();
			this->addToLog(wxString::Format(_("Client (%s) dropped. Sorry, but I'm busy with someone else."), addr.IPAddress().c_str()));
			return;
		} else {
			this->addToLog(wxString::Format(_("Client (%s) connected"), addr.IPAddress().c_str()));
			this->setBusy(true);
		}
	} else {
		this->addToLog(_("Error: Couldn't accept the new connection"));
	}

	// Set the event handler for that connection
	sock->SetEventHandler(*mainFrame, MainFrame::SOCKET_ID);
	sock->SetNotify(wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
	sock->Notify(true);
}

void Server::handleSocketEvent(wxSocketEvent& event) {
	wxSocketBase *sock = event.GetSocket();
	wxIPV4address addr;
	sock->GetPeer(addr);

	// 4kb buffer
	wxUint32 bufSize = 4096;

	wxChar *buf = new wxChar[bufSize];
	wxUint32 len;

	// Process the event
	switch(event.GetSocketEvent()) {
		case wxSOCKET_INPUT:
			// Disable input events, so that this event does not trigger again
			sock->SetNotify(wxSOCKET_LOST_FLAG);

			// Read a character
			len = sock->Read(buf, bufSize).LastCount();
			this->addToLog(wxString::Format(_("Client (%s) sent %d bytes"), addr.IPAddress().c_str(), len));
			this->programmer->getHex().Append(buf, len);

			if(programmer->program(sock)) {
				sock->Destroy();
				this->setBusy(false);
				this->addToLog(wxString::Format(_("Client (%s) dropped. Upload complete."), addr.IPAddress().c_str()));
			}

			// Enable input events again
			sock->SetNotify(wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);
			break;
		case wxSOCKET_LOST:
			sock->Destroy();
			this->setBusy(false);
			this->addToLog(wxString::Format(_("Client (%s) disconnected"), addr.IPAddress().c_str()));
			break;
		default:
			break;
	}
}

void Server::addToLog(const wxString& msg) {
	this->mainFrame->addToLog(msg);
}

void Server::setStarted(bool isStarted) {
	this->_isStarted = isStarted;
}
