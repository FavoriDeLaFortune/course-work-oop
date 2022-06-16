#ifndef _CL_OBJ2_H_
#define _CL_OBJ2_H_

#include "cl_base.h"

class cl_obj2 : public cl_base {
public:
	cl_obj2(cl_base* head_obj, string name);	//конструктор
	~cl_obj2() {}
	void signal(string& msg); //метод сигнала
	void handler(string msg); //метода обработки сигнала
};


#endif