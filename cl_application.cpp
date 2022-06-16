#include "cl_application.h"
#include "cl_obj2.h"
#include "cl_obj3.h"
#include "cl_obj4.h"
#include "cl_obj5.h"
#include "cl_obj6.h"


cl_application::cl_application(cl_base* root) : cl_base(root, "Object_root") { // конструктор, наследуемыей от конструтора базового класса
	class_num = 1;
}

int cl_application::exec_app() { // метод, запускающий приложение
	cout << "Object tree\n";
	this->print(); // вывод древа иерархии
	set_all_state(1);
	string cmd, path, str_arg;
	int state; // инициализация переменных
	cl_base* obj = nullptr;
	cl_base* cur = this; // указатель на текущий объект
	cin >> cmd;
	while (cmd != "END") {
		cin >> path;
		obj = find_object_by_path(path);
		if (cmd == "SET_CONDITION") {
			cin >> state;
			if (!obj) {
				cout << "\nObject " << path << " not found";
			} else {
				obj->set_state(state);
			}
		}
		if (cmd == "EMIT") {
			getline(cin, str_arg);
			if (!obj) {
				cout << "\nObject " << path << " not found";
			} else {
				obj->emit_signal(extract_signal(obj), str_arg);
			}
		}
		if (cmd == "SET_CONNECT") {
			cin >> str_arg;
			cl_base* reciever = find_object_by_path(str_arg);
			if (!obj) {
				cout << "\nObject " << path << " not found";
			} else if (!reciever) {
				cout << "\nHandler object " << str_arg << " not found";
			} else {
				obj->set_connection(extract_signal(obj), reciever, extract_handler(reciever));
			}
		}
		if (cmd == "DELETE_CONNECT") {
			cin >> str_arg;
			cl_base* reciever = find_object_by_path(str_arg);
			if (!obj) {
				cout << "\nObject " << path << " not found";
			} else if (!reciever) {
				cout << "\nHandler object " << str_arg << " not found";
			} else {
				obj->del_connection(extract_signal(obj), reciever, extract_handler(reciever));
			}
		}
		cin >> cmd;
	}
	return 0;
}


void cl_application::build_tree_objects() {
	string root_name;
	cin >> root_name;
	this->set_name(root_name);	//установка корневого объекта дерева
	while (cin >> root_name) {
		if (root_name == "endtree") {  //конец ввода
			break;
		}
		cl_base* root_obj = find_object_by_path(root_name); //поиск указатедя объекта по имени
		string derived_name; //имя зависимого объекта
		int num_class; //номер класса зависимого объекта
		cin >> derived_name >> num_class;
		if (root_obj == nullptr) {
			cout << "Object tree\n";
			this->print();
			cout << "\nThe head object " + root_name + " is not found";
			exit(1);
		}
		switch (num_class) { //создание объекта с соответствием номера его класса
		case 2:
			new cl_obj2(root_obj, derived_name);
			break;
		case 3:
			new cl_obj3(root_obj, derived_name);
			break;
		case 4:
			new cl_obj4(root_obj, derived_name);
			break;
		case 5:
			new cl_obj5(root_obj, derived_name);
			break;
		case 6:
			new cl_obj6(root_obj, derived_name);
			break;
		default:
			break;
		}
	}
	setup_connections();
}

cl_base::TYPE_SIGNAL cl_application::extract_signal(cl_base* obj) {
	int num = obj->get_class_num();
	if (num == 1) return TOSIGNAL(cl_application::signal);
	if (num == 2) return TOSIGNAL(cl_obj2::signal);
	if (num == 3) return TOSIGNAL(cl_obj3::signal);
	if (num == 4) return TOSIGNAL(cl_obj4::signal);
	if (num == 5) return TOSIGNAL(cl_obj5::signal);
	return TOSIGNAL(cl_obj6::signal);
}

cl_base::TYPE_HANDLER cl_application::extract_handler(cl_base* obj) {
	int num = obj->get_class_num();
	if (num == 1) return TOHANDLER(cl_application::handler);
	if (num == 2) return TOHANDLER(cl_obj2::handler);
	if (num == 3) return TOHANDLER(cl_obj3::handler);
	if (num == 4) return TOHANDLER(cl_obj4::handler);
	if (num == 5) return TOHANDLER(cl_obj5::handler);
	return TOHANDLER(cl_obj6::handler);
}

void cl_application::setup_connections() {
	string emitter_path, reciever_path;
	cl_base* emitter, * reciever;
	cin >> emitter_path;
	while (emitter_path != "end_of_connections") {
		emitter = find_object_by_path(emitter_path);
		cin >> reciever_path;
		reciever = find_object_by_path(reciever_path);
		emitter->set_connection(extract_signal(emitter), reciever, extract_handler(reciever));
		cin >> emitter_path;
	}
}

void cl_application::signal(string& msg) {
	cout << "\nSignal from " << this->get_abs_path();
	msg += " (class: 1)";
}

void cl_application::handler(string msg) {
	cout << "\nSignal to " << this->get_abs_path() << " Text: " << msg;
}
