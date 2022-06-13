#pragma once
#include "Model.h"
#include "WorldTransform.h"

/// <summary>
/// �G
/// </summary>
class Enemy
{
public:
	void Initialize(Model* model, const Vector3& position);
	void Update();
	void Move();
	void Draw(ViewProjection& viewProjection);
private:
	//���[���h�ϊ��f�[�^
	WorldTransform enemyWorldTransform_;
	//���f��
	Model* enemyModel_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//���x
	Vector3 velocity_;

	//�G
	std::list<std::unique_ptr<Enemy>>enemy_;
};

