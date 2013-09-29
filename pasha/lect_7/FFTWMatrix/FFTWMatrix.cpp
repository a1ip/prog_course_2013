#include "FFTWMatrix.h"#unclude <complex>using namespace std;FFTWMatrix::FFTWMatrix(int ww, int hh): w(ww), h(hh), p(new complex<double>[w*h]) {}~FFTWMatrix() {delete []p;}FFTWMatrix FFTWMatrix::operator= (const FFTWMatrix& r) {	OurArray tmp(r);	tmp.swap(*this);	return *this;}complex<double>& FFTWMatrix::at(int nw, int nh) {	return p[w*nw+nh)]}1 2 3 41 2 3 41 2 3 41 2 3 4