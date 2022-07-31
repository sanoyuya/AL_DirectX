#pragma once
#include "DebugText.h"
#include "MathUtility.h"
#include "MyMath.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
/// <summary>
/// ���[���J����
/// </summary>
class RailCamera {
public:
	RailCamera();
	~RailCamera();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(const Vector3& position, const Vector3& rotaion);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(Vector3& move, Vector3& rot);

	/// <summary>
	/// �r���[�v���W�F�N�V����������
	/// </summary>
	ViewProjection GetViewProjection();

	/// <summary>
	/// ���[���h�ϊ��f�[�^�̃A�h���X������
	/// </summary>
	WorldTransform* GetWorldTransformPtr();

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;
	//�f�o�b�O�p�\��
	DebugText* debugText_ = nullptr;
};