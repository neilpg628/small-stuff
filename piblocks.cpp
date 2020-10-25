// This script simulates the motions of two frictionless blocks hitting and bouncing off a wall
// to calculate the digits of PI. For the description of the method, see https://arxiv.org/pdf/1712.06698.pdf

#include <iostream>
#include <cmath>
using namespace std;

int main(){

	double m1, m2;
	double v1 = 0;//v of small block
	double v2 = -1;//v of big block
	double u1 = 0;
	int d;
	int n = 0;

	cout << "Enter the number of digits to be computed:\n";
	cin >> d;

	m1 = 1;
	m2 = pow(100.0,d-1.0)*m1;

	while(!v1>=0.0 || !v2 >= v1) {
		u1 = v1;
		v1 = (m1-m2)*v1/(m1+m2) + 2*m2*v2/(m1+m2);
		v2 = (m2-m1)*v2/(m1+m2) + 2*m1*u1/(m1+m2);
		n++;
		cout << v1 << "," << v2 << endl;
		if(v1>=0.0 && v2 >= v1) break;
		v1 = -v1;
		n++;
		//cout << v1 << "," << v2 << endl;
	}

	cout << "The first " << d << " digits of PI are: " << n << endl;

	return 0;
}
