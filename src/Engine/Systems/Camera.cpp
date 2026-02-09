#include "Engine/Systems/Camera.h"

#include <algorithm>
#include <cmath>

void Camera::SetTarget(float x, float y)
{
	TargetX = x;
	TargetY = y;
}

void Camera::ToggleZoomOut()
{
	bIsZoomedOut = !bIsZoomedOut;
}

void Camera::Update(float deltaTime, sf::View& inOutView, float baseViewWidth, float baseViewHeight)
{
	// 부드러운 카메라 추적, 보간 계수 t	
	const float t = 1.0f - std::exp(-SmoothSpeed * deltaTime);
	CurrentX += (TargetX - CurrentX) * t;
	CurrentY += (TargetY - CurrentY) * t;

	// 부드러운 줌 전환
	const float targetZoom = bIsZoomedOut ? ZoomedOutZoom : NormalZoom;
	const float zoomT = 1.0f - std::exp(-ZoomSpeed * deltaTime);
	CurrentZoom += (targetZoom - CurrentZoom) * zoomT;

	// 카메라 보간 결과를 즉시 뷰에 반영
	const float safeZoom = std::max(0.001f, CurrentZoom);
	inOutView.setSize({ baseViewWidth / safeZoom, baseViewHeight / safeZoom });
	inOutView.setCenter({ CurrentX, CurrentY });
}