#include <iostream> #include <fstream>
#include <string>
using namespace std;
int main() {
	string Str;
	getline(cin, Str);
	int Day = 0;
	int Month = 0;
	int Year = 0;
	int Curr_state = 1;
	int i = 0;
	while (true) {
		switch (Curr_state) {
		case 1: {
			if (i == Str.length()) {
				Curr_state = 5;
			}
			if (((int)Str[i] >= 48 && (int)Str[i] <= 57) && ((int)Str[i + 1] >= 48 && (int)Str[i + 1] <= 57)) {
				Curr_state = 2;
				Day = ((int)(Str[i]) - 48) * 10 + ((int)(Str[i + 1]) - 48);
				i = i + 2;
			}
			else
				i++;
			break;
		}
		case 2: {
			if (Day < 32 && (int)Str[i] == 47) {
				i++;
				if (((int)Str[i] >= 48 && (int)Str[i] <= 57) && ((int)Str[i + 1] >= 48 && (int)Str[i + 1] <= 57)) {
					Month = ((int)(Str[i]) - 48) * 10 + ((int)(Str[i + 1]) - 48);
					i = i + 2;
					int temp = 0;
					if (Month == 1 || Month == 3 || Month == 5 || Month == 7 || Month == 8 || Month == 10 || Month == 12)
						temp = 1;
					if (Month == 2)
						temp = 2;
					if (Month == 4 || Month == 6 || Month == 9 || Month == 11)
						temp = 3;
					switch (temp) {
					case 1: {
						if (Day < 32)
							Curr_state = 3;
						else
							Curr_state = 1;
						break;
					}
					case 2: {
						if (Day < 29)
							Curr_state = 3;
						else
							Curr_state = 1;
						break;
					}
					case 3: {
						if (Day < 31)
							Curr_state = 3;
						else
							Curr_state = 1;
						break;
					}
					default:
						Curr_state = 1;
						break;
					}
				}
				else
					Curr_state = 1;
			}
			else
				Curr_state = 1;
			break;
		}
		case 3: {
			if ((int)Str[i] == 47) {
				i++;
				if (((int)Str[i] >= 48 && (int)Str[i] <= 57) && ((int)Str[i + 1] >= 48 && (int)Str[i + 1] <= 57) && ((int)Str[i + 2] >= 48 && (int)Str[i + 2] <= 57) && ((int)Str[i + 3] >= 48 && (int)Str[i + 3] <= 57)) {
					Year = ((int)(Str[i]) - 48) * 1000 + ((int)(Str[i + 1]) - 48) * 100 + ((int)(Str[i + 2]) - 48) * 10 + ((int)(Str[i + 3]) - 48);
					i = i + 4;
					Curr_state = 4;
				}
				else
					Curr_state = 1;
			}
			else
				Curr_state = 1;
			break;
		}
		case 4: {
			cout << Day << "/" << Month << "/" << Year << endl;
			Day = 0;
			Month = 0;
			Year = 0;
			Curr_state = 1;
			break;
		}
		case 5:
			return 0;
		}
	}
}