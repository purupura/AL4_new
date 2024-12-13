#include "Enemy.h"
#include "Player.h"
#include <cassert>
#include <d3d12.h>
#include "MathUtility.h"

Enemy::~Enemy() {

	delete modelbullet_;
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Enemy::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& pos) {
	assert(model);
	model_ = model;
	camera_ = camera;
	modelbullet_ = KamataEngine::Model::CreateFromOBJ("cube", true);
	worldtransfrom_.translation_ = pos;
	worldtransfrom_.Initialize();
}

KamataEngine::Vector3 Enemy::GetWorldPosition() {

	// ワールド座標を入れる変数
	KamataEngine::Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldtransfrom_.matWorld_.m[3][0];
	worldPos.y = worldtransfrom_.matWorld_.m[3][1];
	worldPos.z = worldtransfrom_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::Fire() {

	assert(player_);

	spawnTimer--;

	if (spawnTimer < 0.0f) {

		KamataEngine::Vector3 moveBullet = worldtransfrom_.translation_;

		// 弾の速度
		const float kBulletSpeed = -3.0f;

		KamataEngine::Vector3 velocity(0, 0, 0);

		KamataEngine::Vector3 playerWorldTransform = player_->GetWorldPosition();
		KamataEngine::Vector3 enemyWorldTransform = GetWorldPosition();
		KamataEngine::Vector3 homingBullet = enemyWorldTransform - playerWorldTransform;
		homingBullet = Normalize(homingBullet);
		velocity.x += kBulletSpeed * homingBullet.x;
		velocity.y += kBulletSpeed * homingBullet.y;
		velocity.z += kBulletSpeed * homingBullet.z;

		// 弾を生成し、初期化
		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(modelbullet_, moveBullet, velocity);

		// 弾を登録する
		bullets_.push_back(newBullet);

		spawnTimer = kFireInterval;
	}
}

void Enemy::Update() {

	// 弾更新
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

	// フェーズごとの動作
	switch (phase_) {
	case Phase::Approach:
		// Z方向へ近づく
		Fire();
		worldtransfrom_.translation_.z -= 0.3f;

		// Z位置が閾値に達したらフェーズをLeaveに変更

		if (worldtransfrom_.translation_.z <= -2.0f) {
			phase_ = Phase::Leave;
		}
		break;

	case Phase::Leave:
		// 離れる動作
		worldtransfrom_.translation_.y += 1.0f;


		break;

	default:
		break;
	}

	worldtransfrom_.updateMatrix();
}

void Enemy::Draw() {

	model_->Draw(worldtransfrom_, *camera_);

	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(*camera_);
	}
}