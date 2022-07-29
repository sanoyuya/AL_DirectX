#include "Player.h"
#include<cassert>
#include"MathUtility.h"
using namespace MathUtility;

void Player::Initialize(Model* model, uint32_t textureHandle, WorldTransform* parent, const Vector3& position)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);
	assert(parent);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.parent_ = parent;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.translation_ = position;
	worldTransform_.Initialize();
}

void Player::Update()
{
	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});

	//���@�ړ�����
	Move();
	//���@���񏈗�
	Rot();
	//���[���h�s��v�Z
	myMath::ParenChildUpdate(worldTransform_);
	//���@�U������
	Attack();

	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	debugText_->SetPos(50, 50);
	debugText_->Printf("translation_:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
}

void Player::Move() {
	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };

	float moveSpeed = 0.2f;
	if (input_->PushKey(DIK_A)) {
		move = { -moveSpeed,0.0f ,0.0f };
	}if (input_->PushKey(DIK_D)) {
		move = { moveSpeed,0.0f ,0.0f };
	}if (input_->PushKey(DIK_W)) {
		move = { 0.0f,moveSpeed ,0.0f };
	}if (input_->PushKey(DIK_S)) {
		move = { 0.0f,-moveSpeed ,0.0f };
	}

	//�ړ����E���W
	const float kMoveLimitX = 35.0f;
	const float kMoveLimitY = 19.0f;

	//���W�ړ�
	worldTransform_.translation_ += move;

	//�͈͂𒴂��Ȃ�����
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);

	MatrixCalculation(worldTransform_);
	worldTransform_.TransferMatrix();
}

void Player::Rot()
{
	float rotSpeed = 0.02f;

	if (input_->PushKey(DIK_LEFT)) {
		worldTransform_.rotation_.y -= rotSpeed;
	}if (input_->PushKey(DIK_RIGHT)) {
		worldTransform_.rotation_.y += rotSpeed;
	}
}

Vector3 Player::GetWorldPosition()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 Player::GetWorldRotation()
{
	//���[���h���W������ϐ�
	Vector3 worldRot;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldRot.x = worldTransform_.rotation_.x;
	worldRot.y = worldTransform_.rotation_.y;
	worldRot.z = worldTransform_.rotation_.z;

	return worldRot;
}

void Player::OnCollision()
{
}

void Player::Attack()
{
	if (input_->TriggerKey(DIK_SPACE)) {

		//���L�����̍��W���R�s�[
		Vector3 position = GetWorldPosition();
		//�e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		velocity = VecMatMul(velocity, worldTransform_.matWorld_);

		//�e�𐶐����A������
		std::unique_ptr<PlayerBullet>newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, position, velocity);

		//�e��o�^����
		bullets_.push_back(std::move(newBullet));
	}
}

void Player::Draw(ViewProjection& viewProjection_)
{
	//3D���f���̕`��
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	//�e�`��
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}
}