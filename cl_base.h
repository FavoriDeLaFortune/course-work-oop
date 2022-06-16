#ifndef _CL_BASE_H_
#define _CL_BASE_H_

#define TOSIGNAL(signal_f) (TYPE_SIGNAL) (&signal_f)
#define TOHANDLER(handler_f) (TYPE_HANDLER) (&handler_f)

#include <iostream>
#include <vector>
#include <string>
#include <cstddef>

using namespace std;

class cl_base {
public:
	cl_base(cl_base* head_obj, string name = "Base_object");	//конструктор
	~cl_base();
	void set_name(string name);									//сеттер имени
	string get_name();											//геттер имени
	void print(string spaces = "");								//вывод иерархического дерева
	void print_with_state(string spaces = "");					//вывод иерархического дерева
	void set_head_obj(cl_base* head_obj);						//поменять головной объект
	cl_base* get_head_obj();									//геттер указателя на головной объект
	cl_base* find_object_by_name(string name);					//поиск объекта по имени
	void set_state(int state);									//установка состояния объекта
	cl_base* find_object_by_path(string path);					//поиск объекта по пути
	typedef void (cl_base::* TYPE_SIGNAL)(string&);
	typedef void (cl_base::* TYPE_HANDLER)(string);
	void set_connection(TYPE_SIGNAL signal, cl_base* target, TYPE_HANDLER handler);
	void set_all_state(int state);	//установка всем объектам состояние state
	void del_connection(TYPE_SIGNAL signal, cl_base* target, TYPE_HANDLER handler);	//метод удаления связи
	void emit_signal(TYPE_SIGNAL, string&); 					//метод выдачи сигнала
	int get_class_num();
	string get_abs_path(string end = "");


private:
	struct Connection {
		TYPE_SIGNAL signal;
		cl_base* target;
		TYPE_HANDLER handler;
	};
	vector<Connection*> connections;
	string name;					//имя объекта
	cl_base* head_obj = nullptr;	//указатель на головной объект
	vector<cl_base* > subclasses;	//массив подчинённых объектов
	int state = 0;					//состояние объекта

protected:
	int class_num;					//номер класса объекта
};


#endif