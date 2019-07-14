
#include "../COMMON/etlbase.h"
#include "../ViewModel/ViewModel.h"
#define ERRORBOUND 1e-6

bool ViewModel_Test() throw() {

	try {
		std::shared_ptr<Model> model(std::make_shared<Model>());
		std::shared_ptr<ViewModel> vmdl(std::make_shared<ViewModel>());
		vmdl->SetModel(model);

		//case 1 y = 2x+3
		Param_opcf p;
		Type t = LINEAR_FUNCTION;
		Points sampleP;
		Point p0(1, 5);
		sampleP.push_back(p0);
		Point p1(-1, 1);
		sampleP.push_back(p1);
		p.set_type(LINEAR_FUNCTION);
		p.set_point(sampleP);
		vmdl->get_fitCommand()->SetParameter(p);
		vmdl->get_fitCommand()->Exec();
		std::cout << "In case 1,test fit function 2x+3, use (1,5) (-1,1) and check whether y(10) is 23\n";
		std::cout << abs((vmdl->getFunction())->get_y(10)) << std::endl;
		if (abs((vmdl->getFunction())->get_y(10)) - 23 > ERRORBOUND)
			throw int(1);
		//case 2 y = x^2 + 4*x + 5
		Param_opcf param1;
		t = QUADRATIC_FUNCTION;
		sampleP.clear();
		Point pq0(0, 5);
		sampleP.push_back(pq0);
		Point pq1(1, 10);
		sampleP.push_back(pq1);
		Point pq2(-1, 2);
		sampleP.push_back(pq2);
		param1.set_type(QUADRATIC_FUNCTION);
		param1.set_point(sampleP);
		vmdl->get_fitCommand()->SetParameter(param1);
		vmdl->get_fitCommand()->Exec();
		std::cout << "In case 2,test fit function y = x^2 + 4*x + 5, use (0,5) (1,10),(-1,2) and check whether y(10) is 145\n";
		std::cout << abs((vmdl->getFunction())->get_y(10)) << std::endl;
		if (abs((vmdl->getFunction())->get_y(10)) - 145 > ERRORBOUND)
			throw int(2);
		/*
		//case 3 y = x^3 + x^2 + x + 5
		Param_opcf param2;
		t = CUBIC_FUNCTION;
		sampleP.clear();
		Point pc0(0, 5);
		sampleP.push_back(pc0);
		Point pc1(1, 8);
		sampleP.push_back(pc1);
		Point pc2(-1, 4);
		sampleP.push_back(pc2);
		Point pc3(2, 19);
		sampleP.push_back(pc3);
		param2.set_type(CUBIC_FUNCTION);
		param2.set_point(sampleP);
		vmdl->get_fitCommand()->SetParameter(param2);
		vmdl->get_fitCommand()->Exec();
		std::cout << "In case 3,test fit function y =  x^3 + x^2 + x + 5, use (0,5) (1,8),(-1,4),(2,19) and check whether y(3) is 44\n";
		std::cout << abs((vmdl->getFunction())->get_y(3)) << std::endl;
		if (abs((vmdl->getFunction())->get_y(3)) - 44 > ERRORBOUND)
			throw int(3);
		*/
		//case 4 y = 2 * ln x + 1
		Param_opcf param3;
		t = LN_FUNCTION;
		sampleP.clear();
		Point pl0(1, 1);
		sampleP.push_back(pl0);
		Point pl1(2, 2.38629436);
		sampleP.push_back(pl1);
		param3.set_type(LN_FUNCTION);
		param3.set_point(sampleP);
		vmdl->get_fitCommand()->SetParameter(param3);
		vmdl->get_fitCommand()->Exec();
		std::cout << "In case 4,test fit function y = 2*lnx + 1, use (1,0) (2,2.38629436), and check whether y(10) is 5.60517019\n";
		std::cout << abs((vmdl->getFunction())->get_y(10)) << std::endl;
		if (abs((vmdl->getFunction())->get_y(10)) - 5.60517019 > ERRORBOUND)
			throw int(3);
	}
	catch (int& v) {
		printf("case %d error\n", v);
		return false;
	}
	catch (...) {
		printf("exception\n");
		return false;
	}
	return true;
}