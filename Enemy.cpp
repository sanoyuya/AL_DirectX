#include "Enemy.h"
#include "GameScene.h"
#include <cassert>

// ������
void Enemy::Initialize(std::shared_ptr<Model> model, uint32_t textureHandle, const Vector3& position) {
	// NUUL�|�C���^�v�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	texturehandle_ = textureHandle;

	//�V���O���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;
	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	//�ڋ߃t�F�[�Y������
	approachPhaseInt();
}

// �X�V����
void Enemy::Update() {

	//�ړ�����
	switch (phase_) {
	case Phase::Approach: //�ڋ߃t�F�[�Y
		ApproachVelocity();
		break;
	case Phase::Leave: //���E�t�F�[�Y
		LeaveVelocity();
		break;
	}
	//���[���h�s��v�Z
	myMath::AffineTransformation(worldTransform_);
}

// �`��
void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, texturehandle_);
}

// �e����
void Enemy::Fire() {

	assert(player_);

	//�e�̑��x
	const float kBulletSpeed = 1.0f;

	//�v���C���[�̃��[���h���W���擾
	Vector3 playerPos = player_->GetWorldPosition();
	//�G�L�����̃��[���h���W���擾
	Vector3 enemyPos = GetWorldPosition();
	//�G�L����->���L�����̍����x�N�g�������߂�
	Vector3 velocity = myMath::Vector3Sub(playerPos, enemyPos);
	//�x�N�g���̐��K��
	velocity = myMath::Vector3Normalize(velocity);
	//�x�N�g���̒������A�����ɍ��킹��
	velocity *= kBulletSpeed;

	// �e�𐶐����A������
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	//�e��o�^����
	gameScene_->AddEnemyBullet(newBullet);
}

//�ڋ߃t�F�[�Y������
void Enemy::approachPhaseInt() {
	//���˃^�C�}�[��������
	fileTimer_ = kFireInterval;
}

// ���[���h���W������
Vector3 Enemy::GetWorldPosition() {
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::OnCollision() { isDead_ = true; }

float Enemy::GetRadius() { return radius_; }

// �ڋ߃t�F�[�Y�ړ�����
void Enemy::ApproachVelocity() {

	//���˃^�C�}�[�J�E���g�_�E��
	fileTimer_--;
	//�w�莞�ԂɒB����
	if (fileTimer_ == 0) {
		//�e�̔���
		Fire();
		//���˃^�C�}�[��������
		fileTimer_ = kFireInterval;
	}

	//�ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += approachVelocity_;
	//����̈ʒu�ɓ��������痣�E
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

//���E�t�F�[�Y�ړ�����
void Enemy::LeaveVelocity() {
	//�ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += leaveVelocity_;
}