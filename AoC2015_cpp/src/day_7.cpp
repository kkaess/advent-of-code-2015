#include <string>
#include <iostream>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <memory>
#include <map>
#include <functional>
#include <fstream>
//#include <regex>
#include <boost/regex.hpp>

using namespace boost;
using namespace std;


/*class BCircuitElement;
 class VarElement;
 class ConstElement;

 ConstElement ConstElement::ConstElement();*/

static const map<string, int (*)(int, int)> binOpTable {
	{ "LSHIFT", [](int a,int b) {return a << b;} },
	{ "RSHIFT", [](int a, int b) {return a >> b;} },
	{ "OR", [](int a, int b) {return a | b;} },
	{ "AND", [](int a, int b) {return a & b;} }
};
static const map<string, int (*)(int)> unOpTable {
	{ "NOT", [](int a) {return ~a;} }
};

class BCircuitElement {
public:
	BCircuitElement() {

	}

	virtual int evaluate() {
		throw runtime_error(
				"Should not be calling evaluate on BCircuitElement");
		return 0;
	}

	void add_child(std::shared_ptr<BCircuitElement> &bCircuitElement) {
		if (this->children.size() < this->max_children) {
			this->children.push_back(bCircuitElement);
		} else {
			throw runtime_error(
					"Attempt to add child to BCircuit element over limit: "
							+ this->max_children);
		}

	}

	virtual ~BCircuitElement() {
	}

protected:
	vector<std::shared_ptr<BCircuitElement>> children;
	size_t max_children = 0;

};

class VarElement: public BCircuitElement {
public:
	VarElement(string val) :
			BCircuitElement() {
		this->val = val;
		this->max_children = 1;
	}

	int evaluate() {
		if (this->children.size() < 1) {
			throw runtime_error(
					"VarElement: " + this->val + " has no children");
		} else {
			cout << this->val << " ... ";
			return this->children[0]->evaluate();
		}
	}
	virtual ~VarElement() {
	}

private:
	string val;

};

class ConstElement: public BCircuitElement {
public:
	ConstElement(int val) :
			BCircuitElement() {
		this->val = val;
		this->max_children = 0;
	}

	int evaluate() {
		cout << this-> val << " ... ";
		return this->val;
	}
	virtual ~ConstElement() {
	}

private:
	int val;

};

class BinOpElement: public BCircuitElement {

public:
	BinOpElement(string val) :
			BCircuitElement() {
		this->val = val;
		this->max_children = 2;
	}
	int evaluate() {
		if (this->children.size() < 2) {
			throw runtime_error("BinOpeElement has too few children");
		} else {
			cout << this->val << " ... ";
			return (*binOpTable.find(this->val)).second(
					this->children[0]->evaluate(),
					this->children[1]->evaluate());
		}
	}

private:
	string val;
};

class UnOpElement: public BCircuitElement {
public:
	UnOpElement(string val) :
			BCircuitElement() {
		this->val = val;
		this->max_children = 1;
	}
	int evaluate() {
		if (this->children.size() < 1) {
			throw runtime_error("UnOpeElement has too few children");
		} else {
			cout << this->val << " ... ";
			return (*unOpTable.find(this->val)).second(
					this->children[0]->evaluate());
		}
	}

private:
	string val;
};

class Parser {
public:
	void parse_line(string line) {
		vector<smatch> sm { sregex_iterator { line.begin(), line.end(),
				dictionary }, sregex_iterator { } };
		cout << sm.size() << endl;
		vector<std::shared_ptr<BCircuitElement>> stack;
		bool needsRHS = false;
		bool assign = false;
		for (auto m : sm) {
			std::pair<int, string> token = get_token(m);
			cout << token.first << ": " << token.second << endl;
			switch (token.first) {
			case 1: {
				std::shared_ptr<BCircuitElement> temp { new ConstElement(
						stoi(token.second)) };
				if (needsRHS) {
					cout << "Adding ConstElement " << stoi(token.second) << " to RHS" << endl;
					stack.back()->add_child(temp);
					needsRHS = false;
				} else {
					cout << "Pushing ConstElement " << stoi(token.second) << "to stack bottom" << endl;
					stack.push_back(temp);
				}
			}
				break;
			case 2:
				if (this->varTable.count(token.second) == 0) {
					cout << "Adding VarElement " << token.second << " to varTable" << endl;
					this->varTable.emplace(
							std::pair<string, std::shared_ptr<BCircuitElement>> {
									token.second, std::shared_ptr<
											BCircuitElement> { new VarElement(
											token.second) } });
				}
				if (assign) {
					cout << "Assigning bottom of stack to " << token.second << endl;
					this->varTable[token.second]->add_child(stack.back());
					assign = false;
				} else if (needsRHS) {
					cout << "Pushing " << token.second << " to RHS of stack bottom" <<endl;
					stack.back()->add_child(this->varTable[token.second]);
					needsRHS = false;
				} else {
					cout << "Pushing " << token.second << " to stack bottom" << endl;
					stack.push_back(this->varTable[token.second]);
				}
				break;
			case 3: {
				needsRHS = true;
				std::shared_ptr<BCircuitElement> temp { new BinOpElement(
						token.second) };
				cout << "Assigning bottom of stack to LHS of " << token.second << " operator" <<endl;
				temp->add_child(stack.back());
				stack.pop_back();
				cout << "Pushing " << token.second << " to bottom of stack" << endl;
				stack.push_back(temp);
				break;
			}
			case 4:
				needsRHS = true;
				cout << "Pushing NOT to bottom of stack" << endl;
				stack.push_back(std::shared_ptr<BCircuitElement> {
						new UnOpElement(token.second) });
				break;
			case 5:
				assign = true;
				break;
			default:
				throw runtime_error("Oh, noes. Oh, noes.");
			}
		}
		return;
	}

	void print_keys() {
		for (auto x: this->varTable) {
			cout << x.first << ", ";
		}
		cout << endl;
	}


	void reset() {
		this->varTable.clear();
		return;
	}
	int get_value(string label) {
		if (this->varTable.count(label)) {
			return (*this->varTable.find(label)).second->evaluate();
		} else {
			throw runtime_error("label "+label+ " not found in parser.");
		}
	}
private:
	const regex dictionary {"([0-9]+)|([a-z]+)|(LSHIFT|RSHIFT|OR|AND)|(NOT)|(->)"};

	pair<int,string> get_token(smatch &sm) {
		for (int i = 1;i<6;i++) {
			if (sm.str(i).length()>0) {
				return {i, sm.str(i)};
			}
		}
		throw runtime_error("get_token failed");
	}
	map<string,std::shared_ptr<BCircuitElement>> varTable {};
};

void day_7(string filename) {
	ifstream file;
	file.open(filename);
	vector<string> s { };
	char line[100];
	while (file.getline(line, 100).good()) {
		s.push_back(string(line));
	}
	file.close();

	Parser p;
	for (auto line : s) {
		p.parse_line(line);
	}
	p.print_keys();
	cout << "a = " << p.get_value("a") << endl;

	return;
}
