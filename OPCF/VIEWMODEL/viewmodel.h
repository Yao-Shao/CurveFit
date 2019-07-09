#pragma once


#include "../MODEL/Model.h"
#include "../common/etlbase.h"


class ViewModel :public Proxy_PropertyNotification<ViewModel>,
	public Proxy_CommandNotification<ViewModel>
{
public:
	ViewModel();
	~ViewModel() {}

private:

};
