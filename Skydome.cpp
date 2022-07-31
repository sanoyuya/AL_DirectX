#include "Skydome.h"

void Skydome::Initialize() {
	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
	//�X�P�[����ݒ�
	worldTransform_.scale_ = { 250.0f, 250.0f, 250.0f };
	//�X�P�[�����O�s��
	worldTransform_.matWorld_ = myMath::ScalingForm(worldTransform_.scale_);
	//�s��̓]��
	worldTransform_.TransferMatrix();

	// 3D���f������
	model_ = Model::CreateFromOBJ("Skydome", true);
}

void Skydome::Update() {}

void Skydome::Draw(ViewProjection& viewProjection) {
	//���f���̕`��
	model_->Draw(worldTransform_, viewProjection);
}