#include "Player.h"
#include<cassert>
#include"MathUtility.h"
using namespace MathUtility;

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	//�r���[�v���W�F�N�V����������
	viewProjection_.Initialize();
	viewProjection_.eye.y = 20;

}

void Player::Update()
{
	Move();
	SelectMode();
	debugText_->SetPos(50, 50);
	debugText_->Printf("translation_:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
	debugText_->SetPos(50, 70);
	debugText_->Printf("mode:%d", mode);
}

void Player::Move() {

	Vector3 move(0, 0, 1);
	Vector3 front(0, 0, 2);
	Vector3 camera(0, 0, 0);

	float rotationSpeed = 0.02f;
	float moveSpeed = 0.05f;
	//���_�̈ړ��̑���
	const float kEyeSpeed = 0.02f;

	// 2�΂𒴂�����0�ɖ߂�
	viewAngle = fmodf(viewAngle, 2 * PI);

	//���_�x�N�g�����v�Z(���a1�̉~����̍��W)
	//viewProjection_.eye = { worldTransform_.translation_.x + sinf(viewAngle) * length, 20.0f,worldTransform_.translation_.x + cosf(viewAngle) * length };

	if (mode == false) {
		if (input_->PushKey(DIK_A)) {
			worldTransform_.rotation_.y -= rotationSpeed;
		}
		if (input_->PushKey(DIK_D)) {
			worldTransform_.rotation_.y += rotationSpeed;
		}
		// y����]�s�񉉎Z
		move.x = cos(worldTransform_.rotation_.y) * front.x + sin(worldTransform_.rotation_.y) * front.z;
		move.z = -sin(worldTransform_.rotation_.y) * front.x + cos(worldTransform_.rotation_.y) * front.z;

		if (input_->PushKey(DIK_W)) {
			worldTransform_.translation_.x += moveSpeed * move.x;
			worldTransform_.translation_.z += moveSpeed * move.z;
		}
		if (input_->PushKey(DIK_S)) {
			worldTransform_.translation_.x -= moveSpeed * move.x;
			worldTransform_.translation_.z -= moveSpeed * move.z;
		}
	}
	else {
		if (input_->PushKey(DIK_LEFT)) {
			viewAngle -= kEyeSpeed;
		}
		if (input_->PushKey(DIK_RIGHT)) {
			viewAngle += kEyeSpeed;
		}
	}
	

	

	

	

	worldTransform_.TransferMatrix();
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();

	MatrixCalculation(worldTransform_);
}

void Player::SelectMode() {
	if (input_->TriggerKey(DIK_Q)) {
		if (mode == false) {
			mode = true;
		}
		else {
			mode = false;
		}
	}
}

void Player::Draw()
{
	//3D���f���̕`��
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}
