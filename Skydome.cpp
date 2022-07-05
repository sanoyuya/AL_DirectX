#include "Skydome.h"
#include<cassert>
#include"MathUtility.h"
using namespace MathUtility;

void Skydome::Initialize(Model* model) {
	//NULL�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;

	worldTransform_.scale_ = { 100,100,100 };

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	MatrixCalculation(worldTransform_);
	worldTransform_.TransferMatrix();
}

void Skydome::Update() {

}

void Skydome::Draw(ViewProjection& viewProjection_){
	//3D���f���̕`��
	model_->Draw(worldTransform_, viewProjection_);
}
