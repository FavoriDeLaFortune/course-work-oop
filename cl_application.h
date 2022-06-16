#ifndef _CL_APPLICATION_H_
#define _CL_APPLICATION_H_

#include "cl_base.h"

class cl_application : public cl_base {
public:
	cl_application(cl_base* head_obj); //конструктор
	~cl_application() {}
	void build_tree_objects();			//создания иерархического дерева
	int exec_app();						//запуск системы
	void setup_connections();
	void signal(string& msg);
	void handler(string msg);
	TYPE_SIGNAL extract_signal(cl_base* obj);
	TYPE_HANDLER extract_handler(cl_base* obj);
};


#endif