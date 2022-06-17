#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include"EnemyBullet.h"
#include "Input.h"

/// <summary>
/// �G
/// </summary>
class Enemy
{
public:
	void Initialize(Model* model, const Vector3& position);
	void Update();
	void Move();
	void ApproachPhase();
	void LeavePhase();
	void Attack();
	void Fire();
	void ApproachPhaseInitialize();
	void Draw(ViewProjection& viewProjection);

	//���ԊԊu
	static const int kFireInterval = 60;
private:
	//���[���h�ϊ��f�[�^
	WorldTransform enemyWorldTransform_;
	//���f��
	Model* enemyModel_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//���x
	Vector3 velocity_;
	Input* input_ = nullptr;
	//�G
	std::list<std::unique_ptr<Enemy>>enemy_;

	//�s���t�F�[�Y
	enum class Phase {
		Approach,	//�ڋ�
		Leave,		//���E����
	};

	//�t�F�[�Y
	Phase phase_ = Phase::Approach;

	//�e
	std::list<std::unique_ptr<EnemyBullet>>bullets_;

	//���˃^�C�}�[
	int32_t timer = 0;
};