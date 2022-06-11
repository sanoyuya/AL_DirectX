#pragma once
#include "Audio.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"

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

	void SelectMode();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();
private:
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//Q�L�[�ő���؂�ւ�
	bool mode = false;
	float viewAngle = 3.141592654f;
	float length = 30.0f;
};

