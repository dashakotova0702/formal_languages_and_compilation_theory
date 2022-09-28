#include <iostream> #include <cstdio>
using namespace std;
class Symbols {
public:
	char* Term;
	char* Un_Term;
	int Term_Q;
	int Un_Term_Q;
	char Epsilon;
	Symbols() {
		Term = {};
		Un_Term = {};
		Term_Q = 0;
		Un_Term_Q = 0;
	}
	void init_term();
	void init_un_term();
	void init_epsilon();
	int init_new_un_term(int count_of_new_unterminal_symbols);
};
void Symbols::init_term() {
	cout << "Count of terminal symbols: " << endl;
	cin >> Term_Q;
	Term = new char[Term_Q];
	cout << "Terminal symbols: " << endl;
	for (int i = 0; i < Term_Q; i++) {
		cout << i << ". ";
		cin >> Term[i];
	}
}
void Symbols::init_un_term() {
	cout << "Count of not terminal symbols: " << endl;
	cin >> Un_Term_Q;
	Un_Term = new char[Un_Term_Q];
	cout << "Not terminal symbols: " << endl;
	for (int i = 0; i < Un_Term_Q; i++) {
		cout << i << ". ";
		cin >> Un_Term[i];
	}
}
void Symbols::init_epsilon() {
	cout << "Epsilon symbol: " << endl;
	cin >> Epsilon;
}
int Symbols::init_new_un_term(int count_of_new_unterminal_symbols) {
	for (int i = 0; i < Un_Term_Q; i++) {
		if (Un_Term[i] == '@') {
			int binary_code = (int)(Un_Term[i - 1]);
			binary_code++;
			Un_Term[i] = (char)(binary_code);
		}
	}
	int binary_code = (int)(Un_Term[Un_Term_Q - 1]);
	int Un_Term_Q_new = Un_Term_Q + count_of_new_unterminal_symbols;
	char symb[Un_Term_Q_new];
	for (int i = 0; i < Un_Term_Q; i++) {
		symb[i] = Un_Term[i];
	}
	int i = Un_Term_Q;
	for (; i < Un_Term_Q_new; i++) {
		binary_code++;
		symb[i] = (char)binary_code;
	}
	int Old_Un_Term_Q = Un_Term_Q;
	Un_Term_Q = Un_Term_Q_new;
	Un_Term = new char[Un_Term_Q];
	for (int i = 0; i < Un_Term_Q; i++) {
		Un_Term[i] = symb[i];
	}
	return Old_Un_Term_Q;
}
class Rules {
public:
	char left_symb;
	char* right_symb;
	int right_symb_Q;
	Rules() {
		left_symb = '0';
		right_symb_Q = 1;
	}
	void init_left_symb(Symbols& symb);
	void init_right_symb(Symbols& symb);
	bool Check_left(Symbols& symb);
	bool Check_right(Symbols& symb);
	friend ostream& operator<<(ostream&, Rules);
};
void Rules::init_left_symb(Symbols& symb) {
	do {
		cout << "Left symbol: " << endl;
		cin >> left_symb;
	} while (!Check_left(symb));
}
bool Rules::Check_left(Symbols& symb) {
	for (int i = 0; i < symb.Un_Term_Q; i++) {
		if (symb.Un_Term[i] == left_symb) {
			return true;
		}
	}
	return false;
}
void Rules::init_right_symb(Symbols& symb) {
	do {
		cout << "Count Right symbols: " << endl;
		cin >> right_symb_Q;
		right_symb = new char[right_symb_Q];
		for (int i = 0; i < right_symb_Q; i++) {
			cout << i << ". ";
			cin >> right_symb[i];
		}
	} while (!Check_right(symb));
}
bool Rules::Check_right(Symbols& symb) {
	int i = 0;
	int w = 0;
	while (i < right_symb_Q) {
		w = i;
		for (int j = 0; j < symb.Un_Term_Q; j++) {
			if (symb.Un_Term[j] == right_symb[i] || right_symb[i] == symb.Epsilon) {
				i++;
				break;
			}
			else {
				continue;
			}
		}
		for (int k = 0; k < symb.Term_Q; k++) {
			if (symb.Term[k] == right_symb[i] || right_symb[i] == symb.Epsilon) {
				i++;
				break;
			}
			else {
				continue;
			}
		}
		if (w == i) {
			cout << "Symbol " << right_symb[i] << "does not belong to the alphabet " << endl;
			return false;
		}
	}
	return true;
}
ostream& operator<<(ostream& stream, Rules rul) {
	stream << rul.left_symb << "-> ";
	for (int i = 0; i < rul.right_symb_Q; i++) {
		stream << rul.right_symb[i];
	}
	stream << endl;
	return stream;
}
class Transformations {
public:
	void Unattainable_States(Symbols& symb, int count_of_rules, Rules* rul);
	void Unsolvable_States(Symbols& symb, int count_of_rules, Rules* rul);
	int Check_Unsolvable_States(Symbols& symb, int count_of_rules, Rules* rul);
	void Check_Recursion(Symbols& symb, int count_of_rules, Rules* rul);
	int Count_of_New_Unterminal_Symbols(Symbols& symb, int count_of_rules, Rules* rul);
	void Mixed_Chains(Symbols& symb, int count_of_rules, Rules* rul, int count_of_new_rules, Rules* new_rules, int old_count);
	bool Check_Long_Rules(Symbols& symb, int count_of_rules, Rules* rul);
	bool Check_Epsilon_Symbol(Symbols& symb, int count_of_rules, Rules* rul);
	bool Check_of_Start_Symbol(Symbols& symb, int count_of_rules, Rules* rul);
	void Start_Symbol_Replacement(Symbols& symb, int count_of_rules, Rules* rul, int count_of_homsky_rules, Rules* h_rul);
};
void Transformations::Unattainable_States(Symbols& symb, int count_of_rules, Rules* rul) {
	int count_of_attainability[count_of_rules] = { 0 };
	for (int i = 0; i < count_of_rules; i++) {
		for (int j = 0; j < count_of_rules; j++) {
			for (int k = 0; k < rul[j].right_symb_Q; k++) {
				if (j == i) {
				}
				else {
					if ((rul[i].left_symb == rul[j].right_symb[k]) || (rul[i].left_symb == rul[0].left_symb)) {
						count_of_attainability[i]++;
					}
				}
			}
		}
	}
	for (int i = 0; i < count_of_rules; i++) {
		if (count_of_attainability[i] == 0) {
			rul[i].left_symb = '@';
		}
	}
	for (int i = 0; i < count_of_rules; i++) {
		if (rul[i].left_symb == '@') {
			for (int j = 0; j < rul[i].right_symb_Q; j++) {
				for (int k = 0; k < symb.Un_Term_Q; k++) {
					if (rul[i].right_symb[j] == symb.Un_Term[k]) {
						for (int n = 0; n < count_of_rules; n++) {
							if (symb.Un_Term[k] == rul[n].left_symb) {
								rul[n].left_symb = '@';
							}
						}
					}
				}
			}
		}
	}
	cout << "Rules without unattainable states:" << endl << endl;
	for (int i = 0; i < count_of_rules; i++) {
		if (rul[i].left_symb != '@') {
			cout << rul[i].left_symb << "-> ";
			for (int j = 0; j < rul[i].right_symb_Q; j++) {
				cout << rul[i].right_symb[j];
			}
			cout << endl;
		}
	}
}
void Transformations::Unsolvable_States(Symbols& symb, int count_of_rules, Rules* rul) {
	int unsolvable_state = 0;
	do {
		Check_Recursion(symb, count_of_rules, rul);
		unsolvable_state = Check_Unsolvable_States(symb, count_of_rules, rul);
		rul[unsolvable_state].left_symb = '@';
	} while (unsolvable_state >= 0);
	cout << "Rules without unsolvable states:" << endl << endl;
	for (int i = 0; i < count_of_rules; i++) {
		if (rul[i].left_symb != '@') {
			cout << rul[i].left_symb << "-> ";
			for (int j = 0; j < rul[i].right_symb_Q; j++) {
				cout << rul[i].right_symb[j];
			}
			cout << endl;
		}
	}
}
int Transformations::Check_Unsolvable_States(Symbols& symb, int count_of_rules, Rules* rul) {
	int availability_of_rule[count_of_rules] = { 0 };
	for (int i = 0; i < count_of_rules; i++) {
		if (rul[i].left_symb != '@') {
				for (int j = 0; j < rul[i].right_symb_Q; j++) {
					for (int k = 0; k < symb.Un_Term_Q; k++) {
						if (rul[i].right_symb[j] == symb.Un_Term[k]) {
							for (int n = 0; n < count_of_rules; n++) {
								if (rul[i].right_symb[j] == rul[n].left_symb) {
									availability_of_rule[i]++;
								}
							}
						}
						if (rul[i].right_symb[j] == symb.Epsilon) {
							availability_of_rule[i]++;
						}
					}
				}
		}
	}
	int count_of_terminal_symbols = 0;
	for (int i = 0; i < count_of_rules; i++) {
		if (rul[i].left_symb != '@') {
			for (int j = 0; j < rul[i].right_symb_Q; j++) {
				for (int k = 0; k < symb.Term_Q; k++) {
					if (rul[i].right_symb[j] == symb.Term[k]) {
						count_of_terminal_symbols++;
					}
				}
			}
			if (count_of_terminal_symbols == rul[i].right_symb_Q) {
				availability_of_rule[i] = 1;
			}
			count_of_terminal_symbols = 0;
		}
	}
	for (int i = 0; i < count_of_rules; i++) {
		if (availability_of_rule[i] == 0) {
			if (rul[i].left_symb != '@') {
				return i;
			}
		}
	}
	return -1;
}
void Transformations::Check_Recursion(Symbols& symb, int count_of_rules, Rules* rul) {
	int count_of_eponymous_rules = 0;
	for (int i = 0; i < count_of_rules; i++) {
		if (rul[i].left_symb != '@') {
			for (int j = 0; j < rul[i].right_symb_Q; j++) {
				if (rul[i].left_symb == rul[i].right_symb[j]) {
					for (int k = 0; k < count_of_rules; k++) {
						if (rul[i].left_symb == rul[k].left_symb && i != k) {
							count_of_eponymous_rules++;
						}
					}
					if (count_of_eponymous_rules == 0) {
						rul[i].left_symb = '@';
					}
					count_of_eponymous_rules = 0;
				}
			}
		}
	}
}
int Transformations::Count_of_New_Unterminal_Symbols(Symbols& symb, int count_of_rules, Rules* rul) {
	int count_of_new_unterminal_symbols = 0;
	for (int i = 0; i < count_of_rules; i++) {
		if (rul[i].left_symb != '@') {
			if (rul[i].right_symb_Q > 1) {
				for (int j = 0; j < rul[i].right_symb_Q; j++) {
					for (int k = 0; k < symb.Term_Q; k++) {
						if (rul[i].right_symb[j] == symb.Term[k]) {
							count_of_new_unterminal_symbols++;
						}
					}
				}
			}
		}
	}
	return count_of_new_unterminal_symbols;
}
void Transformations::Mixed_Chains(Symbols& symb, int count_of_rules, Rules* rul, int count_of_new_rules, Rules* new_rul, int old_count) {
	bool x = false;
	for (int i = 0; i < count_of_rules; i++) {
		for (int j = 0; j < count_of_new_rules; j++) {
			while (rul[i].left_symb == '@') {
				i++;
			}
			if (i >= count_of_rules) {
				j = count_of_new_rules;
			}
			if (j != count_of_new_rules) {
				new_rul[j].left_symb = rul[i].left_symb;
				new_rul[j].right_symb_Q = rul[i].right_symb_Q;
				new_rul[j].right_symb = new char[new_rul[j].right_symb_Q];
				for (int k = 0; k < rul[i].right_symb_Q; k++) {
					for (int n = 0; n < symb.Term_Q; n++) {
						if (rul[i].right_symb_Q > 1) {
							if (rul[i].right_symb[k] == symb.Term[n]) {
								x = true;
							}
						}
					}
					if (x) {
						new_rul[j].right_symb[k] = symb.Un_Term[old_count];
						new_rul[j + 1].left_symb = symb.Un_Term[old_count];
						old_count++;
						new_rul[j + 1].right_symb = new char[1];
						new_rul[j + 1].right_symb[0] = rul[i].right_symb[k];
						new_rul[j + 1].right_symb_Q = 1;
						x = false;
					}
					else {
						new_rul[j].right_symb[k] = rul[i].right_symb[k];
					}
					if ((new_rul[j].right_symb_Q - 1 == k) && (rul[i].right_symb_Q > 1)) {
						j++;
					}
				}
				i++;
			}
		}
	}
	cout << "Rules without mixed chains: " << endl << endl;
	for (int i = 0; i < count_of_new_rules; i++) {
		if (new_rul[i].left_symb != '0' && new_rul[i].left_symb != '@') {
			cout << new_rul[i].left_symb << "-> ";
			for (int j = 0; j < new_rul[i].right_symb_Q; j++) {
				cout << new_rul[i].right_symb[j];
			}
			cout << endl;
		}
	}
}
bool Transformations::Check_Long_Rules(Symbols& symb, int count_of_rules, Rules* rul) {
	int count_of_long_rules = 0;
	for (int i = 0; i < count_of_rules; i++) {
		if (rul[i].right_symb_Q > 2) {
			count_of_long_rules++;
		}
	}
	if (count_of_long_rules > 0) {
		return true;
	}
	return false;
}
bool Transformations::Check_Epsilon_Symbol(Symbols& symb, int count_of_rules, Rules* rul) {
	int count_of_epsilon_rules = 0;
	for (int i = 0; i < count_of_rules; i++) {
		if (rul[i].left_symb != '@' && rul[i].left_symb != '0') {
			for (int j = 0; j < rul[i].right_symb_Q; j++) {
				if ((rul[i].right_symb[j] == symb.Epsilon) && (rul[i].left_symb != symb.Un_Term[0])) {
					count_of_epsilon_rules++;
				}
			}
		}
	}
	if (count_of_epsilon_rules > 0) {
		return true;
	}
	return false;
}
bool Transformations::Check_of_Start_Symbol(Symbols& symb, int count_of_rules, Rules* rul) {
	for (int i = 0; i < count_of_rules; i++) {
		if (rul[i].left_symb != '@' && rul[i].left_symb != '0') {
			for (int j = 0; j < rul[i].right_symb_Q; j++) {
				if (symb.Term[0] == rul[i].right_symb[j]) {
					return true;
				}
			}
		}
	}
	return false;
}
void Transformations::Start_Symbol_Replacement(Symbols& symb, int count_of_rules, Rules* rul, int count_of_homsky_rules, Rules* h_rul) {
	h_rul[0].left_symb = symb.Un_Term[symb.Un_Term_Q - 1];
	h_rul[0].right_symb_Q = 1;
	h_rul[0].right_symb = new char[h_rul[0].right_symb_Q];
	h_rul[0].right_symb[0] = symb.Un_Term[0];
	for (int i = 0; i < count_of_rules; i++) {
		for (int j = 1; j < count_of_homsky_rules; j++) {
			while (rul[i].left_symb == '@' || rul[i].left_symb == '0') {
				i++;
			}
			if (i >= count_of_rules) {
				j = count_of_homsky_rules;
			}
			if (j != count_of_homsky_rules) {
				if (rul[i].right_symb[0] == symb.Epsilon) {
					rul[i].left_symb = symb.Un_Term[symb.Un_Term_Q - 1];
				}
				h_rul[j].left_symb = rul[i].left_symb;
				h_rul[j].right_symb_Q = rul[i].right_symb_Q;
				h_rul[j].right_symb = new char[h_rul[j].right_symb_Q];
				for (int k = 0; k < h_rul[j].right_symb_Q; k++) {
					h_rul[j].right_symb[k] = rul[i].right_symb[k];
					if ((h_rul[j].right_symb_Q - 1 == k) && (rul[i].right_symb_Q > 1)) {
						j++;
					}
				}
				i++;
			}
		}
	}
	cout << "Homsky grammar: " << endl << endl;
	for (int i = 0; i < count_of_homsky_rules; i++) {
		if (h_rul[i].left_symb != '0' && h_rul[i].left_symb != '@' && (int)(h_rul[i].left_symb) != 0) {
			cout << h_rul[i].left_symb << "-> ";
			for (int j = 0; j < h_rul[i].right_symb_Q; j++) {
				cout << h_rul[i].right_symb[j];
			}
			cout << endl;
		}
	}
}
int main() {
	int Rules_Q = 0;
	Symbols symb;
	symb.init_term();
	symb.init_un_term();
	symb.init_epsilon();
	cout << "Count quantity rules" << endl;
	cin >> Rules_Q;
	Rules* rul = new Rules[Rules_Q];
	for (int i = 0; i < Rules_Q; i++) {
		rul[i].init_left_symb(symb);
		rul[i].init_right_symb(symb);
	}
	cout << "------------" << endl;
	for (int i = 0; i < Rules_Q; i++) {
		cout << rul[i];
	}
	Transformations transform;
	cout << "------------" << endl;
	transform.Unattainable_States(symb, Rules_Q, rul);
	cout << "------------" << endl;
		transform.Unsolvable_States(symb, Rules_Q, rul);
	cout << "------------" << endl;
	int count_of_new_unterminal_symbols = transform.Count_of_New_Unterminal_Symbols(symb, Rules_Q, rul);
	int count_of_old_unterminal_symbols = symb.init_new_un_term(count_of_new_unterminal_symbols);
	int count_of_new_rules = Rules_Q + count_of_new_unterminal_symbols;
	Rules* new_rules = new Rules[count_of_new_rules];
	transform.Mixed_Chains(symb, Rules_Q, rul, count_of_new_rules, new_rules, count_of_old_unterminal_symbols);
	delete[] rul;
	cout << "------------" << endl;
	if (!transform.Check_Long_Rules(symb, count_of_new_rules, new_rules)) {
		cout << "Grammar doesn't contain long rules. Elimination of long rules is pointless." << endl;
	}
	cout << "------------" << endl;
	if (!transform.Check_Epsilon_Symbol(symb, count_of_new_rules, new_rules)) {
		cout << "Epsilon symbol contained only in start symbol. Elimination of epsilon rules is pointless." << endl;
	}
	cout << "------------" << endl;
	if (transform.Check_of_Start_Symbol(symb, Rules_Q, new_rules)) {
		count_of_old_unterminal_symbols = symb.init_new_un_term(1);
		int count_of_rules_of_homsky_grammar = count_of_new_rules + 1;
		Rules* homsky_grammar = new Rules[count_of_rules_of_homsky_grammar];
		transform.Start_Symbol_Replacement(symb, count_of_new_rules, new_rules, count_of_rules_of_homsky_grammar, homsky_grammar);
		delete[] new_rules;
		delete[] homsky_grammar;
	}
	return 0;
}