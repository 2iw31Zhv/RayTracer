//���һԪ�Ĵη��� a*x^4+b*x^3+c*x^2+d*x+e=0��
//��������ϵ��a,b,c,d,e; ����ĸ���ֱ�Ϊx1,x2,x3,x4��

#include  <complex>
#include  <iostream>
#include  <math.h>
using   namespace   std;

//�����õ��Ĵη��̵ĺ�����������
void quartic_equation(
					  double a,double b,double c,double d,double e,
					  complex<double> &x1,complex<double> &x2,
					  complex<double> &x3,complex<double> &x4
					  );

void main()
{
	double a,b,c,d,e;
	complex<double>  x1,x2,x3,x4;

	std::cout<<"����һԪ�Ĵη��� a*x^4+b*x^3+c*x^2+d*x+e=0 ��ϵ�� a, b, c, d, e : "; 
	std::cin>>a>>b>>c>>d>>e; 

	quartic_equation(a,b,c,d,e,x1,x2,x3,x4);

	std::cout<<" x1="<< x1 <<endl;
	std::cout<<" x2="<< x2 <<endl;
	std::cout<<" x3="<< x3 <<endl;
	std::cout<<" x4="<< x4 <<endl;

}






// 1. ���η��̡��Ĵη����������õĶ��η��̣�
void  quadratic_equation(double a4,double b4,double c4,complex<double> &z1,complex<double> &z2)
{
		
	double delta;
	complex<double> temp1,temp2;

	delta=b4*b4-4*a4*c4; 

	
	complex<double> temp(delta,0);

    temp1 = (-b4)/(2*a4);
	temp2 = sqrt(temp)/(2*a4);

	z1=temp1+temp2;
	z2=temp1-temp2;
	
}





// 2. �Ĵη����������õ����η��̣�
double  cubic_equation(double a3,double b3,double c3,double d3)
{

	double p,q,delta; 
	double M,N;	
	double y0;

	complex<double> temp1,temp2;
	complex<double> x1,x2,x3;
	complex<double> y1,y2;



	if(a3==0)
	{
		quadratic_equation(b3,c3,d3,y1,y2);

		x1 = y1;
		x2 = y2;
		x3 = 0;
	}
	else
	{

    p = -1.0/3*pow((b3*1.0/a3),2.0)+c3*1.0/a3;                  
    q = 2.0/27*pow((b3*1.0/a3),3.0)-1.0/3*b3*c3/(a3*a3)+d3*1.0/a3;  
	
	// p = -1/3*(b3/a3)^2+c3/a3;
	// q = 2/27*(b3/a3)^3-1/3*b3*c3/(a3*a3)+d3/a3;	
	//���� y^3+p*y+q=0 ��ʽ��


	delta = pow((q/2.0),2.0)+pow((p/3.0),3.0);
	
    //�б�ʽ delta = (q/2)^2+(p/3)^3;
	
	//cout<<" delta>0,��һ��ʵ��������������delta=0,������ʵ����delta<0,���������ȵ�ʵ��"<<endl; 
    //cout<<" please output the discriminant  delta="<<delta<<endl;
	//delta>0,��һ��ʵ��������������
	//delta=0,������ʵ����
    //delta<0,���������ȵ�ʵ����

	
    complex<double>  omega1(-1.0/2, sqrt(3.0)/2.0);
	complex<double>  omega2(-1.0/2, -sqrt(3.0)/2.0);

	complex<double>  yy(b3/(3.0*a3),0.0);	

    
    M = -q/2.0;

	if(delta<0)
	{

		 N = sqrt(fabs(delta));
         complex<double>  s1(M,N);  
         complex<double>  s2(M,-N);	
		 
		 x1 = (pow(s1,(1.0/3))+pow(s2,(1.0/3)))-yy;
		 x2 = (pow(s1,(1.0/3))*omega1+pow(s2,(1.0/3))*omega2)-yy;
		 x3 = (pow(s1,(1.0/3))*omega2+pow(s2,(1.0/3))*omega1)-yy;
		 
	}
	else
	{

		N = sqrt(delta);

		complex<double>  f1(M+N,0); 
		complex<double>  f2(M-N,0);

		 if(M+N >= 0)
		     temp1 = pow((f1),1.0/3);
		 else
			 temp1 = -norm(pow(sqrt(f1),1.0/3));


		 if(M-N >= 0)
			 temp2 = pow((f2),1.0/3);
		 else		
			 temp2 = -norm(pow(sqrt(f2),1.0/3));


		 x1 = temp1+temp2-yy;		 
         x2 = omega1*temp1+omega2*temp2-yy;
         x3 = omega2*temp1+omega1*temp2-yy; 
		 
	}

	}

       y0 = real(x1);   
    return y0;

    // y0 Ϊ�����õ����η��̵ķ���ֵ��

}






// 3.main���������õ��Ĵη��̣�
void quartic_equation(
					  double a,double b,double c,double d,double e,
					  complex<double> &x1,complex<double> &x2,
					  complex<double> &x3,complex<double> &x4
					  )

{
	
	double a2,b2,c2;
	double a4,b4,c4;
	double a3,b3,c3,d3;	
	double y;
	complex<double>  y1,y2,y3,y4;
	

	//cout<<"����һԪ�Ĵη��� a*x^4+b*x^3+c*x^2+d*x+e=0 ��ϵ�� a, b, c, d, e : "; 
	//cin>>a>>b>>c>>d>>e; 

	//����һԪ�Ĵη��̵�ϵ�����÷���Ϊa*x^4+b*x^3+c*x^2+d*x+e=0��

	if(b==0 && c==0 && d==0 && e==0)
	{
		x1 = 0; x2 = 0; x3 = 0; x4 = 0;
	}
	else if (b==0 && d==0 && e==0)
	{
		b3 = a; c3 = 0; d3 = c;
		quadratic_equation(b3,c3,d3,y1,y2);
		x1 = y1; x2 = y2;  x3 = 0; x4 = 0; 
	}
	else
	{
//������ϵ�����Ĵη��̻�Ϊ����ϵ��Ϊ1���Ĵη��̣�
	 b= b/a;  c= c/a;  d= d/a;  e= e/a; a= a/a; 
	 
//�����õ����η��̵�ϵ��;	
	a3= 8.0;  
	b3= -4.0*c;  
	c3= 2.0*b*d-8.0*e;  
	d3= e*(4.0*c-b*b)-d*d;


	    y = cubic_equation(a3,b3,c3,d3);
		//�����η��̵ķ���ֵ���� y �� 


		
 //��һ�ε��õĶ��η��̵�ϵ����
	a2= 1.0;  
	b2= b/2.0-sqrt(8.0*y+b*b-4*c)/2.0;  
	c2= y-(b*y-d)/sqrt(8.0*y+b*b-4*c);
	quadratic_equation(a2,b2,c2,y1,y2);	

	x1 = y1;
	x2 = y2;


//�ڶ��ε��õĶ��η��̵�ϵ����
    a4= 1.0;   
	b4= b/2.0+sqrt(8.0*y+b*b-4.0*c)/2.0;   
	c4= y+(b*y-d)/sqrt(8.0*y+b*b-4.0*c);
	quadratic_equation(a4,b4,c4,y3,y4);

	x3 = y3;
	x4 = y4;

	}

	//������Ϊ����һԪ�Ĵη��� a*x^4+b*x^3+c*x^2+d*x+e=0 
    //���ĸ��⡣
}

