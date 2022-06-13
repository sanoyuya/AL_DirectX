#include "PlayerBullet.h"
#include<cassert>
#include <Player.h>
#include "MathUtility.h"
using namespace MathUtility;

void PlayerBullet::Initialize(Model* model, const Vector3& position)
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
}

void PlayerBullet::Update()
{
	MatrixCalculation(bulletWorldTransform_);
	bulletWorldTransform_.TransferMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	bulletModel_->Draw(bulletWorldTransform_, viewProjection, textureHandle_);
}
