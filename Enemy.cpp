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
}

void Enemy::Move()
{
	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };

	float moveSpeed = 0.2f;
	move = { 0.0f,0.0f ,-moveSpeed };

	//���W�ړ�
	enemyWorldTransform_.translation_ += move;

	MatrixCalculation(enemyWorldTransform_);
	enemyWorldTransform_.TransferMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection)
{
	//3D���f���̕`��
	enemyModel_->Draw(enemyWorldTransform_, viewProjection, textureHandle_);
}
