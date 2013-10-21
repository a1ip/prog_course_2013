#include "matrix.h"
#include "util.h"
#include "Tensor4.h"
#include "vec3.h"
#include "polynom.h"
#include "solpart.h"
#include <vector>
using namespace std;
Tensor4 makeTetragonalTensor(double c11, double c12, double c13, double c33, double c44, double c66) {
    Tensor4 new_tens;

    double tens2[36];
    for(int t = 0; t < 36; t++) {
      tens2[t] = 0;
    }

    tens2[0] = c11;  tens2[1] = c12;  tens2[2] = c13;
    tens2[6] = c12;  tens2[7] = c11;  tens2[8] = c13;
    tens2[12] = c13; tens2[13] = c13; tens2[14] = c33;
    tens2[21] = c44; tens2[28] = c44; tens2[35] = c66;

    int a=0, b = 0;
    int i = 0, j = 0, k = 0, l = 0;
    for(int n=0; n<36; ++n) {

        if((n%6 == 0) && n!=0) a+=1;
        b = n - 6*a;

        if(a==0 || a==1 || a==2) i=a, j=a;
        if(a==3) i=1, j=2;
        if(a==4) i=0, j=2;
        if(a==5) i=0, j=1;

        if(b==0 || b==1 || b==2) k=b, l=b;
        if(b==3) k=1, l=2;
        if(b==4) k=0, l=2;
        if(b==5) k=0, l=1;

	double v = tens2[b+6*a];
        new_tens.set(i,j,k,l,v);
	new_tens.set(j,i,k,l,v);
	new_tens.set(i,j,l,k,v);
	new_tens.set(j,i,l,k,v);
	new_tens.set(k,l,i,j,v);
	new_tens.set(l,k,i,j,v);
	new_tens.set(l,k,j,i,v);
	new_tens.set(k,l,j,i,v);
    }
    return new_tens;
}

Matrix christoffel(const Tensor4& tens4, const Vec3& n) {
  Matrix new_matrix(3,3);
  
    //\Gamma_il= c_ijkl * n_j * n_k;
    for(int i=0; i<3; ++i) {
        for(int l=0; l<3; ++l) {
	  double val = 0;
	  for(int j=0; j<3; ++j) {
	    for(int k=0; k<3; ++k) {
	      val += tens4.at(i,j,k,l) * n.at(j) * n.at(k);
	    }
	  }
	  new_matrix.Set(i, l, val);
        }
    }
  
    return new_matrix;
}


Polynom MatrixToPoly(const Matrix& mat)
{
  Matrix mat2 = mat*mat;
  Polynom Pol(1,2,3,4);
  double a1,a2,a3,a4; // polynomial coefficents at x^3, x^2, x^1, x^0; 
  a1 = 1;
  a2 = -1*mat.trace();
  a3 = -0.5*mat2.trace() + 0.5*mat.trace()*mat.trace();
  a4 = -1*mat.det();
  Pol.set(a1,a2,a3,a4); 
  return Pol;
}

Vec3 CalcPol(const Matrix& G) {
    Vec3 a0, a1, a2;

    a0.set(0, G.Get(0,1)*G.Get(1,2) -
              G.Get(1,1)*G.Get(0,2));
    a0.set(1, G.Get(1,0)*G.Get(0,2) -
              G.Get(0,0)*G.Get(1,2));
    a0.set(2, G.Get(0,0)*G.Get(1,1) -
              G.Get(1,0)*G.Get(0,1));

    a1.set(0, G.Get(1,1)*G.Get(2,2) -
              G.Get(2,1)*G.Get(1,2));
    a1.set(1, G.Get(2,0)*G.Get(1,2) -
              G.Get(1,0)*G.Get(2,2));
    a1.set(2, G.Get(1,0)*G.Get(2,1) -
              G.Get(2,0)*G.Get(1,1));

    a2.set(0, G.Get(0,1)*G.Get(2,2) -
              G.Get(2,1)*G.Get(0,2));
    a2.set(1, G.Get(2,0)*G.Get(0,2) -
              G.Get(0,0)*G.Get(2,2));
    a2.set(2, G.Get(0,0)*G.Get(2,1) -
              G.Get(2,0)*G.Get(0,1));

    double norm[3] = {a0.abs(), a1.abs(), a2.abs()};
    double max_norm = norm[0];
    double n = 0;
    for(int i=1; i<=2; ++i) {
        if(max_norm < norm[i]) {
            max_norm = norm[i];
            n += 1;
        }
    }

    if(n==0) return a0.normalized();
    if(n==1) return a1.normalized();
    return a2.normalized();
}

Matrix eval(const Matrix& a, double gamma){
  Matrix b(3,3);
  b = a;
  for(int i = 0; i < 3; ++i){
    b[i][i] = a[i][i] - gamma; 
  }
  return b;
}

vector<SolPart> solveChristoffel(const Matrix& chrMat){
  vector<SolPart> ret(3);
  vector<double> roots;
  Matrix Eigen(3,3);  
  Polynom Pol;
  Vec3 polvector;
  Pol = MatrixToPoly(chrMat);
  roots = Pol.solvePolynom();
  for(int i=0;i<2;++i){
    Eigen = eval(chrMat,roots[i]);
    polvector = CalcPol(Eigen);
    ret[i].Put(roots[i], polvector);
    }
  return ret;
}

Matrix Rot_x(double a)
{
  Matrix dat(3,3);
  
  dat.Set(0, 0, 1);
  dat.Set(0, 1, 0);
  dat.Set(0, 2, 0);

  dat.Set(1, 0, 0);
  dat.Set(1, 1, cos(a));
  dat.Set(1, 2, sin(a));

  dat.Set(2, 0, 0);
  dat.Set(2, 1, -sin(a));
  dat.Set(2, 2, cos(a));

return dat;
}

Matrix Rot_y(double a)
{
  Matrix dat(3,3);
  
  dat.Set(0, 0, cos(a));
  dat.Set(0, 1, 0);
  dat.Set(0, 2, -sin(a));

  dat.Set(1, 0, 0);
  dat.Set(1, 1, 1);
  dat.Set(1, 2, 0);

  dat.Set(2, 0, sin(a));
  dat.Set(2, 1, 0);
  dat.Set(2, 2, cos(a));

return dat;
}


Matrix Rot_z(double a)
{
  Matrix dat(3,3);
  
  dat.Set(0, 0, cos(a));
  dat.Set(0, 1, sin(a));
  dat.Set(0, 2, 0);

  dat.Set(1, 0, -sin(a));
  dat.Set(1, 1, cos(a));
  dat.Set(1, 2, 0);

  dat.Set(2, 0, 0);
  dat.Set(2, 1, 0);
  dat.Set(2, 2, 1);

return dat;
}
