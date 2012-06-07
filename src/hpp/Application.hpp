#ifndef APPLICATION_HPP_
#define APPLICATION_HPP_
#include <wx/wx.h>

class Application: public wxApp {
	public:
		virtual bool OnInit();
};

#endif
