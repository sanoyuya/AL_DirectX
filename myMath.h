#pragma once
#include "Matrix4.h"
#include "Vector3.h"
#include "WorldTransform.h"
#include <cmath>

namespace myMath
{

	//�~����
	const float PI = 3.1415926535f;

	const float PI2 = PI * 2;

	/// <summary>
	/// �x���@����ʓx�@��
	/// </summary>
	/// <param name="angle">�p�x(�x���@)</param>
	float ChangeDira(const float& angle);

	/// <summary>
	/// �ʓx�@����x���@��
	/// </summary>
	/// <param name="angle">�p�x(�ʓx�@)</param>
	float ChangeRadi(const float& angle);

	/// <summary>
	/// �P�ʍs��𐶐�
	/// </summary>
	/// <returns>�P�ʍs��</returns>
	Matrix4 IdentityMatrix();

	/// <summary>
	/// �X�P�[�����O�s�񐶐�
	/// </summary>
	/// <param name="matScal"> : �X�P�[�����O�s��(�P�ʍs��)</param>
	/// <param name="scal"> : X,Y,Z�̃X�P�[�����O�ݒ�</param>
	void ScalingForm(Matrix4& matScal, Vector3& scal);

	/// <summary>
	/// �X�P�[�����O�s�񐶐�
	/// </summary>
	/// <param name="scal"> : X,Y,Z�̃X�P�[�����O�ݒ�</param>
	Matrix4 ScalingForm(Vector3& scale);

	/// <summary>
	/// X����]�s��𐶐�
	/// </summary>
	/// <param name="matRotX"> : X����]�s��(�P�ʍs��)</param>
	/// <param name="angle"> : ��]������p�x(���W�A��)</param>
	void RotationXForm(Matrix4& matRotX, float angle);

	/// <summary>
	/// X����]�s��𐶐�
	/// </summary>
	/// <param name="angle"> : ��]������p�x(���W�A��)</param>
	Matrix4 RotationXForm(float angle);

	/// <summary>
	/// Y����]�s��𐶐�
	/// </summary>
	/// <param name="matRotY"> : Y����]�s��(�P�ʍs��)</param>
	/// <param name="angle"> : ��]������p�x(���W�A��)</param>
	void RotationYForm(Matrix4& matRotY, float angle);

	/// <summary>
	/// Y����]�s��𐶐�
	/// </summary>
	/// <param name="angle"> : ��]������p�x(���W�A��)</param>
	Matrix4 RotationYForm(float angle);

	/// <summary>
	/// Z����]�s��𐶐�
	/// </summary>
	/// <param name="matRotZ"> : Z����]�s��(�P�ʍs��)</param>
	/// <param name="angle"> : ��]������p�x(���W�A��)</param>
	void RotationZForm(Matrix4& matRotZ, float angle);

	/// <summary>
	/// Z����]�s��𐶐�
	/// </summary>
	/// <param name="angle"> : ��]������p�x(���W�A��)</param>
	Matrix4 RotationZForm(float angle);

	/// <summary>
	/// ��]�s��𐶐�
	/// </summary>
	/// <param name="matRot"> : ��]�s��(�P�ʍs��)</param>
	/// <param name="rotaion"> : X,Y,Z���̉�]������p�x(���W�A��)</param>
	void RotationForm(Matrix4& matRot, Vector3& rotaion);

	/// <summary>
	/// ��]�s��𐶐�
	/// </summary>
	/// <param name="rotaion"> : X,Y,Z���̉�]������p�x(���W�A��)</param>
	Matrix4 RotationForm(Vector3& rotaion);

	/// <summary>
	/// ���s�ړ��s��𐶐�
	/// </summary>
	/// /// <param name="matTrans"> : ���s�ړ��s��(�P�ʍs��)</param>
	/// <param name="trans"> : X,Y,Z�̈ړ���</param>
	void TransferForm(Matrix4& matTrans, Vector3& trans);

	/// <summary>
	/// ���s�ړ��s��𐶐�
	/// </summary>
	/// <param name="trans"> : X,Y,Z�̈ړ���</param>
	Matrix4 TransferForm(Vector3& trans);

	/// <summary>
	/// ���[���h�s��𐶐�
	/// </summary>
	/// /// <param name="matWorld"> : ���[���h�s��𐶐�(�P�ʍs��)</param>
	/// <param name="worldTransform"> : ���[���h�g�����X�t�H�[��</param>
	void WorldForm(Matrix4& matWorld, WorldTransform& worldTransform);

	/// <summary>
	/// ���[���h�s��𐶐�
	/// </summary>
	/// <param name="worldTransform"> : ���[���h�g�����X�t�H�[��</param>
	Matrix4 WorldForm(WorldTransform& worldTransform);

	/// <summary>
	/// �A�t�B���ϊ�
	/// </summary>
	/// <param name="worldTransform"> : ���[���h�g�����X�t�H�[��</param>
	void AffineTransformation(WorldTransform& worldTransform);

	/// <summary>
	/// �e�q�֌W�X�V
	/// </summary>
	// <param name="worldTransform"> : ���[���h�g�����X�t�H�[��</param>
	void ParenChildUpdate(WorldTransform& worldTransform);

	/// <summary>
	/// �m���������߂�
	/// </summary>
	/// <returns>�x�N�g���̒���</returns>
	float Vector3Length(Vector3 vec);

	/// <summary>
	/// ���K������
	/// </summary>
	/// <returns>���K�����ꂽ�x�N�g��</returns>
	Vector3& Vector3Normalize(Vector3& vec);


	/// <summary>
	/// �P�����Z�q
	/// </summary>
	const Vector3 Vector3Sub(const Vector3& v1, const Vector3& v2);

	/// <summary>
	/// �x�N�g���ƍs��̊|���Z
	/// </summary>
	/// <param name="vec">�x�N�g��</param>
	/// <param name="mat">�s��</param>
	/// <returns></returns>
	Vector3 VecMatMul(Vector3& vec, Matrix4& mat);

	/// <summary>
	/// ���[���h���W���擾
	/// </summary>
	/// <param name="worldTransform_"></param>
	/// <returns></returns>
	Vector3 GetWorldPosition(WorldTransform& worldTransform_);
} // namespace MyMath