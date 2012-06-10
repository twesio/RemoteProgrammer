#include "hpp/Application.hpp"
#include "hpp/MainFrame.hpp"

bool Application::OnInit() {
	if(!wxApp::OnInit()) {
		return false;
	}

	// application window
	MainFrame *frame = new MainFrame(_("RemoteProgrammer"));
	frame->Show(true);

	return true;
}
