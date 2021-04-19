/* ----------------------------------------------------------------------------
* ----------------------------------------------------------------------------
*
* 三维各向同性Delaunay网格生成器 (版本号：0.3)
* 3D Isotropic Delaunay Mesh Generation (Version 0.3)
*
* 陈建军 中国 浙江大学工程与科学计算研究中心
* 版权所有	  2005年9月15日
* Chen Jianjun  Center for Engineering & Scientific Computation,
* Zhejiang University, P. R. China
* Copyright reserved, 2005, 10, 26
*
* 联系方式
*   电话：+86-571-87953165
*   传真：+86-571-87953167
*   邮箱：zdchenjj@yahoo.com.cn
* For further information, please conctact
*  Tel: +86-571-87953165
*  Fax: +86-571-87953167
* Mail: zdchenjj@yahoo.com.cn
*
* ------------------------------------------------------------------------------
* ------------------------------------------------------------------------------*/

#ifndef __iso3d_vertex_h__
#define	__iso3d_vertex_h__
#include<iostream>
using namespace std;
/**
* @brief 定义三维矢量
* define 3-dimensional vector
* @note 从使用方便的角度出发，返回值不应该为const，已被yhf修改2019/08/23
*/

class vertex
{
public:
	friend ostream& operator <<(ostream&,const vertex& vec);

	vertex() { x = y = z = 0.; }
	vertex(double v1, double v2, double v3) {
		x = v1; y = v2; z = v3;
	}
	vertex(double v[3]) {
		x = v[0]; y = v[1]; z = v[2];
	}
	vertex(const vertex& v):x(v.x),y(v.y),z(v.z) {}
	double& operator[](const int &index);
	/* 矢量求模 calc. magnitude */
	double magnitude() const;

	double length() const;

	void set(double x0,double y0,double z0);

	bool isApproximatelyEqualTo(const vertex v1);

	double magnitude2() const;

	/**
	* @brief 返回点积
	* @author yhf
	* @return 返回点积结果
	*/
	static double dotProduct(const vertex v1, const vertex v2);

	/**
	* @brief 返回叉积
	* @author yhf
	* @return 返回叉积结果
	* @brief 符合右手定则，从v1绕向v2
	*/
	static vertex crossProduct(const vertex v1, const vertex v2);
	vertex normalized();

	/* 归一 normalization */
	void normalize();

	/* **********************************
	* 重载操作符 overloaded operator
	* **********************************/

	/* 赋值 assignment */
	const vertex& operator = (const vertex& v);
	const vertex& operator = (double v);

	/* 四则运算 simple mathamatic operator */
	vertex operator + (const vertex& v) const;

	vertex operator - (const vertex& v) const;
	vertex operator * (const double& s) const;
	vertex operator / (const double& s) const;

	/*
	* x,y,z分别除
	*/
	vertex operator / (const vertex& v) const;
	/*
	* x,y,z分别乘
	*/
	vertex getmul (const vertex& v) const;

	vertex& operator += (const vertex& v);
	vertex& operator -= (const vertex& v);
	vertex& operator += (double delta);
	vertex& operator -= (double delta);

	/* 点积 & 叉积 dot & cross */
	double operator * (const vertex& v) const;
	vertex operator ^ (const vertex& V) const;
	vertex operator % (const vertex& v) const;

	friend vertex operator - (const vertex& p);
	friend vertex operator * (double scale, const vertex& v);

public:
	double x, y, z;
};

#endif 
