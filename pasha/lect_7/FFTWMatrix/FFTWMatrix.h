#include <iostream>#include <complex>class FFTWMatrix {	int w,h;	complex<double> *p;public:	FFTWMatrix(int,int);	~FFTWMatrix();	FFTWMatrix operator= (const FFTWMatrix&);	complex<double>& at(int,int);	FFTWMatrix load_matrix(char);	FFTWMatrix save_matrix(char);};