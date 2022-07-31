#include "EnemyBullet.h"

/// <summary>
/// ������
/// </summary>
/// <param name="model">���f��</param>
/// <param name="position">�������W </param>
void EnemyBullet::Initialize(
	std::shared_ptr<Model> model, const Vector3& position, const Vector3& velocity) {

	// NULL�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	velocity_ = velocity;

	//�e�N�X�`���ǂݍ���
	texturehandle_ = TextureManager::Load("kamata.ico");

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;
	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
}

// �X�V
void EnemyBullet::Update() {
	//���W���ړ�������(1�t���[�����̈ړ�����������)
	worldTransform_.translation_ += velocity_;

	//���[���h�g�����X�t�H�[���̍X�V
	myMath::AffineTransformation(worldTransform_);

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

// �`��
void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	//���f���̕`��
	model_->Draw(worldTransform_, viewProjection, texturehandle_);
}

//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
void EnemyBullet::OnCollision() {
	//�f�X�t���O�𗧂Ă�
	isDead_ = true;
}

//���[���h���W���擾
Vector3 EnemyBullet::GetWorldPosition() {
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

float EnemyBullet::GetRadius() { return radius_; }