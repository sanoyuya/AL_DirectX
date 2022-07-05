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
	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();

	//�����Ŏ󂯎�����������W���Z�b�g
	enemyWorldTransform_.translation_ = position;

	//���[���h�ϊ��̏�����
	enemyWorldTransform_.Initialize();
}

void Enemy::Update()
{
	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
		});

	Move();
	ApproachPhase();
	//�ڋ߃t�F�[�Y������
	ApproachPhaseInitialize();

	//�e�X�V
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}
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

void Enemy::Attack()
{
	//�e�̑��x
	const float kBulletSpeed = 1.0f;

	Vector3 player = player_->GetWorldPosition();
	Vector3 enemy = GetWorldPosition();
	Vector3 l = player - enemy;
	Vector3Normalize(l);
	Vector3 velocity(l * kBulletSpeed);

	//�e�𐶐����A������
	std::unique_ptr<EnemyBullet>newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(enemyModel_, enemyWorldTransform_.translation_, velocity);

	//�e��o�^����
	bullets_.push_back(std::move(newBullet));
}

void Enemy::Fire()
{
	Attack();
}

void Enemy::ApproachPhaseInitialize()
{
	//���˃^�C�}�[�J�E���g�_�E��
	timer--;
	//�w�莞�ԂɒB����
	if (timer < 0) {
		//�e�𔭎�
		Fire();
		//���˃^�C�}�[��������
		timer = kFireInterval;
	}
}



void Enemy::Draw(ViewProjection& viewProjection)
{
	//3D���f���̕`��
	enemyModel_->Draw(enemyWorldTransform_, viewProjection, textureHandle_);

	//�e�`��
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

Vector3 Enemy::GetWorldPosition()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos.x = enemyWorldTransform_.translation_.x;
	worldPos.y = enemyWorldTransform_.translation_.y;
	worldPos.z = enemyWorldTransform_.translation_.z;

	return worldPos;
}

void Enemy::OnCollision()
{
}