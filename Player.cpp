#include "Player.h"
#include<cassert>
#include"MathUtility.h"
using namespace MathUtility;

void Player::Initialize(Model* model, uint32_t textureHandle, WorldTransform* parent, const Vector3& position)
{
	//NULLポインタチェック
	assert(model);
	assert(parent);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.parent_ = parent;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.translation_ = position;
	worldTransform_.Initialize();
}

void Player::Update()
{
	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});

	//自機移動処理
	Move();
	//自機旋回処理
	Rot();
	//ワールド行列計算
	myMath::ParenChildUpdate(worldTransform_);
	//自機攻撃処理
	Attack();

	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	debugText_->SetPos(50, 50);
	debugText_->Printf("translation_:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
}

void Player::Move() {
	//キャラクターの移動ベクトル
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

	//移動限界座標
	const float kMoveLimitX = 35.0f;
	const float kMoveLimitY = 19.0f;

	//座標移動
	worldTransform_.translation_ += move;

	//範囲を超えない処理
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
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 Player::GetWorldRotation()
{
	//ワールド座標を入れる変数
	Vector3 worldRot;
	//ワールド行列の平行移動成分を取得(ワールド座標)
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

		//自キャラの座標をコピー
		Vector3 position = GetWorldPosition();
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = VecMatMul(velocity, worldTransform_.matWorld_);

		//弾を生成し、初期化
		std::unique_ptr<PlayerBullet>newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, position, velocity);

		//弾を登録する
		bullets_.push_back(std::move(newBullet));
	}
}

void Player::Draw(ViewProjection& viewProjection_)
{
	//3Dモデルの描画
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	//弾描画
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}
}