#ifndef REGISTRY_HPP_
#define REGISTRY_HPP_
#include <wx/wx.h>
#include <wx/msw/registry.h>

class Registry {
public:
	Registry();
	~Registry();

	wxString getIcdCmdPath();
	wxString getIcdCmdOptions();

	void setIcdCmdPath(const wxString& value);
	void setIcdCmdOptions(const wxString& value);

	static const wxString PARENT_REG_KEY;
	static const wxString ICD3CMD_PATH_DEFAULT;
	static const wxString ICD3CMD_PATH_REG_KEY_NAME;

	static const wxString ICD3CMD_OPTIONS_DEFAULT;
	static const wxString ICD3CMD_OPTIONS_REG_KEY_NAME;

	static const wxString ICD3CMD_OPTIONS_PLACEHOLDER_PATH;

private:
	wxRegKey* getParentKey();
	wxString getValue(const wxString& name);
	bool hasValue(const wxString& name);
	void setValue(const wxString& name, const wxString& value);
	void initValue(const wxString& name, const wxString& value);
};


#endif
