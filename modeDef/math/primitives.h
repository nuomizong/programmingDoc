/

inline void RigidTransform::compose(const RigidTransform& a, const RigidTransform& b)
{
  //t = ta + Ra*tb
  //R = Ra*Rb
  a.R.mul(b.t, t);
  t += a.t;
  R.mul(a.R,b.R);
}
inline void RigidTransform::mul(const RigidTransform& a, const RigidTransform& b) { compose(a,b); }
//vector operators
inline void RigidTransform::mul(const Vector3& a, Vector3& out) const { mulPoint(a,out); }
inline void RigidTransform::mul(const Vector4& a, Vector3& out) const { R.mul(Vector3(a.x,a.y,a.z), out); out.madd(t, a.w); }
inline void RigidTransform::mulPoint(const Vector3& a, Vector3& out) const { R.mul(a,out); out += t; }
inline void RigidTransform::mulVector(const Vector3& a, Vector3& out) const { R.mul(a,out); }


inline void RigidTransform::inplaceInverse() { R.inplaceTranspose(); Vector3 tmp; tmp.setNegative(t); R.mul(tmp,t); }
inline void RigidTransform::inplaceRotate(const Matrix3& m) { R *= m; }
inline void RigidTransform::inplaceShift(const Vector3& v) { t += v; }



//inlined standalone functions/operators (less efficient than the member functions)

//Vector3



//RigidTransform

inline RigidTransform operator * (const RigidTransform& a, const RigidTransform& b)
{
	RigidTransform x;
	x.compose(a,b);
	return x;
}

inline RigidTransform operator * (const RigidTransform& a, const Matrix3& b)
{
	RigidTransform x;
	x.R.mul(a.R,b);
	x.t = a.t;
	return x;
}

inline RigidTransform operator * (const Matrix3& a, const RigidTransform& b)
{
	RigidTransform x;
	x.R.mul(a,b.R);
	a.mul(b.t,x.t);
	return x;
}

inline RigidTransform operator + (const RigidTransform& a, const Vector3& b)
{
	return RigidTransform (a.R,a.t+b);
}

inline RigidTransform operator - (const RigidTransform& a, const Vector3& b)
{
	return RigidTransform (a.R,a.t-b);
}

inline Vector3 operator * (const RigidTransform& a, const Vector3& b)
{
	Vector3 x;
	a.mul(b,x);
	return x;
}





#endif
