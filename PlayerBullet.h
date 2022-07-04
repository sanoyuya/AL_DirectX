#pragma once
#include "Model.h"
#include"WorldTransform.h"

/// <summary>
/// ���@�̒e
/// </summary>
class PlayerBullet
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model* model, const Vector3& position,const Vector3& velocity);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	Vector3 GetWorldPosition();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="viewProjection">�r���[�v���W�F�N�V����</param>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// �Q�b�^�[
	/// </summary>
	bool IsDead()const { return isDead_; }

private:
	//���[���h�ϊ��f�[�^
	WorldTransform bulletWorldTransform_;
	//���f��
	Model* bulletModel_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//���x
	Vector3 velocity_;
	//����<frm>
	static const int32_t kLifeTime = 60 * 5;
	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;
	//�f�X�t���O
	bool isDead_ = false;
};

