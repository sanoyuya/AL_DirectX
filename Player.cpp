#include "Player.h"

float Player::GetRadius() { return radius_; }

void Player::DrawUI() { sprite2DReticle_->Draw(); }

// �ړ�
void Player::Move() {
	//�L�����N�^�[�ړ�����
	{
		//�L�����N�^�[�ړ��x�N�g��
		Vector3 move = { 0, 0, 0 };

		const float kCharacterSpeed = 0.5f;

		//�����������ňړ��x�N�g����ύX
		if (input_->PushKey(DIK_LEFT)) {
			move = { -kCharacterSpeed, 0, 0 };
		}
		else if (input_->PushKey(DIK_RIGHT)) {
			move = { kCharacterSpeed, 0, 0 };
		}

		worldTransform_.translation_ += move;
	}

	{
		//�L�����N�^�[�ړ��x�N�g��
		Vector3 move = { 0, 0, 0 };

		const float kCharacterSpeed = 0.5f;

		//�����������ňړ��x�N�g����ύX
		if (input_->PushKey(DIK_UP)) {
			move = { 0, kCharacterSpeed, 0 };
		}
		else if (input_->PushKey(DIK_DOWN)) {
			move = { 0, -kCharacterSpeed, 0 };
		}

		worldTransform_.translation_ += move;
	}

	//�ړ�����
	{
		const float kMoveLimitX = 20.0f;
		const float kMoveLimitY = 10.0f;

		worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
		worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);

		worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
		worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);
	}
}

// ����
void Player::Rotate() {

	//��]����
	{
		//��]�̑���
		const float kChestaRotSpeed = 0.07f;

		//�����������ňړ��x�N�g����ύX
		if (input_->PushKey(DIK_F)) {
			worldTransform_.rotation_.y -= kChestaRotSpeed;
		}
		else if (input_->PushKey(DIK_D)) {
			worldTransform_.rotation_.y += kChestaRotSpeed;
		}

		//�ړ�����
		{
			const float kMoveLimitX = 20.0f;
			const float kMoveLimitY = 10.0f;

			worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
			worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);

			worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
			worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);
		}
	}
}

//�U��
void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {

		//���L�����̍��W���R�s�[
		Vector3 position = GetWorldPosition();

		//�e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity = myMath::Vector3Sub(
			myMath::GetWorldPosition(worldTransform3DReticle_), GetWorldPosition());
		velocity = myMath::Vector3Mul(myMath::Vector3Normalize(velocity), kBulletSpeed);

		// �e�𐶐����A������
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, position, velocity);

		//�e��o�^����
		bullets_.push_back(std::move(newBullet));
	}
}

void Player::Reticle3D() {
	//���@����3D���e�B�N���ւ̋���
	const float kDistancePlayerTo3DReticle = 50.0f;
	//���@����3D���e�B�N���ւ̃I�t�Z�b�g(Z+����)
	Vector3 offset{ 0, 0, 1.0f };
	//���@�̃��[���h�s��̉�]�𔽉f
	offset = myMath::VecMatMul(offset, worldTransform_.matWorld_);
	//�x�N�g���̒����𐮂���
	offset = myMath::Vector3Mul(myMath::Vector3Normalize(offset), kDistancePlayerTo3DReticle);
	// 3D���e�B�N���̍��W��ݒ�
	worldTransform3DReticle_.translation_ = myMath::Vector3Add(offset, GetWorldPosition());
	myMath::AffineTransformation(worldTransform3DReticle_);
}

void Player::Reticle2D(const ViewProjection& viewProjection) {
	Vector3 positionReticle = myMath::GetWorldPosition(worldTransform3DReticle_);

	float winH = static_cast<float>(WinApp::GetInstance()->kWindowHeight / 2);
	float winW = static_cast<float>(WinApp::GetInstance()->kWindowWidth / 2);

	//�r���[�|�[�g�s��
	Matrix4 matViewport = { winW, 0, 0, 0, 0, -winH, 0, 0, 0, 0, 1, 0, winW, winH, 0, 1 };

	//�r���[�s��ƃv���W�F�N�V�����s��A�r���[�|�[�g�s�����������
	Matrix4 matViewProjectionViewport =
		viewProjection.matView * viewProjection.matProjection * matViewport;

	//���[���h���X�N���[�����W�ϊ�(������3D����2D�ɂȂ�)
	positionReticle = myMath::VecMatMatMulWdiv(positionReticle, matViewProjectionViewport);

	//�X�v���C�g�̃��e�B�N���ɍ��W�ݒ�
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
}

// ������
void Player::Initialize(
	std::shared_ptr<Model> model, uint32_t textureHandle, WorldTransform* parent,
	const Vector3& position) {
	// NUUL�|�C���^�v�`�F�b�N
	assert(model);
	assert(parent);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	texturehandle_ = textureHandle;
	worldTransform_.parent_ = parent;

	//�V���O���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.translation_ = position;
	worldTransform_.Initialize();

	// 3D���e�B�N���̃��[���h�g�����X�t�H�[��������
	worldTransform3DReticle_.Initialize();

	//���e�B�N���p�e�N�X�`���擾
	uint32_t textureReticle = TextureManager::Load("reticle.png");
	//�X�v���C�g����
	sprite2DReticle_.reset(Sprite::Create(
		textureReticle,
		{ static_cast<float>(WinApp::GetInstance()->kWindowWidth / 2),
		 static_cast<float>(WinApp::GetInstance()->kWindowHeight / 2) },
		{ 1.0f, 1.0f, 1.0f, 1.0f }, { 0.5f, 0.5f }));
}

//�X�V����
void Player::Update(const ViewProjection& viewProjection) {

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { return bullet->IsDead(); });

	//�ړ�
	Move();

	//����
	Rotate();

	//���[���h�s��v�Z
	myMath::ParenChildUpdate(worldTransform_);

	//���e�B�N���v�Z
	Reticle3D();

	Reticle2D(viewProjection);

	//�U��
	Attack();

	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//�f�o�b�O�p�\��
	debugText_->SetPos(0, 0);
	debugText_->Printf(
		"Root:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
		worldTransform_.translation_.z);

	debugText_->SetPos(0, 30);
	debugText_->Printf("Rot:%f", worldTransform_.rotation_.y);
}

// �`��
void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, texturehandle_);
	// 3D���e�B�N����`��
	model_->Draw(worldTransform3DReticle_, viewProjection, texturehandle_);

	//�e�`��
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

//���[���h���W���擾
Vector3 Player::GetWorldPosition() {
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::OnCollision() {}