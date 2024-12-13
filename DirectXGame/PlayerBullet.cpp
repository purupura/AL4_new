#include "PlayerBullet.h"
#include "base/TextureManager.h"
#include <cassert>

PlayerBullet::~PlayerBullet() { model_ = nullptr; }

void PlayerBullet::OnCollision() { isDead_ = true; }

KamataEngine::Vector3 PlayerBullet::GetWorldPosition() { 	// ワールド座標を入れる変数
	KamataEngine::Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldtransfrom_.matWorld_.m[3][0];
	worldPos.y = worldtransfrom_.matWorld_.m[3][1];
	worldPos.z = worldtransfrom_.matWorld_.m[3][2];

	return worldPos;
}

void PlayerBullet::Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity) {

	assert(model);
	model_ = model;
	worldtransfrom_.translation_ = position;
	worldtransfrom_.Initialize();
	velocity_ = velocity;
}

void PlayerBullet::Update() {

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	worldtransfrom_.translation_.x += velocity_.x;
	worldtransfrom_.translation_.y += velocity_.y;
	worldtransfrom_.translation_.z += velocity_.z;

	worldtransfrom_.updateMatrix();
}

void PlayerBullet::Draw(const KamataEngine::Camera& camera) {

	// モデルの描画
	model_->Draw(worldtransfrom_, camera);
}