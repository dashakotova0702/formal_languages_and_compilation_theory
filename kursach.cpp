#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <regex>
using namespace std;

class Rules {
public:
    string left_symb;
    string right_symb;
};

struct FirstFollow {
    string unterm;
    list <string> first;
    list <string> follow;
};

class Grammar {
public:
    string programm;
    string reserved[15] = { "int", "float", "bool", "ass", "if", "then", "else", "for", "to", "do", "while", "read", "write", "true", "false" };
    string separators[22] = { "\s", "{", "}", ";", ",", "&", "(", ")", ":", "/", "\n", "[", "]", "<", ">", "=", "+", "-", "*", "or", "and", "not" };
    list <string> names;
    list <int> id_table;
    list <int> id_element;
};

void scanner(Grammar* g) {
    ifstream file("code.txt");
    string code;
    while (getline(file, code))
        g->programm += code;
    file.close();
    cout << g->programm << endl;
    string str;
    smatch m;
    int count_names = -1;
    string sprtrs;
    for (int i = 0; i < 22; i++) {
        if (i < 19) {
            sprtrs += "\\";
            sprtrs += g->separators[i];
        }
        else {
            sprtrs += "\\s";
            sprtrs += g->separators[i];
            sprtrs += "\\s";
        }
        if (i != 21)
            sprtrs += "|";
    }
    /*cout << sprtrs << endl;*/
    const regex sep(sprtrs);
    while (regex_search(g->programm, m, sep)) {
        int start = m.position();
        str.clear();
        if (start != 0) {
            for (int j = 0; j < start; j++) {
                str += g->programm[j];
            }
            int count = 0;
            for (int j = 0; j < 15; j++) {
                if (str == g->reserved[j]) {
                    g->id_table.push_back(0);
                    g->id_element.push_back(j);
                    count++;
                }
            }
            if (count == 0) {
                count_names++;
                g->names.push_back(str);
                g->id_table.push_back(2);
                g->id_element.push_back(count_names);
            }
        }
        int id;
        for (int i = 0; i < 22; i++) {
            if (m.str() == g->separators[i]) {
                id = i;
            }
        }
        if (m.str() != " ") {
            g->id_table.push_back(1);
            g->id_element.push_back(id);
        }
        g->programm = m.suffix();
    }
    /*cout << "Number of table:   ";
    for (int n : g->id_table)
        cout << n << " ";
    cout << endl << "Number of element: ";
    for (int n : g->id_element)
        cout << n << " ";
    cout << endl;*/
}
void init_first(Rules* r, Grammar* g, FirstFollow* f, int count_r) {
    string temp;
    smatch m;
    string sprtrs;
    for (int i = 0; i < 22; i++) {
        if (i < 19) {
            sprtrs += "\\";
            sprtrs += g->separators[i];
        }
        else {
            sprtrs += "\\s";
            sprtrs += g->separators[i];
            sprtrs += "\\s";
        }
        if (i != 21)
            sprtrs += "|";
    }
    int c = 0;
    const regex sep(sprtrs);
    for (int i = 0; i < count_r; i++) {
        f[c].unterm = r[i].left_symb;
        if (regex_search(r[i].right_symb, m, sep)) {
            int start = m.position();
            if (start == 0) {
                f[c].first.push_back(m[0]);
                c++;
            }
            else {
                f[c].first.push_back(m.prefix());
                if (i + 1 != count_r) {
                    while (r[i + 1].left_symb == f[c].unterm) {
                        if (regex_search(r[i + 1].right_symb, m, sep)) {
                                if (f[c].first.front() != m.prefix())
                                    f[c].first.push_back(m.prefix());
                                i++;
                        }
                        else {
                            if (f[c].first.front() != r[i + 1].right_symb)
                                f[c].first.push_back(r[i + 1].right_symb);
                            i++;
                        }
                    }
                }
                c++;
            }
        }
        else {
            f[c].first.push_back(r[i].right_symb);
            if (i + 1 != count_r) {
                while (r[i + 1].left_symb == f[c].unterm) {
                    if (regex_search(r[i + 1].right_symb, m, sep)) {
                        int start = m.position();
                        if (start == 0) {
                            f[c].first.push_back(m[0]);
                        i++;
                    }
                        else {
                            if (f[c].first.front() != m.prefix())
                                f[c].first.push_back(m.prefix());
                            i++;
                        }
                    }
                    else {
                        if (f[c].first.front() != r[i + 1].right_symb) {
                            if (((int(r[i + 1].right_symb[1]) < 48) || (int(r[i + 1].right_symb[1]) > 57)) && i + 1 != 40 && i + 1 != 42) {
                                string a = "";
                                a += r[i + 1].right_symb[0];
                                f[c].first.push_back(a);
                            }
                            else
                                f[c].first.push_back(r[i + 1].right_symb);
                        }
                        i++;
                    }
                }
            }
            c++;
        }
    }
}
void first_array(FirstFollow *f, int count_r) {
    /*cout << endl;*/
    int count = 0, c = 0;
    for (int i = 0; i < count_r; i++) {
        if (f[i].unterm != "") {
            count++;
        }
    }
    for (int i = count - 1; i >= 0; i--) {
        for (int j = 0; j < count; j++) {
            while (f[i].first.front() == f[j].unterm) {
                    if (f[i].first.front() == f[i].unterm) {
                        f[i].first.pop_front();
                        j = -1;
                    }
                    else {
                            int k = 0;
                            f[i].first.pop_front();
                            while (k < f[j].first.size()) {
                                f[i].first.push_back(f[j].first.front());
                                f[j].first.push_back(f[j].first.front());
                                f[j].first.pop_front();
                                k++;
                            }
                            j = -1;
                            c++;
                    }
            }
        }
        f[i].first.sort();
    }
    for (int i = 0; i < count; i++)
        f[i].first.unique();
    
}
void follow_array(FirstFollow* f, Rules* r, int count_r) {
    cout << endl;
    smatch m1, m2, m3;
    int count = 0, c = 0;
    for (int i = 0; i < count_r; i++) {
        if (f[i].unterm != "") {
            count++;
        }
    }
    f[0].follow.push_back("&");
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < count_r; j++) {
            const regex unterm(f[i].unterm);
            if (regex_search(r[j].right_symb, m1, unterm)) {
                string suffix = m1.suffix();
                if (suffix[0] == ' ') {
                    if (suffix[1] == '[')
                        f[i].follow.push_back("[");
                    else {
                        regex follow_elem("\\s");
                        regex_search(suffix, m2, follow_elem);
                        string str = m2.suffix();
                        if (regex_search(str, m3, follow_elem)) {
                            f[i].follow.push_back(m3.prefix());
                        }
                        else {
                            f[i].follow.push_back(str);
                        }
                    }
                }
                else if (suffix[0] == '1')
                    continue;
                else if (m1.position() + m1.length() == r[j].right_symb.length())
                    continue;
                else {
                    string s = "";
                    s += r[j].right_symb[m1.position() + m1.length()];
                    int x = 0;
                    for (int k = 0; k < count; k++) {
                        if (s == f[k].unterm) {
                            for (int n = 0; n < f[k].first.size(); n++) {
                                f[i].follow.push_back(f[k].first.front());
                                f[k].first.push_back(f[k].first.front());
                                f[k].first.pop_front();
                            }
                            x++;
                        }
                    }
                    if (x == 0)
                        f[i].follow.push_back(s);
                }
            }
        }
    }
    f[5].follow.push_back("]");
    for (int i = 0; i < count; i++) {
        f[i].follow.sort();
        f[i].follow.unique();
    }
    int l = 0;
    while (l < 2) {
        for (int i = 0; i < count; i++) {
            for (int j = 0; j < count_r; j++) {
                const regex unterm(f[i].unterm);
                    if (regex_search(r[j].right_symb, m1, unterm)) {
                        string suffix = m1.suffix();
                            if (suffix[0] != '1' && f[i].unterm != r[j].left_symb) {
                                for (int k = 0; k < count; k++) {
                                    if (r[j].left_symb == f[k].unterm) {
                                        for (int n = 0; n < f[k].follow.size(); n++) {
                                            f[i].follow.push_back(f[k].follow.front());
                                            f[k].follow.push_back(f[k].follow.front());
                                            f[k].follow.pop_front();
                                        }
                                    }
                                }
                            }
                    }
            }
        }
        l++;
        for (int i = 0; i < count; i++) {
            f[i].follow.sort();
            f[i].follow.unique();
        }
    }
    /*cout << endl;
    for (int i = 0; i < count; i++) {
        cout << endl << "Unterm: " << f[i].unterm << " Follow: ";
        for (string n : f[i].follow)
            cout << n << " ";
    }*/
}
int check_stack_and_buffer(list <string>* s, list <pair <int, int>>* id, FirstFollow* f, Grammar* g, Rules* r, int count_r);
int stack_replacement(list <string>* s, list <pair <int, int>>* id, FirstFollow* f, Grammar* g, Rules* r, int id_rule, int count_r) {
    list <string> stck;
    list <pair <int, int>> buffer;
    for (int j = 0; j < s->size(); j++) {
        stck.push_back(s->front());
        s->push_back(s->front());
        s->pop_front();
    }
    for (int j = 0; j < id->size(); j++) {
        buffer.push_back(make_pair(id->front().first, id->front().second));
        id->push_back(make_pair(id->front().first, id->front().second));
        id->pop_front();
    }
    string buf_elem;
    list <string> lexems;
    if (buffer.front().first == 0)
        buf_elem = g->reserved[buffer.front().second];
    if (buffer.front().first == 1)
        buf_elem = g->separators[buffer.front().second];
    if (buffer.front().first == 2)
        buf_elem = "id";
    int id_first;
    for (int i = 0; i < 12; i++) {
        if (f[i].unterm == r[id_rule].left_symb) {
            id_first = i;
        }
    }
    smatch m;
    string sprtrs;
    for (int i = 0; i < 22; i++) {
        if (i < 19) {
            sprtrs += "\\";
            sprtrs += g->separators[i];
        }
        else {
            sprtrs += "\\s";
            sprtrs += g->separators[i];
            sprtrs += "\\s";
        }
        if (i != 21)
            sprtrs += "|";
    }
    int count_of_lexems = 0;
    const regex sep(sprtrs);
    string temp = r[id_rule].right_symb;
    while (!temp.empty()) {
        if (regex_search(temp, m, sep)) {
            int start = m.position();
            if (start != 0) {
                lexems.push_back(m.prefix());
                if (m.str() != " ")
                    lexems.push_back(m.str());
            }
            else {
                if (m.str() != " ")
                    lexems.push_back(m.str());
            }
            temp = m.suffix();
        }
        else {
            lexems.push_back(temp);
            break;
        }
    }
    int count = 0;
    for (string n : f[id_first].first) {
        if (n == buf_elem) {
            count = 1;
            break;
        }
    }
    for (string n : f[id_first].follow) {
        if (n == buf_elem) {
            count = 2;
            break;
        }
    }
    if (count == 1) {
        stck.pop_front();
        while (!lexems.empty()) {
            stck.push_front(lexems.back());
            lexems.pop_back();
        }
        return check_stack_and_buffer(&stck, &buffer, f, g, r, count_r);
    }
    if (count == 2) {
        stck.pop_front();
        return check_stack_and_buffer(&stck, &buffer, f, g, r, count_r);
    }
    if (count == 0)
        return 1;
    if (buf_elem == "&" && stck.empty())
        return 0;
}
int check_stack_and_buffer(list <string>* s, list <pair <int, int>>* id, FirstFollow* f, Grammar* g, Rules* r, int count_r) {
    list <string> stck;
    list <pair <int, int>> buffer;
    for (int j = 0; j < s->size(); j++) {
        stck.push_back(s->front());
        s->push_back(s->front());
        s->pop_front();
    }
    for (int j = 0; j < id->size(); j++) {
        buffer.push_back(make_pair(id->front().first, id->front().second));
        id->push_back(make_pair(id->front().first, id->front().second));
        id->pop_front();
    }
    string buf_elem;
    if (buffer.front().first == 0)
        buf_elem = g->reserved[buffer.front().second];
    if (buffer.front().first == 1)
        buf_elem = g->separators[buffer.front().second];
    if (buffer.front().first == 2)
        buf_elem = "id";
    while (stck.front() == buf_elem) {
        stck.pop_front();
        buffer.pop_front();
        if (!buffer.empty()) {
            if (buffer.front().first == 0)
                buf_elem = g->reserved[buffer.front().second];
            if (buffer.front().first == 1)
                buf_elem = g->separators[buffer.front().second];
            if (buffer.front().first == 2)
                buf_elem = "id";
        }
        if (stck.empty())
            break;
    }
    if (buffer.empty() && stck.empty()) {
        return 0;
    }
    if (buffer.empty() && !stck.empty())
        buffer.push_back(make_pair(1, 5));
    int count_symb = 1, number_of_rule = -1;
    for (int k = 0; k < count_r; k++) {
        if (stck.front() == r[k].left_symb) {
            number_of_rule = k;
            while (r[k + 1].left_symb == stck.front()) {
                count_symb++;
                k++;
            }
        }
    }
    if (number_of_rule == -1)
        return 2;
    for (int j = number_of_rule; j < (count_symb + number_of_rule); j++) {
        int a = stack_replacement(&stck, &buffer, f, g, r, j, count_r);
        if (j == 1 && a != 0)
            j++;
        if (j == 8 && a != 0)
            return a;
        if (a == 0) {
            return a;
        }
    }
    return 1;
}
void table(FirstFollow* f, Grammar* g, Rules* r, int count_r) {
    list <string> stck;
    list <pair <int, int>> id;
    for (int j = 0; j < g->id_table.size(); j++) {
        id.push_back(make_pair(g->id_table.front(), g->id_element.front()));
        g->id_table.push_back(g->id_table.front());
        g->id_element.push_back(g->id_element.front());
        g->id_table.pop_front();
        g->id_element.pop_front();
    }
    stck.push_back("P");
    int code = check_stack_and_buffer(&stck, &id, f, g, r, count_r);
    if (code == 0) {
        cout << endl << "Errors not detected" << endl;
    }
    else {
        cout << endl << "Errors detected. Error code " << code << endl;
    }
}
void parser(Rules* r, FirstFollow* f, Grammar* g) {
    string rule;
    fstream fout;
    fout.open("rules.txt");
    smatch m;
    int count = 0;
    while (getline(fout, rule)) {
        count++;
    }
    fout.clear();
    fout.seekg(0);
    r = new Rules[count];
    f = new FirstFollow[count];
    for (int i = 0; i < count; i++) {
        getline(fout, rule);
        const regex r1("\\->");
        regex_search(rule, m, r1);
        r[i].left_symb = m.prefix();
        r[i].right_symb = m.suffix();
    }
    init_first(r, g, f, count);
    first_array(f, count);
    first_array(f, count);
    /*for (int i = 0; i < count; i++) {
        if (f[i].unterm != "") {
            cout << endl << "Unterm: " << f[i].unterm << " First: ";
            for (string n : f[i].first)
                cout << n << " ";
        }
    }*/
    follow_array(f, r, count);
    table(f, g, r, count);
}

int main(int argc, char const* argv[]) {
    Rules r;
    Grammar g;
    FirstFollow* f = new FirstFollow;
    scanner(&g);
    parser(&r, f, &g);
    return 0;
}
