#include "Enemy.h"
#include<cassert>
#include"MathUtility.h"
using namespace MathUtility;

void Enemy::Initialize(Model* model, const Vector3& position)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	enemyModel_ = model;

	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("oityan.jpg");

	//���[���h�ϊ��̏�����
	enemyWorldTransform_.Initialize();
	//�����Ŏ󂯎�����������W���Z�b�g
	enemyWorldTransform_.translation_ = position;
}

void Enemy::Update()
{
	Move();
	ApproachPhase();
}

void Enemy::Move()
{
	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };

	float moveSpeed = 0.2f;
	switch (phase_) {
	case Phase::Approach:
	default:
		ApproachPhase();
		break;
	case Phase::Leave:
		LeavePhase();
		break;
	}

	MatrixCalculation(enemyWorldTransform_);
	enemyWorldTransform_.TransferMatrix();
}

void Enemy::ApproachPhase()
{
	float moveSpeed = 0.2f;
	//�ړ�(�x�N�g�����Z)
	enemyWorldTransform_.translation_ += { 0.0f, 0.0f, -moveSpeed };
	//�K��̈ʒu�ɓ��B�����痣�E
	if (enemyWorldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::LeavePhase()
{
	float moveSpeed = 0.2f;
	//�ړ�(�x�N�g�����Z)
	enemyWorldTransform_.translation_ += {-moveSpeed, moveSpeed, -moveSpeed };
}



void Enemy::Draw(ViewProjection& viewProjection)
{
	//3D���f���̕`��
	enemyModel_->Draw(enemyWorldTransform_, viewProjection, textureHandle_);
}
