#include "../viewmodel.h"
#define NDEBUG

ViewModelSink::ViewModelSink(ViewModel* p) throw():ptr_viewmodel(p)
{
}

void ViewModelSink::OnPropertyChanged(const std::string& str)
{
#ifndef NDEBUG
	qDebug() << "viewmodel sink get fire\nand send fire view_model->fireonPropertychange()\n";
#endif // !NDEBUG
	ptr_viewmodel->Fire_OnPropertyChanged(str);
}
