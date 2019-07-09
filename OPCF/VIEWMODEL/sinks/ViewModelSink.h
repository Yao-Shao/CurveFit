#pragma once

#include "../../COMMON/etlbase.h"
#include "../viewmodel.h"

class  ViewModel;

class ViewModelSink:public IPropertyNotification
{
public:
	ViewModelSink(ViewModel* p) throw();

	virtual void OnPropertyChanged(const std::string& str);

private:
	ViewModel* ptr_viewmodel;
};

