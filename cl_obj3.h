#ifndef _CL_OBJ3_H_
#define _CL_OBJ3_H_

#include "cl_base.h"

class cl_obj3 : public cl_base {
public:
	cl_obj3(cl_base* head_obj, string name);	//конструктор
	~cl_obj3() {}
	void signal(string& msg); //метод сигнала
	void handler(string msg); //метода обработки сигнала
};


#endif