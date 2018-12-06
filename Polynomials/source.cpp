#include<iostream>
#include<math.h>
#include<string>
#include<tuple>
#include<list>
#include<map>

using namespace std;

string readstring()
{
	string exp;
	cout << "Enter Expression: ";
	getline(cin, exp);
	return exp;
}

bool IsNumber(char num)
{
    if (num>='0' && num<='9')
        return true;
    return false;
}
bool IsPoint(char point)
{
	if (point == '.')
		return true;
	return false;
}
bool IsValidExpression(char C)
{
	if ( (C>='0' && C<='9') || (C=='.')||(C=='^') || (C=='x') )
		return true;
	return false;
}
char IsValidChar(char C)
{
	if ( (C>='0' && C<='9') || (C=='+')|| (C=='-') ||(C=='.')||(C=='^') || (C=='x') )
		return C;
	else
	{
		if(C == ' ')
			cout<<"Invalid Character found: <space>"<<endl;
		else
			cout<<"Invalid Character found: "<<C<<endl;
		return '?';
	}
}
string CleanString(string old_exp)
{
	string exp;
	/* remove whitespaces and invalid characters*/
	for(unsigned int i = 0; i < old_exp.size(); i++)
	{
		char a = IsValidChar(old_exp[i]);
		if(a != '?')
		exp += a;
	}
	return exp;
}

list<string> tokenize(string exp)
{
	string tmp_var;
	list<string> list_nomial;

	exp = CleanString(exp);
	bool sign = false;
	for(unsigned int i = 0; i < exp.size(); i++)
	{
		// check if first nomial is negative
		if( tmp_var.empty() and exp[i] == '-' )
			sign = true;
		if (exp[i] != '+' and exp[i] != '-')
		{
			if(IsValidExpression(exp[i]))
				tmp_var += exp[i];
		}
		else
		{

			if(tmp_var.empty())
				continue;
			if(sign)
				tmp_var = '-' + tmp_var;
			if (tmp_var[0] == 'x')
				tmp_var = '1'+tmp_var;
			list_nomial.push_back(tmp_var);
			tmp_var = string();

			//check for negative nomials
			if(exp[i] == '-')
				sign = true;
			else if(exp[i] == '+')
				sign = false;
		}
	}
	if (tmp_var[0] == 'x')
		tmp_var = '1'+tmp_var;
	if(sign)
		tmp_var = '-' + tmp_var;
	list_nomial.push_back(tmp_var);
/*
	for(auto v:list_nomial)
		cout<<v<<endl;
*/
	return list_nomial;
}

tuple<int,float>  tokenize_exp(string exp)
{
	string tmp_a;
	bool p;
	bool x;
	p = false;
	x = false;
	float a;
	for(unsigned int i = 0; i < exp.size(); i++)
	{
		if ( (exp[i] != 'x') )
			tmp_a += exp[i];
		else
		{
			//cout<<tmp_a<<' '<<exp[i]<<endl;
			x = true;
			// x^n, where n>1
			if( (i+1) < exp.size() )
			{
				if (exp[i+1] == '^')
					p = true;
				else
				{
					cout<<"Wrong format for expression. x should be followed by ^";
					exit(0);
				}
			}
			// x^1
			if( (p==false) and (x==true) )
				return make_tuple(1,stof(tmp_a));
			else
			{
				a = stof(tmp_a);
				tmp_a = string();
			}
		}
	}
	// x^0
	if(x == false)
		return make_tuple(0,stof(tmp_a));
	// x^n

	return make_tuple(stoi(tmp_a.erase(0,1)),a);
}

void display(map<int,float,greater <int>> poly,bool show=true)
{
	string exp;
	string h;
	int i = 0;
	for(auto v: poly)
	{
		if(v.second == 0)
		{
			i++;
			continue;
		}
		if(v.first > 1)
			h = to_string(v.second) + "x^" + to_string(v.first);
		else if(v.first == 1)
			h = to_string(v.second) + "x";
		else
			h = to_string(v.second);
		if(i==0)
			exp = h;
		else
			if (h[0] == '-')
				exp = exp +" " + h;
			else
				exp = exp + " + " + h;
		i++;
	}
	if(show)
		cout<<"f(x) = "<<exp<<endl<<endl;
	else
		cout<<exp;
}

map<int,float,greater <int>> readpoly(list<string> nomials)
{
	// create map, float is 'a', int is 'n'
	map<int,float,greater <int>> poly;
	string tmp_a;
	int n;
	float a;

	for(auto exp:nomials)
	{
		tie(n, a) = tokenize_exp(exp);
		if (poly.count(n))
		{
			cout<<"invalid expression. multiple entries for \'n\': "<<n;
			exit(0);
		}
		poly[n] = a;
	}
	display(poly);
	return poly;
}

map<int,float,greater <int>> addTerm(int n,float a,map<int,float,greater <int>> poly)
{
	if (poly.count(n))
	{
		cout<<"Entry for \'n\'="<<n<<" already exists! try option \'Edit Term\'"<<endl; ;
	}
	else
	{
		poly[n] = a;
		cout<<"Updated: ";
		display(poly);
	}
	return poly;
}
map<int,float,greater <int>> deleteTerm(int n,map<int,float,greater <int>> poly)
{
	if (poly.count(n))
	{
		poly.erase(n);
		cout<<"Updated: ";
		display(poly);
	}
	else
		cout<<"No Entry found for \'n\'="<<n<<endl;
	return poly;
}
map<int,float,greater <int>> editTerm(int n,float a,map<int,float,greater <int>> poly)
{
	if (poly.count(n))
	{
		poly[n] = a;
		cout<<"Updated: ";
		display(poly);
	}
	else
		cout<<"No Entry found for \'n\'="<<n<<" found. Try Add Term option"<<endl;
	return poly;
}
void evaluate(float a, map<int,float,greater <int>> poly)
{
	double result;
	result = 0.0;
	for(auto v: poly)
	{
		result += (v.second * pow(a,v.first));
	}
	cout<<"\nf("<<a<<") = "<<result<<endl<<endl;
}
map<int,float,greater <int>> addition(map<int,float,greater <int>> poly,map<int,float,greater <int>> poly2)
{
	map<int,float,greater <int>> new_poly;
	for(auto v1: poly)
	{
		for(auto v2: poly2)
		{
			if (v1.first == v2.first)
			{
				new_poly[v1.first] = v1.second + v2.second;
			}
		}
	}
	for(auto v1: poly)
	{
		if (not new_poly.count(v1.first))
		{
			new_poly[v1.first] = v1.second;
		}
	}
	for(auto v2: poly2)
	{
		if (not new_poly.count(v2.first))
		{
			new_poly[v2.first] = v2.second;
		}
	}
	return new_poly;
}
map<int,float,greater <int>> subtraction(map<int,float,greater <int>> poly,map<int,float,greater <int>> poly2)
{
	map<int,float,greater <int>> new_poly;
	for(auto v1: poly)
	{
		for(auto v2: poly2)
		{
			if (v1.first == v2.first)
			{
				new_poly[v1.first] = v1.second - v2.second;
			}
		}
	}
	for(auto v1: poly)
	{
		if (not new_poly.count(v1.first))
		{
			new_poly[v1.first] = v1.second;
		}
	}
	for(auto v2: poly2)
	{
		if (not new_poly.count(v2.first))
		{
			new_poly[v2.first] = -1*v2.second;
		}
	}
	return new_poly;
}

map<int,float,greater <int>> multiplication(map<int,float,greater <int>> poly,map<int,float,greater <int>> poly2)
{
	map<int,float,greater <int>> new_poly;
	int x;
	float y;
	for(auto v1: poly)
	{
		for(auto v2: poly2)
		{
			// add the powers
			x = v1.first + v2.first;
			y =  v1.second * v2.second;
			if (not new_poly.count(x))
				new_poly[x] = y;
			else
				new_poly[x] += y;
		}
	}
	return new_poly;
}
map<int,float,greater <int>> division(map<int,float,greater <int>> poly,map<int,float,greater <int>> poly2)
{

	map<int,float,greater <int>> new_poly;
	int x;
	float y;
	int max_pow = 0;

	max_pow = poly.begin()->first;
	for(int i=0;i<max_pow;i++)
	{
		if (not poly.count(i))
			poly[i] = 0;
	}

	map<int,float,greater <int>> total_result_eq;
	map<int,float,greater <int>> remainder_eq;
	remainder_eq = poly;
	for(int i=max_pow;i>0;i--)
	{
		x = remainder_eq.begin()->first - poly2.begin()->first;
		y = remainder_eq.begin()->second / poly2.begin()->second;
		map<int,float,greater <int>> tmp_eq;
		tmp_eq[x] = y;
		total_result_eq[x] = y;
		for(auto v1: remainder_eq)
		{
			for(auto v2: poly2)
			{
				int tmp_x;
				float tmp_y;
				//add powers
				tmp_x = v2.first+tmp_eq.begin()->first;
				tmp_y = v2.second*tmp_eq.begin()->second;

				if(v1.first == tmp_x)
					remainder_eq[v1.first] = v1.second - tmp_y;
			}
		}
		remainder_eq.erase(i);
	}
	cout<<"Remainder: "<<remainder_eq[0]<<endl;
	return total_result_eq;
}

map<int,float,greater <int>> algebra(map<int,float,greater <int>> poly)
{
	cout<<"Enter the new polynomial here: ";
	string exp2;
	//some error with getline, need to add twice
	getline(cin, exp2);
	getline(cin, exp2);
	cout<<"New Polynomial's ";
	list<string> nomials2 = tokenize(exp2);
	map<int,float,greater <int>> new_polynomial = readpoly(nomials2);

	int options;
	cout<<"---------------------------------"<<endl;
	cout<<"           MENU Algebra          "<<endl;
	cout<<"---------------------------------"<<endl;
	cout<<"Please choose: "<<endl;
	cout<<"1. Addition"<<endl;
	cout<<"2. Subtraction"<<endl;
	cout<<"3. Multiplication"<<endl;
	cout<<"4. Division"<<endl;
	cout<<"5. Main Menu"<<endl;
	cout<<"---------------------------------"<<endl;
	cout<<"Option: ";
	cin>>options;
	switch(options)
	{
		case 1:
			cout<<"\n=== You chose Addition ===\n";
			display(poly,false);
			cout<<endl<<'+'<<endl<<'(';
			display(new_polynomial,false);
			cout<<')'<<endl<<'='<<endl;
			new_polynomial = addition(poly,new_polynomial);
			display(new_polynomial,false);
			cout<<endl<<endl;
			break;
		case 2:
			cout<<"\n=== You chose Subtraction ===\n";
			display(poly,false);
			cout<<endl<<'-'<<endl<<'(';
			display(new_polynomial,false);
			cout<<')'<<endl<<'='<<endl;
			new_polynomial = subtraction(poly,new_polynomial);
			display(new_polynomial,false);
			cout<<endl<<endl;
			break;
		case 3:
			cout<<"\n=== You chose Multiplication ===\n";
			display(poly,false);
			cout<<endl<<'x'<<endl<<'(';
			display(new_polynomial,false);
			cout<<')'<<endl<<'='<<endl;
			new_polynomial = multiplication(poly,new_polynomial);
			display(new_polynomial,false);
			cout<<endl<<endl;
			break;
		case 4:
			cout<<"\n=== You chose Division ===\n";
			display(poly,false);
			cout<<endl<<'/'<<endl<<'(';
			display(new_polynomial,false);
			cout<<')'<<endl<<'='<<endl;
			new_polynomial = division(poly,new_polynomial);
			display(new_polynomial,false);
			cout<<endl<<endl;
			break;
		case 5:
			cout<<"\n=== You chose to return to Main Menu ===\n";
			display(poly);
			return poly;
		default:
			cout<<"Invalid Input";
			break;
	}
	return new_polynomial;
}

void Functions(map<int,float,greater <int>> polynomial)
{
	while(true)
	{
		unsigned int choice;
		cout<<"=================================="<<endl;
		cout<<"            MAIN MENU             "<<endl;
		cout<<"=================================="<<endl;
		cout<<"Please choose: "<<endl;
		cout<<"1. Add Term"<<endl;
		cout<<"2. Delete Term"<<endl;
		cout<<"3. Edit Term"<<endl;
		cout<<"4. Algebra with polynomials"<<endl;
		cout<<"5. Evaluate"<<endl;
		cout<<"6. Show Current Expression"<<endl;
		cout<<"7. Exit"<<endl;
		cout<<"=================================="<<endl;
		cout<<"Choice: ";
		cin>>choice;
		switch( choice )
		{
			int n;
			float a;
			case 1:
				cout<<"\n--- You chose Add Term ---\n";
				cout<<"Give an integer value for n: ";
				cin>>n;
				if (n < 0) {cout << "\'n\' cannot be negative\n";break;}
				if(!cin.good())
				{
					cout << "Error: \'n\' should be an integer\n";
					cin.clear();
					cin.ignore();
					break;
				}
				else
				{
					cout<<"Give a float value for \'a\': ";
					cin>>a;
					if (!cin.good())
					{
						cout << "Error: \'a\' should be a float\n";
						cin.clear();
						cin.ignore();
						break;
					}
					else
						polynomial = addTerm(n,a,polynomial);
				}
				break;
			case 2:
				cout<<"\n--- You chose Delete Term ---\n";
				cout<<"Give the integer value for \'n\': ";
				cin>>n;
				if (n < 0) {cout << "Error: \'n\' cannot be negative\n";break;}
				if(!cin.good())
				{
					cout << "Error: \'n\' should be an integer"<<endl;
					cin.clear();
					cin.ignore();
					break;
				}
				else
					polynomial = deleteTerm(n,polynomial);
				break;
			case 3:
				cout<<"\n--- You chose Edit Term ---\n";
				cout<<"Give an integer value for n: ";
				cin>>n;
				if (n < 0) {cout << "Error: \'n\' cannot be negative\n";break;}
				if(!cin.good())
				{
					cout << "Error: \'n\' should be an integer\n";
					cin.clear();
					cin.ignore();
					break;
				}
				else
				{
					cout<<"Give a float value for \'a\': ";
					cin>>a;
					if (!cin.good())
					{
						cout << "Error: \'a\' should be a float\n";
						cin.clear();
						cin.ignore();
						break;
					}
					else
						polynomial = editTerm(n,a,polynomial);
				}
				break;
			case 4:
				cout<<"\n--- You chose Algebra With Polynomials ---\n";
				polynomial = algebra(polynomial);
				break;
			case 5:
				cout<<"\n--- You chose Evaluate ---\n";
				cout<<"Give a float to evaluate with: ";
				cin>>a;
				if (!cin.good())
				{
					cout << "Error: \'x\' should be a float to evaluate\n";
					cin.clear();
					cin.ignore();
					break;
				}
				else
					evaluate(a,polynomial);
				break;
			case 6:
				cout<<"\n--- You Chose Current Expression ---\n";
				display(polynomial);
				break;
			case 7:
				cout<<"---> Exiting ...";
				exit(0);
			default:
				cout<<"Invalid Input\n";
				cin.clear();
				cin.ignore();
				break;
		}
	}
}
int main()
{
	string Intro ="* This code takes in a polynomial expression as input\n\
* The algorithm cleans the expressions automatically\n\
* Use \'x\' as the variable, ^ for power (spaces will be ignored automatically\n\
* Select an option for adding, editing or deleting any term with power \'n\'\
* Evaluate the polynomial with any float value\n\
* Add, subtract, multiply or divide algebraic expressions\n\
* Limit 1: Does not understand brackets for now. Brackets will be ignored\n\
* Limit 2: Does not know if the expression is properly divisible\n\
* Example expression: 2x^5-3x^2+4x^3+0.4x-1.89";
	cout<<Intro<<endl<<endl;
	string exp;
	exp = readstring();
	list<string> nomials = tokenize(exp);
	map<int,float,greater <int>> polynomial = readpoly(nomials);

	Functions(polynomial);
	return 0;
}
