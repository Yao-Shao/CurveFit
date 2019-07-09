#include "ViewModelSink.h"

ViewModelSink::ViewModelSink(ViewModel* p) throw():ptr_viewmodel(p)
{
}

void ViewModelSink::OnPropertyChanged(const std::string& str)
{
	ptr_viewmodel->Fire_OnPropertyChanged(str);
}
