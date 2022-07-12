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

		//�X�P�[�����O�{�����s��ɐݒ肷��
		matScal.m[0][0] = scale.x;
		matScal.m[1][1] = scale.y;
		matScal.m[2][2] = scale.z;
	}

	Matrix4 ScalingForm(Vector3& scale)
	{
		Matrix4 matScal = IdentityMatrix();
		//�X�P�[�����O�{�����s��ɐݒ肷��
		matScal.m[0][0] = scale.x;
		matScal.m[1][1] = scale.y;
		matScal.m[2][2] = scale.z;
		return matScal;
	}

	void RotationXForm(Matrix4& matRotX, float angle)
	{
		// X����]�s��̊e�v�f��ݒ肷��
		matRotX.m[1][1] = std::cos(angle);
		matRotX.m[1][2] = std::sin(angle);

		matRotX.m[2][1] = -std::sin(angle);
		matRotX.m[2][2] = std::cos(angle);
	}

	Matrix4 RotationXForm(float angle)
	{
		Matrix4 matRotX = IdentityMatrix();

		// X����]�s��̊e�v�f��ݒ肷��
		matRotX.m[1][1] = std::cos(angle);
		matRotX.m[1][2] = std::sin(angle);

		matRotX.m[2][1] = -std::sin(angle);
		matRotX.m[2][2] = std::cos(angle);

		return matRotX;
	}

	void RotationYForm(Matrix4& matRotY, float angle)
	{
		// Y����]�s��̊e�v�f��ݒ肷��
		matRotY.m[0][0] = std::cos(angle);
		matRotY.m[0][2] = -std::sin(angle);

		matRotY.m[2][0] = std::sin(angle);
		matRotY.m[2][2] = std::cos(angle);
	}

	Matrix4 RotationYForm(float angle)
	{
		Matrix4 matRotY = IdentityMatrix();

		// Y����]�s��̊e�v�f��ݒ肷��
		matRotY.m[0][0] = std::cos(angle);
		matRotY.m[0][2] = -std::sin(angle);

		matRotY.m[2][0] = std::sin(angle);
		matRotY.m[2][2] = std::cos(angle);

		return matRotY;
	}

	void RotationZForm(Matrix4& matRotZ, float angle)
	{
		// Z����]�s��̊e�v�f��ݒ肷��
		matRotZ.m[0][0] = std::cos(angle);
		matRotZ.m[0][1] = std::sin(angle);

		matRotZ.m[1][0] = -std::sin(angle);
		matRotZ.m[1][1] = std::cos(angle);
	}

	Matrix4 RotationZForm(float angle)
	{
		Matrix4 matRotZ = IdentityMatrix();
		// Z����]�s��̊e�v�f��ݒ肷��
		matRotZ.m[0][0] = std::cos(angle);
		matRotZ.m[0][1] = std::sin(angle);

		matRotZ.m[1][0] = -std::sin(angle);
		matRotZ.m[1][1] = std::cos(angle);

		return matRotZ;
	}

	void RotationForm(Matrix4& matRot, Vector3& rotaion)
	{
		// matWorld_��Z����]�s����|���Z
		matRot *= RotationZForm(rotaion.z);
		// matWorld_��X����]�s����|���Z
		matRot *= RotationXForm(rotaion.x);
		// matWorld_��Y����]�s����|���Z
		matRot *= RotationYForm(rotaion.y);
	}

	Matrix4 RotationForm(Vector3& rotaion)
	{
		Matrix4 matRot = IdentityMatrix();

		// matWorld_��Z����]�s����|���Z
		matRot *= RotationZForm(rotaion.z);
		// matWorld_��X����]�s����|���Z
		matRot *= RotationXForm(rotaion.x);
		// matWorld_��Y����]�s����|���Z
		matRot *= RotationYForm(rotaion.y);

		return matRot;
	}

	void TransferForm(Matrix4& matTrans, Vector3& trans)
	{
		//�ړ��ʂ��s��ɐݒ肷��
		matTrans.m[3][0] = trans.x;
		matTrans.m[3][1] = trans.y;
		matTrans.m[3][2] = trans.z;
	}

	Matrix4 TransferForm(Vector3& trans)
	{
		//���s�ړ��s���錾
		Matrix4 matTrans = IdentityMatrix();

		//�ړ��ʂ��s��ɐݒ肷��
		matTrans.m[3][0] = trans.x;
		matTrans.m[3][1] = trans.y;
		matTrans.m[3][2] = trans.z;

		return matTrans;
	}

	//���[���h�s��𐶐�
	void WorldForm(Matrix4& matWorld, WorldTransform& worldTransform)
	{
		// matWorld_�ɃX�P�[�����O�{�����|���Z
		matWorld *= ScalingForm(worldTransform.scale_);

		// matWorld_��Z����]�s����|���Z
		matWorld *= RotationForm(worldTransform.rotation_);

		// matWorld_�Ɉړ��ʂ��|���Z
		matWorld *= TransferForm(worldTransform.translation_);
	}

	Matrix4 WorldForm(WorldTransform& worldTransform)
	{
		Matrix4 matWorld = IdentityMatrix();

		// matWorld_�ɃX�P�[�����O�{�����|���Z
		matWorld *= ScalingForm(worldTransform.scale_);

		// matWorld_�ɉ�]�s����|���Z
		matWorld *= RotationForm(worldTransform.rotation_);

		// matWorld_�Ɉړ��ʂ��|���Z
		matWorld *= TransferForm(worldTransform.translation_);

		return matWorld;
	}

	void AffineTransformation(WorldTransform& worldTransform)
	{
		//�P�ʍs�����
		worldTransform.matWorld_ = IdentityMatrix();

		// matWorld_�ɃX�P�[�����O�{�����|���Z
		worldTransform.matWorld_ *= ScalingForm(worldTransform.scale_);

		// matWorld_�ɉ�]�s����|���Z
		worldTransform.matWorld_ *= RotationForm(worldTransform.rotation_);

		// matWorld_�Ɉړ��ʂ��|���Z
		worldTransform.matWorld_ *= TransferForm(worldTransform.translation_);

		//�s��̓]��
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

	//���[���h���W���擾
	Vector3 GetWorldPosition(WorldTransform& worldTransform_)
	{
		//���[���h���W������ϐ�
		Vector3 worldPos;
		//���[���h�s��̕��s�ړ��������擾(���[���h���W)
		worldPos.x = worldTransform_.matWorld_.m[3][0];
		worldPos.y = worldTransform_.matWorld_.m[3][1];
		worldPos.z = worldTransform_.matWorld_.m[3][2];

		return worldPos;
	}

} // namespace MyMath