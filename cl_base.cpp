#include "cl_base.h"

cl_base::cl_base(cl_base* head_obj, string name) {
	this->name = name;
	this->head_obj = head_obj;
	if (head_obj) {	//если объект не является корнем дерева
		head_obj->subclasses.push_back(this);
	}
}

cl_base::~cl_base() {
	for (int i = 0; i < subclasses.size(); ++i) {
		delete subclasses[i];
	}
}

void cl_base::set_name(string name) {
	this->name = name;
}

string cl_base::get_name() {
	return this->name;
}

void cl_base::print(string spaces) {
	if (this->head_obj) { //если объект имеет родительский объект, то нужно сделать отступ
		cout << '\n';
	}
	cout << spaces + this->name;
	if (this->subclasses.size() == 0) { //если объект не имеет дочерних объектов, то предыдущему объекту
		return;
	}
	for (int i = 0; i < this->subclasses.size(); ++i) { // вывод подчинённых объектов
		this->subclasses[i]->print(spaces + "    ");
	}
}

void cl_base::print_with_state(string spaces) {
	if (this->head_obj) { //если объект имеет родительский объект, то нужно сделать отступ
		cout << '\n';
	}
	cout << spaces + this->name;
	if (this->state) {
		cout << " is ready";
	} else {
		cout << " is not ready";
	}
	if (this->subclasses.size() == 0) { //если объект не имеет дочерних объектов, то предыдущему объекту
		return;
	}
	for (int i = 0; i < this->subclasses.size(); ++i) { // вывод подчинённых объектов
		this->subclasses[i]->print_with_state(spaces + "    ");
	}
}

void cl_base::set_head_obj(cl_base* head_obj) {
	if (this->head_obj) { //если не корневой объект дерева
		for (int i = 0; i < (this->head_obj)->subclasses.size(); ++i) { //поиск текущего объекта в массиве подчинённых объектов головного объекта
			if ((this->head_obj)->subclasses[i] == this) {
				(this->head_obj)->subclasses.erase((this->head_obj)->subclasses.begin() + i); //удаление текущего объекта из массива
				break;
			}
		}
		head_obj->subclasses.push_back(this); //привязка текущего объекта к другому головному объекту
		this->head_obj = head_obj;
	}
}

cl_base* cl_base::get_head_obj() {
	return this->head_obj;
}

cl_base* cl_base::find_object_by_name(string name) {
	if (this->name == name) { //если объект найден по имени, то возвращаем указатель на него
		return this;
	}
	cl_base* obj = nullptr;
	for (int i = 0; i < this->subclasses.size(); ++i) {
		if (obj == nullptr) { //пока объект не найден, то продолжаем поиск по дереву
			obj = this->subclasses[i]->find_object_by_name(name);
		} else {
			break;
		}
	}
	return obj;
}

void cl_base::set_state(int state) {
	if (state) { //если текущий объект требуется включить
		cl_base* head_obj = this->head_obj;
		bool flag = true; //флаг, указывающий на то, что объекты сверху по дереву включены
		while (head_obj != nullptr) {
			if (head_obj->state) { //если головной объект включен
				head_obj = head_obj->head_obj;	//идем дальше наверх по иерархии дерева
			} else {
				flag = false;
				break;
			}
		}
		if (flag) { //если все объекты сверху включены, то включаем текущий объект
			this->state = state;
		}
	} else {
		this->state = state; //выключаем текущий объект
		for (int i = 0; i < this->subclasses.size(); ++i) { //выключаем все объекты снизу
			this->subclasses[i]->set_state(state);
		}
	}
}

cl_base* cl_base::find_object_by_path(string path) {
	if (path.find("//") == 0) {
		if (this->head_obj == nullptr) return this->find_object_by_name(path.erase(0, 2));
		return this->head_obj->find_object_by_path(path);
	}
	if (path[0] == '/' && path.size() == 1) {
		if (this->head_obj == nullptr) return this;
		return this->head_obj->find_object_by_path("/");
	}
	if (path[0] == '/') {
		if (this->head_obj == nullptr) return find_object_by_path(path.erase(0, 1));
		return this->head_obj->find_object_by_path(path);
	}
	if (path.find("/") != -1 && path.find("/") != 0) {
		string step = path, obj_name;
		for (int i = 0; i < path.find("/"); i++) {
			obj_name.push_back(path[i]);
			step.erase(0, 1);
		}
		for (int i = 0; i < subclasses.size(); i++) {
			if (subclasses[i]->get_name() == obj_name)
				return subclasses[i]->find_object_by_path(step.erase(0, 1));
		}
	}
	if (path == ".") return this;
	for (int i = 0; i < subclasses.size(); i++) {
		if (subclasses[i]->get_name() == path) return subclasses[i];
	}
	return nullptr;
}

void cl_base::set_connection(TYPE_SIGNAL signal, cl_base* target, TYPE_HANDLER handler) {
	for (int i = 0; i < connections.size(); i++) { //идем по всем установленным связям
		if (connections[i]->signal == signal && connections[i]->target == target && connections[i]->handler == handler) { //проверка на существование уже требуемой связи
			return;
		}
	}
	Connection* cn = new Connection; //создаем новую связь
	cn->signal = signal;
	cn->target = target;
	cn->handler = handler;
	connections.push_back(cn);
}

void cl_base::del_connection(TYPE_SIGNAL signal, cl_base* target, TYPE_HANDLER handler) {
	for (int i = 0; i < connections.size(); i++) { //идем по всем установленным связям
		Connection* cn = connections[i];
		if (cn->signal == signal && cn->target == target && cn->handler == handler) { //если нашли связь, которую нужно удалить
			delete connections[i];
			return;
		}
	}
}

void cl_base::emit_signal(TYPE_SIGNAL signal, string& msg) {
	if (state == 0) { //если объект не включен
		return;
	}
	(this->*signal)(msg); //создание сигнала с посланием msg
	if (connections.empty()) { //если никаких связей нет
		return;
	}
	for (int i = 0; i < connections.size(); i++) { //идем по массиву связей
		Connection* cn = connections[i];
		if (cn->signal == signal && cn->target->state != 0) { //если текущий объект имеет связь с целевым объект, который имеет включенное состояние
			(cn->target->*cn->handler)(msg); //передаем послание
		}
	}
}

string cl_base::get_abs_path(string end) {
	if (!this->head_obj) {
		return "/";
	}
	return this->head_obj->get_abs_path("/") + this->name + end;
}

int cl_base::get_class_num() {
	return this->class_num;
}

void cl_base::set_all_state(int state) {
	this->set_state(state);
	for (int i = 0; i < subclasses.size(); i++) {
		subclasses[i]->set_all_state(state);
	}
}

















