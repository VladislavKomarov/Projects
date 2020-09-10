#include <windows.h>											// Header File For Windows

#include "math.h"                                               // Needed for sqrtf

#include "MouseRotation.h"

Matrix4fT   Transform = { 1.0f, 0.0f, 0.0f, 0.0f,				// NEW: Final Transform
0.0f, 1.0f, 0.0f, 0.0f,
0.0f, 0.0f, 1.0f, 0.0f,
0.0f, 0.0f, 0.0f, 1.0f };

Matrix3fT   LastRot = { 1.0f, 0.0f, 0.0f,					// NEW: Last Rotation
0.0f, 1.0f, 0.0f,
0.0f, 0.0f, 1.0f };

Matrix3fT   ThisRot = { 1.0f, 0.0f, 0.0f,					// NEW: This Rotation
0.0f, 1.0f, 0.0f,
0.0f, 0.0f, 1.0f };

extern GLint winWidth, winHeight;
ArcBallT    ArcBall(winWidth, winHeight);				                // NEW: ArcBall Instance
Point2fT    MousePt;												// NEW: Current Mouse Point
bool        isClicked = false;										// NEW: Clicking The Mouse?
bool        isRClicked = false;										// NEW: Clicking The Right Mouse Button?
bool        isDragging = false;					                    // NEW: Dragging The Mouse?

void ArcBall_t::_mapToSphere(const Point2fT* NewPt, Vector3fT* NewVec) const
{
	Point2fT TempPt;
	GLfloat length;

	//Copy paramter into temp point
	TempPt = *NewPt;

	//Adjust point coords and scale down to range of [-1 ... 1]
	TempPt.s.X = (TempPt.s.X * this->AdjustWidth) - 1.0f;
	TempPt.s.Y = 1.0f - (TempPt.s.Y * this->AdjustHeight);

	//Compute the square of the length of the vector to the point from the center
	length = (TempPt.s.X * TempPt.s.X) + (TempPt.s.Y * TempPt.s.Y);

	//If the point is mapped outside of the sphere... (length > radius squared)
	if (length > 1.0f)
	{
		GLfloat norm;

		//Compute a normalizing factor (radius / sqrt(length))
		norm = 1.0f / FuncSqrt(length);

		//Return the "normalized" vector, a point on the sphere
		NewVec->s.X = TempPt.s.X * norm;
		NewVec->s.Y = TempPt.s.Y * norm;
		NewVec->s.Z = 0.0f;
	}
	else    //Else it's on the inside
	{
		//Return a vector to a point mapped inside the sphere sqrt(radius squared - length)
		NewVec->s.X = TempPt.s.X;
		NewVec->s.Y = TempPt.s.Y;
		NewVec->s.Z = FuncSqrt(1.0f - length);
	}
}

//Create/Destroy
ArcBall_t::ArcBall_t(GLfloat NewWidth, GLfloat NewHeight)
{
	//Clear initial values
	this->StVec.s.X =
		this->StVec.s.Y =
		this->StVec.s.Z =

		this->EnVec.s.X =
		this->EnVec.s.Y =
		this->EnVec.s.Z = 0.0f;

	//Set initial bounds
	this->setBounds(NewWidth, NewHeight);
}

void ArcBall_t::ArcBall_t_Set(GLfloat NewWidth, GLfloat NewHeight)
{
	//Clear initial values
	this->StVec.s.X =
		this->StVec.s.Y =
		this->StVec.s.Z =

		this->EnVec.s.X =
		this->EnVec.s.Y =
		this->EnVec.s.Z = 0.0f;

	//Set initial bounds
	this->setBounds(NewWidth, NewHeight);
}

//Mouse down
void    ArcBall_t::click(const Point2fT* NewPt)
{
	//Map the point to the sphere
	this->_mapToSphere(NewPt, &this->StVec);
}

//Mouse drag, calculate rotation
void    ArcBall_t::drag(const Point2fT* NewPt, Quat4fT* NewRot)
{
	//Map the point to the sphere
	this->_mapToSphere(NewPt, &this->EnVec);

	//Return the quaternion equivalent to the rotation
	if (NewRot)
	{
		Vector3fT  Perp;

		//Compute the vector perpendicular to the begin and end vectors
		Vector3fCross(&Perp, &this->StVec, &this->EnVec);

		//Compute the length of the perpendicular vector
		if (Vector3fLength(&Perp) > Epsilon)    //if its non-zero
		{
			//We're ok, so return the perpendicular vector as the transform after all
			NewRot->s.X = Perp.s.X;
			NewRot->s.Y = Perp.s.Y;
			NewRot->s.Z = Perp.s.Z;
			//In the quaternion values, w is cosine (theta / 2), where theta is rotation angle
			NewRot->s.W = Vector3fDot(&this->StVec, &this->EnVec);
		}
		else                                    //if its zero
		{
			//The begin and end vectors coincide, so return an identity transform
			NewRot->s.X =
				NewRot->s.Y =
				NewRot->s.Z =
				NewRot->s.W = 0.0f;
		}
	}
}

void Update()														// Perform Motion Updates Here
{
	if (isRClicked)													// If Right Mouse Clicked, Reset All Rotations
	{
		Matrix3fSetIdentity(&LastRot);								// Reset Rotation
		Matrix3fSetIdentity(&ThisRot);								// Reset Rotation
		Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot);		// Reset Rotation
	}

	if (!isDragging)												// Not Dragging
	{
		if (isClicked)												// First Click
		{
			isDragging = true;										// Prepare For Dragging
			LastRot = ThisRot;										// Set Last Static Rotation To Last Dynamic One
			ArcBall.click(&MousePt);								// Update Start Vector And Prepare For Dragging
		}
	}
	else
	{
		if (isClicked)												// Still Clicked, So Still Dragging
		{
			Quat4fT     ThisQuat;

			ArcBall.drag(&MousePt, &ThisQuat);						// Update End Vector And Get Rotation As Quaternion
			Matrix3fSetRotationFromQuat4f(&ThisRot, &ThisQuat);		// Convert Quaternion Into Matrix3fT
			Matrix3fMulMatrix3f(&ThisRot, &LastRot);				// Accumulate Last Rotation Into This One
			Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot);	// Set Our Final Transform's Rotation From This One
		}
		else														// No Longer Dragging
			isDragging = false;
	}
}
