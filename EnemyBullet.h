#pragma once
#include "DebugText.h"
#include "Model.h"
#include "MyMath.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include <cassert>

/// <summary>
/// �G�̒e
/// </summary>
class EnemyBullet {

public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name = "model">���f��</param>
	/// <param name = "position">�������W</param>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name = "viewProjection">�r���[�v���W�F�N�V����</param>
	void Draw(const ViewProjection& viewProjection);

	///�f�X�t���O�Q�b�^�[
	bool IsDead() const;

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	/// <summary>
	/// ���[���h���W������
	/// </summary>
	Vector3 GetWorldPosition();

	/// <summary>
	/// ���a������
	/// </summary>
	float GetRadius();

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	//���f��
	Model* model_ = nullptr;

	//�e�N�X�`���n���h��
	uint32_t texturehandle_ = 0u;

	//�f�o�b�O�p�\��
	DebugText* debugText_ = nullptr;

	//���x
	Vector3 velocity_;

	//����<frm>
	static const int32_t kLifeTime = 60 * 5;

	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;
	//�f�X�t���O
	bool isDead_ = false;

	const float radius_ = 1.0f;
};

inline bool EnemyBullet::IsDead() const { return isDead_; }