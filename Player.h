#pragma once
#include <cassert>
#include <list>
#include <memory>

#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WinApp.h"
#include "WorldTransform.h"

#include "myMath.h"
#include "PlayerBullet.h"

/// <summary>
/// ���L����
/// </summary>
class Player {
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="textureHandle">�e�N�X�`���n���h��</param>
	void Initialize(
		std::shared_ptr<Model> model, uint32_t textureHandle, WorldTransform* parent_,
		const Vector3& position);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(const ViewProjection& viewProjection);

	/// <summary>
	/// �`��
	/// </summary>
	/// /// <param name="viewProjection">�r���[�v���W�F�N�V����(�Q�Ɠn��)</param>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// ���[���h���W������
	/// </summary>
	Vector3 GetWorldPosition();

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	//�e���X�g���擾
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullet() { return bullets_; }

	/// <summary>
	/// ���a������
	/// </summary>
	float GetRadius();

	/// <summary>
	/// UI�`��
	/// </summary>
	void DrawUI();

	Player() = default;
	~Player() = default;

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	//���f��
	std::shared_ptr<Model> model_;

	//�e�N�X�`���n���h��
	uint32_t texturehandle_ = 0u;

	//�L�[�{�[�h����
	Input* input_ = nullptr;

	//�f�o�b�O�p�\��
	DebugText* debugText_ = nullptr;

	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	//���a
	const float radius_ = 1.0f;

	// 3D���e�B�N���p���[���h�g�����X�t�H�[��
	WorldTransform worldTransform3DReticle_;

	// 2D���e�B�N���p�X�v���C�g
	std::unique_ptr<Sprite> sprite2DReticle_;

	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();

	/// <summary>
	/// ����
	/// </summary>
	void Rotate();

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

	/// <summary>
	/// 3D���e�B�N���̌v�Z
	/// </summary>
	void Reticle3D();

	/// <summary>
	/// 2D���e�B�N���̌v�Z
	/// </summary>
	void Reticle2D(const ViewProjection& viewProjection);
};