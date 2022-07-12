#include "myMath.h"
namespace myMath
{

	float ChangeRadi(const float& angle)
	{
		float radian = angle * PI / 180;
		return radian;
	}

	float ChangeDira(const float& angle)
	{
		float radian = angle * 180 / PI;
		return radian;
	}

	Matrix4 IdentityMatrix()
	{
		Matrix4 matIdentity = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

		return matIdentity;
	}

	void ScalingForm(Matrix4& matScal, Vector3& scale)
	{

		//ƒXƒP[ƒŠƒ“ƒO”{—¦‚ğs—ñ‚Éİ’è‚·‚é
		matScal.m[0][0] = scale.x;
		matScal.m[1][1] = scale.y;
		matScal.m[2][2] = scale.z;
	}

	Matrix4 ScalingForm(Vector3& scale)
	{
		Matrix4 matScal = IdentityMatrix();
		//ƒXƒP[ƒŠƒ“ƒO”{—¦‚ğs—ñ‚Éİ’è‚·‚é
		matScal.m[0][0] = scale.x;
		matScal.m[1][1] = scale.y;
		matScal.m[2][2] = scale.z;
		return matScal;
	}

	void RotationXForm(Matrix4& matRotX, float angle)
	{
		// X²‰ñ“]s—ñ‚ÌŠe—v‘f‚ğİ’è‚·‚é
		matRotX.m[1][1] = std::cos(angle);
		matRotX.m[1][2] = std::sin(angle);

		matRotX.m[2][1] = -std::sin(angle);
		matRotX.m[2][2] = std::cos(angle);
	}

	Matrix4 RotationXForm(float angle)
	{
		Matrix4 matRotX = IdentityMatrix();

		// X²‰ñ“]s—ñ‚ÌŠe—v‘f‚ğİ’è‚·‚é
		matRotX.m[1][1] = std::cos(angle);
		matRotX.m[1][2] = std::sin(angle);

		matRotX.m[2][1] = -std::sin(angle);
		matRotX.m[2][2] = std::cos(angle);

		return matRotX;
	}

	void RotationYForm(Matrix4& matRotY, float angle)
	{
		// Y²‰ñ“]s—ñ‚ÌŠe—v‘f‚ğİ’è‚·‚é
		matRotY.m[0][0] = std::cos(angle);
		matRotY.m[0][2] = -std::sin(angle);

		matRotY.m[2][0] = std::sin(angle);
		matRotY.m[2][2] = std::cos(angle);
	}

	Matrix4 RotationYForm(float angle)
	{
		Matrix4 matRotY = IdentityMatrix();

		// Y²‰ñ“]s—ñ‚ÌŠe—v‘f‚ğİ’è‚·‚é
		matRotY.m[0][0] = std::cos(angle);
		matRotY.m[0][2] = -std::sin(angle);

		matRotY.m[2][0] = std::sin(angle);
		matRotY.m[2][2] = std::cos(angle);

		return matRotY;
	}

	void RotationZForm(Matrix4& matRotZ, float angle)
	{
		// Z²‰ñ“]s—ñ‚ÌŠe—v‘f‚ğİ’è‚·‚é
		matRotZ.m[0][0] = std::cos(angle);
		matRotZ.m[0][1] = std::sin(angle);

		matRotZ.m[1][0] = -std::sin(angle);
		matRotZ.m[1][1] = std::cos(angle);
	}

	Matrix4 RotationZForm(float angle)
	{
		Matrix4 matRotZ = IdentityMatrix();
		// Z²‰ñ“]s—ñ‚ÌŠe—v‘f‚ğİ’è‚·‚é
		matRotZ.m[0][0] = std::cos(angle);
		matRotZ.m[0][1] = std::sin(angle);

		matRotZ.m[1][0] = -std::sin(angle);
		matRotZ.m[1][1] = std::cos(angle);

		return matRotZ;
	}

	void RotationForm(Matrix4& matRot, Vector3& rotaion)
	{
		// matWorld_‚ÉZ²‰ñ“]s—ñ‚ğŠ|‚¯Z
		matRot *= RotationZForm(rotaion.z);
		// matWorld_‚ÉX²‰ñ“]s—ñ‚ğŠ|‚¯Z
		matRot *= RotationXForm(rotaion.x);
		// matWorld_‚ÉY²‰ñ“]s—ñ‚ğŠ|‚¯Z
		matRot *= RotationYForm(rotaion.y);
	}

	Matrix4 RotationForm(Vector3& rotaion)
	{
		Matrix4 matRot = IdentityMatrix();

		// matWorld_‚ÉZ²‰ñ“]s—ñ‚ğŠ|‚¯Z
		matRot *= RotationZForm(rotaion.z);
		// matWorld_‚ÉX²‰ñ“]s—ñ‚ğŠ|‚¯Z
		matRot *= RotationXForm(rotaion.x);
		// matWorld_‚ÉY²‰ñ“]s—ñ‚ğŠ|‚¯Z
		matRot *= RotationYForm(rotaion.y);

		return matRot;
	}

	void TransferForm(Matrix4& matTrans, Vector3& trans)
	{
		//ˆÚ“®—Ê‚ğs—ñ‚Éİ’è‚·‚é
		matTrans.m[3][0] = trans.x;
		matTrans.m[3][1] = trans.y;
		matTrans.m[3][2] = trans.z;
	}

	Matrix4 TransferForm(Vector3& trans)
	{
		//•½sˆÚ“®s—ñ‚ğéŒ¾
		Matrix4 matTrans = IdentityMatrix();

		//ˆÚ“®—Ê‚ğs—ñ‚Éİ’è‚·‚é
		matTrans.m[3][0] = trans.x;
		matTrans.m[3][1] = trans.y;
		matTrans.m[3][2] = trans.z;

		return matTrans;
	}

	//ƒ[ƒ‹ƒhs—ñ‚ğ¶¬
	void WorldForm(Matrix4& matWorld, WorldTransform& worldTransform)
	{
		// matWorld_‚ÉƒXƒP[ƒŠƒ“ƒO”{—¦‚ğŠ|‚¯Z
		matWorld *= ScalingForm(worldTransform.scale_);

		// matWorld_‚ÉZ²‰ñ“]s—ñ‚ğŠ|‚¯Z
		matWorld *= RotationForm(worldTransform.rotation_);

		// matWorld_‚ÉˆÚ“®—Ê‚ğŠ|‚¯Z
		matWorld *= TransferForm(worldTransform.translation_);
	}

	Matrix4 WorldForm(WorldTransform& worldTransform)
	{
		Matrix4 matWorld = IdentityMatrix();

		// matWorld_‚ÉƒXƒP[ƒŠƒ“ƒO”{—¦‚ğŠ|‚¯Z
		matWorld *= ScalingForm(worldTransform.scale_);

		// matWorld_‚É‰ñ“]s—ñ‚ğŠ|‚¯Z
		matWorld *= RotationForm(worldTransform.rotation_);

		// matWorld_‚ÉˆÚ“®—Ê‚ğŠ|‚¯Z
		matWorld *= TransferForm(worldTransform.translation_);

		return matWorld;
	}

	void AffineTransformation(WorldTransform& worldTransform)
	{
		//’PˆÊs—ñ‚ğ‘ã“ü
		worldTransform.matWorld_ = IdentityMatrix();

		// matWorld_‚ÉƒXƒP[ƒŠƒ“ƒO”{—¦‚ğŠ|‚¯Z
		worldTransform.matWorld_ *= ScalingForm(worldTransform.scale_);

		// matWorld_‚É‰ñ“]s—ñ‚ğŠ|‚¯Z
		worldTransform.matWorld_ *= RotationForm(worldTransform.rotation_);

		// matWorld_‚ÉˆÚ“®—Ê‚ğŠ|‚¯Z
		worldTransform.matWorld_ *= TransferForm(worldTransform.translation_);

		//s—ñ‚Ì“]‘—
		worldTransform.TransferMatrix();
	}

	void ParenChildUpdate(WorldTransform& worldTransform)
	{
		worldTransform.matWorld_ = WorldForm(worldTransform);

		worldTransform.matWorld_ *= worldTransform.parent_->matWorld_;

		worldTransform.TransferMatrix();
	}

	Vector3 VecMatMul(Vector3& vec, Matrix4& mat)
	{
		Vector3 retVec = {};

		retVec.x = vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0];

		retVec.y = vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1];

		retVec.z = vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2];

		return retVec;
	}

	const Vector3 Vector3Sub(const Vector3& v1, const Vector3& v2)
	{
		Vector3 temp(v1);
		return temp -= v2;
	}

	float Vector3Length(Vector3 vec)
	{
		return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	}

	Vector3& Vector3Normalize(Vector3& vec)
	{
		float len = Vector3Length(vec);

		if (len != 0)
		{
			return vec /= len;
		}

		return vec;
	}

	//ƒ[ƒ‹ƒhÀ•W‚ğæ“¾
	Vector3 GetWorldPosition(WorldTransform& worldTransform_)
	{
		//ƒ[ƒ‹ƒhÀ•W‚ğ“ü‚ê‚é•Ï”
		Vector3 worldPos;
		//ƒ[ƒ‹ƒhs—ñ‚Ì•½sˆÚ“®¬•ª‚ğæ“¾(ƒ[ƒ‹ƒhÀ•W)
		worldPos.x = worldTransform_.matWorld_.m[3][0];
		worldPos.y = worldTransform_.matWorld_.m[3][1];
		worldPos.z = worldTransform_.matWorld_.m[3][2];

		return worldPos;
	}

} // namespace MyMath