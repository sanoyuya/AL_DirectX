#include "EnemyBullet.h"
#include<cassert>
#include <Enemy.h>
#include "MathUtility.h"
using namespace MathUtility;

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);

	bulletModel_ = model;
	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("oityan.jpg");

	//���[���h�ϊ��̏�����
	bulletWorldTransform_.Initialize();
	//�����Ŏ󂯎�����������W���Z�b�g
	bulletWorldTransform_.translation_ = position;

	//�����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;
}

void EnemyBullet::Update()
{
	//���W���ړ�������(�t���[�����̈ړ��ʂ𑫂�����)
	bulletWorldTransform_.translation_ += velocity_;

	MatrixCalculation(bulletWorldTransform_);
	bulletWorldTransform_.TransferMatrix();

	//���Ԍo�߂Ńf�X
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::OnCollision()
{
	isDead_ = true;
}

Vector3 EnemyBullet::GetWorldPosition()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos.x = bulletWorldTransform_.translation_.x;
	worldPos.y = bulletWorldTransform_.translation_.y;
	worldPos.z = bulletWorldTransform_.translation_.z;

	return worldPos;
}

void EnemyBullet::Draw(const ViewProjection& viewProjection)
{
	bulletModel_->Draw(bulletWorldTransform_, viewProjection, textureHandle_);
}