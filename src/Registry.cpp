#include "hpp/Registry.hpp"

const wxString Registry::PARENT_REG_KEY("Software\\RemoteProgrammer");

const wxString Registry::ICD3CMD_PATH_DEFAULT("C:\\Program Files (x86)\\Microchip\\MPLAB IDE\\Programmer Utilities\\ICD3\\ICD3CMD.EXE");
const wxString Registry::ICD3CMD_PATH_REG_KEY_NAME("ICD3CMD_PATH");

const wxString Registry::ICD3CMD_OPTIONS_DEFAULT("-P32MX795F512H -L -M -F\"$PATH\"");
const wxString Registry::ICD3CMD_OPTIONS_REG_KEY_NAME("ICD3CMD_OPTIONS");

const wxString Registry::ICD3CMD_OPTIONS_PLACEHOLDER_PATH("$PATH");

Registry::Registry() {
	// Initialize required keys
	this->initValue(Registry::ICD3CMD_PATH_REG_KEY_NAME, Registry::ICD3CMD_PATH_DEFAULT);
	this->initValue(Registry::ICD3CMD_OPTIONS_REG_KEY_NAME, Registry::ICD3CMD_OPTIONS_DEFAULT);
}

Registry::~Registry() {

}

wxString Registry::getIcdCmdPath() {
	return this->getValue(Registry::ICD3CMD_PATH_REG_KEY_NAME);
}

wxString Registry::getIcdCmdOptions() {
	return this->getValue(Registry::ICD3CMD_OPTIONS_REG_KEY_NAME);
}

void Registry::setIcdCmdPath(const wxString& value) {
	this->setValue(Registry::ICD3CMD_PATH_REG_KEY_NAME, value);
}

void Registry::setIcdCmdOptions(const wxString& value) {
	this->setValue(Registry::ICD3CMD_OPTIONS_REG_KEY_NAME, value);
}

wxRegKey* Registry::getParentKey() {
	wxRegKey* parentKey = new wxRegKey(wxRegKey::HKLM, Registry::PARENT_REG_KEY);

	// Create if it does not exist
	if(!parentKey->Exists()) {
		parentKey->Create(false);
	}

	return parentKey;
}

wxString Registry::getValue(const wxString& name) {
	wxString value("");
	this->getParentKey()->QueryValue(name, value);
	return value;
}

bool Registry::hasValue(const wxString& name) {
	return this->getParentKey()->HasValue(name);
}

void Registry::setValue(const wxString& name, const wxString& value) {
	this->getParentKey()->SetValue(name, value);
}

void Registry::initValue(const wxString& name, const wxString& value) {
	if(!this->hasValue(name)) {
		this->setValue(name, value);
	}
}
