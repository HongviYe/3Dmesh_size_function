
/* ----------------------------------------------------------------------------
* ----------------------------------------------------------------------------
*
* ��ѧ��Ӧ��ģ��ĸ��ܻ���
* Enabling Environment for Multi-displinary Application Simulations
*
* �½��� �й� �㽭��ѧ�������ѧ�����о�����
* ��Ȩ����	  2007��10��15��
* Chen Jianjun  Center for Engineering & Scientific Computation,
* Zhejiang University, P. R. China
* Copyright reserved, Oct. 15, 2007
*
* ��ϵ��ʽ (For further information, please conctact)
*   �绰 (Tel)��+86-571-87953165
*   ���� (Fax)��+86-571-87953167
*   ���� (Mail)��chenjj@zju.edu.cn
*
* �ļ����� (File Name)��vector.h
* ��ʼ�汾 (Initial Version): V1.0
* ���ܽ��� (Function Introduction��
*     ������һ���ܶȿ��ƻ���
*     Define a set of element spacing controlling scheme.
*
*
* -----------------------------�޸ļ�¼ (Revision Record)------------------------
* �޸��� (Revisor): yhf
* �޸����� (Revision Date):20190823
* ��ǰ�汾 (Current Version):
* �޸Ľ��� (Revision Introduction):�������쳣����������һϵ�е��·��ţ�������QVector3D���������޸�
* ------------------------------------------------------------------------------
* ------------------------------------------------------------------------------*/


#include "vertex.h"
#include <cmath>


/* *********************************************************************************
* ������άʸ��
* define 3-dimensional vector
* **********************************************************************************/

double& vertex::operator[](const int & index)
{
	if (index > 2) {
		return x;
	}
	if (index == 0)
		return x;
	if (index == 1)
		return y;
	if (index == 2)
		return z;
}

/* ʸ����ģ calc. magnitude */
double vertex::magnitude() const
{
	return sqrt(x * x + y * y + z * z);
}

double vertex::length() const
{
	return magnitude();
}

void vertex::set(double x0, double y0, double z0)
{
	x = x0, y = y0, z = z0;
}

bool vertex::isApproximatelyEqualTo(const vertex v1)
{
	return (abs(x - v1.x) <  (1e-7)) && (abs(y - v1.y) <  (1e-7)) && (abs(z - v1.z) <  (1e-7));
}

/* ʸ����ģ calc. magnitude */
double vertex::magnitude2() const
{
	return (x * x + y * y +
		z * z);
}
ostream& operator <<(ostream& out,const  vertex& vec) {
	out <<"("<< vec.x << " " << vec.y << " " << vec.z << ")";
	return out;
}
double vertex::dotProduct(const vertex v1, const vertex v2)
{
	return v1 * v2;
}

vertex vertex::crossProduct(const vertex v1, const vertex v2)
{
	return v1 ^ v2;
}
vertex vertex::normalized() {
	double m = magnitude();
	if (m == 0)
	{
		return vertex(0, 0, 0);
	}
	else {
		return vertex(x / m, y / m, z / m);
	}
}
/* ��һ normalization */
void vertex::normalize()
{
	double m = magnitude();
	if (abs(m) < 1e-10)//�޸ĸ���������0Ϊ����ֵС����� yhf20190823
	{
		x = 0.0;
		y = 0.0;
		z = 0.0;
	}
	else {
		x = x / m;
		y = y / m;
		z = z / m;
	}
}

/* **********************************
* ���ز����� overloaded operator
* **********************************/

/* ��ֵ assignment */
const vertex& vertex::operator = (const vertex& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

const vertex& vertex::operator = (double v)
{
	x = y = z = v;
	return *this;
}

/* �������� simple mathamatic operator */
vertex vertex::operator + (const vertex& v) const
{

	return vertex(x + v.x, y + v.y, z + v.z);
}

vertex vertex::operator - (const vertex& v) const
{
	return vertex(x - v.x, y - v.y, z - v.z);
}

vertex vertex::operator * (const double& s) const
{
	return vertex(x * s,y*s,z*s);
}
vertex vertex::operator / (const double& s) const
{

	return vertex(x/ s, y/s, z/s);
}


vertex vertex::operator/(const vertex & v) const
{
	if (abs(v.x) < 1e-10)
		return vertex(1.0, 0.0,0.0);
	if (abs(v.y) < 1e-10)
		return vertex(0.0, 1.0, 0.0);
	if (abs(v.z) < 1e-10)
		return vertex(0.0, 0.0, 1.0);
	return  vertex(x / v.x, y / v.y, z / v.z);
}


vertex vertex::getmul(const vertex & v) const
{
	vertex vr;
	vr.x = x * v.x;
	vr.y = y * v.y;
	vr.z = z * v.z;
	return vr;
}

vertex& vertex::operator += (const vertex& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}
vertex& vertex::operator -= (const vertex& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

vertex& vertex::operator += (double delta)
{
	x += delta;
	y += delta;
	z += delta;
	return *this;
}
vertex& vertex::operator -= (double delta)
{
	x -= delta;
	y -= delta;
	z -= delta;
	return *this;
}


/* ��� & ��� dot & cross */
double vertex::operator * (const vertex& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

vertex vertex::operator ^ (const vertex& v) const
{
	vertex vr;
	vr.x = y * v.z - v.y * z;
	vr.y = v.x * z - x * v.z;
	vr.z = x * v.y - y * v.x;
	return vr;
}

vertex vertex::operator%(const vertex & v) const
{
	vertex vr;
	vr.x = y * v.z - v.y * z;
	vr.y = v.x * z - x * v.z;
	vr.z = x * v.y - y * v.x;
	return vr;
}


vertex operator - (const vertex& v)
{
	vertex vr;
	vr.x = -v.x;
	vr.y = -v.y;
	vr.z = -v.z;
	return vr;
}
vertex operator * (double scale, const vertex& v)
{
	return v * scale;
}

