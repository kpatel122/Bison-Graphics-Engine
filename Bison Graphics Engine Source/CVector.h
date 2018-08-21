// This is our basic 3D point/vector class

#include <math.h>
struct CVector3
{
public:
	
	// A default constructor
	CVector3() {}

	// This is our constructor that allows us to initialize our data upon creating an instance
	CVector3(float X, float Y, float Z) 
	{ 
		x = X; y = Y; z = Z;
	}

	// Here we overload the + operator so we can add vectors together 
	CVector3 operator+(CVector3 vVector)
	{
		// Return the added vectors result.
		return CVector3(vVector.x + x, vVector.y + y, vVector.z + z);
	}

	// Here we overload the - operator so we can subtract vectors 
	CVector3 operator-(CVector3 vVector)
	{
		// Return the subtracted vectors result
		return CVector3(x - vVector.x, y - vVector.y, z - vVector.z);
	}
	
	// Here we overload the * operator so we can multiply by scalars
	CVector3 operator*(float num)
	{
		// Return the scaled vector
		return CVector3(x * num, y * num, z * num);
	}
	
	
	float operator*(const CVector3 &rhs) const
	{
    return x * rhs.x + y * rhs.y + z * rhs.z;
	}
	
	// Here we overload the / operator so we can divide by a scalar
	CVector3 operator/(float num)
	{
		// Return the scale vector
		return CVector3(x / num, y / num, z / num);
	}

	const float Length() const
	{
		return (float)sqrt((double)(x*x + y*y + z*z));
	}

	float operator%(const CVector3 &vec) const
	{
		// Return the scale vector
		return x*vec.x + y*vec.y + z*vec.z;
	}





float x, y, z;						
};


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

// This is our basic 2D point class.  This will be used to store the UV coordinates.
class CVector2 
{
public:

		// A default constructor
	CVector2() {}

	// This is our constructor that allows us to initialize our data upon creating an instance
	CVector2(float X, float Y) 
	{ 
		x = X; y = Y;
	}

	// Here we overload the + operator so we can add vectors together 
	CVector2 operator+(CVector2 vVector)
	{
		// Return the added vectors result.
		return CVector2(vVector.x + x, vVector.y + y);
	}

	// Here we overload the - operator so we can subtract vectors 
	CVector2 operator-(CVector2 vVector)
	{
		// Return the subtracted vectors result
		return CVector2(x - vVector.x, y - vVector.y);
	}
	
	// Here we overload the * operator so we can multiply by scalars
	CVector2 operator*(float num)
	{
		// Return the scaled vector
		return CVector2(x * num, y * num);
	}

	// Here we overload the / operator so we can divide by a scalar
	CVector2 operator/(float num)
	{
		// Return the scale vector
		return CVector2(x / num, y / num);
	}

	float x, y;
};

CVector3 Cross(CVector3 vVector1, CVector3 vVector2);
float Magnitude(CVector3 vNormal);
CVector3 Normalize(CVector3 vVector);