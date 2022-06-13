#pragma once
#include "Audio.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include"PlayerBullet.h"

/// <summary>
/// ���@
/// </summary>
class Player
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	void Move();
	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection& viewProjection);
private:
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//�e
	PlayerBullet* bullet_ = nullptr;
};

