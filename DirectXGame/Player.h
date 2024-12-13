#pragma once
#include "AABB.h"
#include "PlayerBullet.h"
#include <3d/Camera.h>
#include <3d/Model.h>
#include <KamataEngine.h>
#include <list>

namespace KamataEngine {
class Input;
};

class Player {
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& pos);
	void Update();
	void Draw();
	~Player();
	void Attack();
	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	KamataEngine::Vector3 GetWorldPosition();

	AABB GetAABB();

	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	static inline const float kWidth = 1.0f;
	static inline const float kHeight = 1.0f;

private:
	bool isDead_ = false;

	KamataEngine::WorldTransform worldtransfrom_;

	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Input* input_ = nullptr;

	KamataEngine::Camera* camera_ = nullptr;

	KamataEngine::Model* modelbullet_ = nullptr;

	// 弾
	std::list<PlayerBullet*> bullets_;
};