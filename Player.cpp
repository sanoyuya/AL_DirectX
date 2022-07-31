#include "Player.h"

float Player::GetRadius() { return radius_; }

void Player::DrawUI() { sprite2DReticle_->Draw(); }

// 移動
void Player::Move() {
	//キャラクター移動処理
	{
		//キャラクター移動ベクトル
		Vector3 move = { 0, 0, 0 };

		const float kCharacterSpeed = 0.5f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_LEFT)) {
			move = { -kCharacterSpeed, 0, 0 };
		}
		else if (input_->PushKey(DIK_RIGHT)) {
			move = { kCharacterSpeed, 0, 0 };
		}

		worldTransform_.translation_ += move;
	}

	{
		//キャラクター移動ベクトル
		Vector3 move = { 0, 0, 0 };

		const float kCharacterSpeed = 0.5f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_UP)) {
			move = { 0, kCharacterSpeed, 0 };
		}
		else if (input_->PushKey(DIK_DOWN)) {
			move = { 0, -kCharacterSpeed, 0 };
		}

		worldTransform_.translation_ += move;
	}

	//移動制限
	{
		const float kMoveLimitX = 20.0f;
		const float kMoveLimitY = 10.0f;

		worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
		worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);

		worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
		worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);
	}
}

// 旋回
void Player::Rotate() {

	//回転処理
	{
		//回転の速さ
		const float kChestaRotSpeed = 0.07f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_F)) {
			worldTransform_.rotation_.y -= kChestaRotSpeed;
		}
		else if (input_->PushKey(DIK_D)) {
			worldTransform_.rotation_.y += kChestaRotSpeed;
		}

		//移動制限
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

//攻撃
void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {

		//自キャラの座標をコピー
		Vector3 position = GetWorldPosition();

		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity = myMath::Vector3Sub(
			myMath::GetWorldPosition(worldTransform3DReticle_), GetWorldPosition());
		velocity = myMath::Vector3Mul(myMath::Vector3Normalize(velocity), kBulletSpeed);

		// 弾を生成し、初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, position, velocity);

		//弾を登録する
		bullets_.push_back(std::move(newBullet));
	}
}

void Player::Reticle3D() {
	//自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 50.0f;
	//自機から3Dレティクルへのオフセット(Z+向き)
	Vector3 offset{ 0, 0, 1.0f };
	//自機のワールド行列の回転を反映
	offset = myMath::VecMatMul(offset, worldTransform_.matWorld_);
	//ベクトルの長さを整える
	offset = myMath::Vector3Mul(myMath::Vector3Normalize(offset), kDistancePlayerTo3DReticle);
	// 3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_ = myMath::Vector3Add(offset, GetWorldPosition());
	myMath::AffineTransformation(worldTransform3DReticle_);
}

void Player::Reticle2D(const ViewProjection& viewProjection) {
	Vector3 positionReticle = myMath::GetWorldPosition(worldTransform3DReticle_);

	float winH = static_cast<float>(WinApp::GetInstance()->kWindowHeight / 2);
	float winW = static_cast<float>(WinApp::GetInstance()->kWindowWidth / 2);

	//ビューポート行列
	Matrix4 matViewport = { winW, 0, 0, 0, 0, -winH, 0, 0, 0, 0, 1, 0, winW, winH, 0, 1 };

	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4 matViewProjectionViewport =
		viewProjection.matView * viewProjection.matProjection * matViewport;

	//ワールド→スクリーン座標変換(ここで3Dから2Dになる)
	positionReticle = myMath::VecMatMatMulWdiv(positionReticle, matViewProjectionViewport);

	//スプライトのレティクルに座標設定
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
}

// 初期化
void Player::Initialize(
	std::shared_ptr<Model> model, uint32_t textureHandle, WorldTransform* parent,
	const Vector3& position) {
	// NUULポインタ」チェック
	assert(model);
	assert(parent);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	texturehandle_ = textureHandle;
	worldTransform_.parent_ = parent;

	//シングルインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.translation_ = position;
	worldTransform_.Initialize();

	// 3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();

	//レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("reticle.png");
	//スプライト生成
	sprite2DReticle_.reset(Sprite::Create(
		textureReticle,
		{ static_cast<float>(WinApp::GetInstance()->kWindowWidth / 2),
		 static_cast<float>(WinApp::GetInstance()->kWindowHeight / 2) },
		{ 1.0f, 1.0f, 1.0f, 1.0f }, { 0.5f, 0.5f }));
}

//更新処理
void Player::Update(const ViewProjection& viewProjection) {

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { return bullet->IsDead(); });

	//移動
	Move();

	//旋回
	Rotate();

	//ワールド行列計算
	myMath::ParenChildUpdate(worldTransform_);

	//レティクル計算
	Reticle3D();

	Reticle2D(viewProjection);

	//攻撃
	Attack();

	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//デバッグ用表示
	debugText_->SetPos(0, 0);
	debugText_->Printf(
		"Root:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
		worldTransform_.translation_.z);

	debugText_->SetPos(0, 30);
	debugText_->Printf("Rot:%f", worldTransform_.rotation_.y);
}

// 描画
void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, texturehandle_);
	// 3Dレティクルを描画
	model_->Draw(worldTransform3DReticle_, viewProjection, texturehandle_);

	//弾描画
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

//ワールド座標を取得
Vector3 Player::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::OnCollision() {}