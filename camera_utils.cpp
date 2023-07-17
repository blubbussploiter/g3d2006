
#include "camera.h"
#include "sounds.h"

RBX::Sound* switch3 = RBX::Sound::fromFile(GetFileInPath("/content/sounds/SWITCH3.wav"));

void RBX::Camera::lookAt(const Vector3& position)
{
	const Vector3 look = (position - cframe.translation);
	yaw = aTan2(look.x, -look.z);

	pitch = -aTan2(look.y, distance(look.x, look.z));
	CoordinateFrame frame = cframe.translation;
	frame.rotation = Matrix3::fromEulerAnglesZYX(0, -yaw, -pitch);
	cframe = frame;
}

void RBX::Camera::setFrame(const CoordinateFrame& cf)
{
	Vector3 look = cf.getLookVector();
	cframe.translation = lerp(cframe.translation, cf.translation, 0.69999998f);
	cframe.rotation = cf.rotation;
	lookAt(cframe.translation + look);
	focusPosition = cframe.translation + cframe.lookVector() * zoom;
}

CoordinateFrame RBX::Camera::getCoordinateFrame()
{
	CoordinateFrame cf;
	cf.translation = translation;
	cf.rotation = Matrix3::fromEulerAnglesZYX(0, -yaw, -pitch);
	return cf;
}

void RBX::Camera::refreshZoom(const CoordinateFrame& frame)
{
	CoordinateFrame zoomFrame = focusPosition - frame.lookVector() * zoom;
	zoomFrame.lookAt(focusPosition);
	setFrame(zoomFrame);
}

void RBX::Camera::pan(CoordinateFrame* frame, float spdX, float spdY, bool shouldLerp, float lerpTime)
{

	Vector3 pos;
	Vector3 _old;

	yaw += spdX;
	pitch += spdY;

	if (pitch > 1.4f) pitch = 1.4f;
	if (pitch < -1.4f) pitch = -1.4f;

	_old = frame->translation;

	pos = Vector3(sin(-yaw) * zoom * cos(pitch), sin(pitch) * zoom, cos(-yaw) * zoom * cos(pitch)) + focusPosition;

	if(shouldLerp)
		frame->translation = lerp(_old, pos, lerpTime);
	else
		frame->translation = pos;
	frame->lookAt(focusPosition);
}

void RBX::Camera::panLock(CoordinateFrame* frame, float spdX, float spdY)
{

	int sign = 0;


	yaw = toDegrees(yaw);
	if ((((yaw - fmod(yaw, 45)) / 45) * 45) < 0)
	{
		sign = 1;
	}
	yaw = fabs(yaw);
	yaw = ((yaw - fmod(yaw, 45)) / 45) * 45;
	yaw = toRadians(yaw);

	if (sign == 1)
	{
		yaw = yaw * -1;
	}

	yaw += spdX;
	pitch += spdY;

	if (pitch > 1.4f) pitch = 1.4f;
	if (pitch < -1.4f) pitch = -1.4f;
	frame->translation = Vector3(sin(-yaw) * zoom * cos(pitch), sin(pitch) * zoom, cos(-yaw) * zoom * cos(pitch)) + focusPosition;
	frame->lookAt(focusPosition);
}

void RBX::Camera::Zoom(short delta)
{
	
	CoordinateFrame frame = cframe;
	isZooming = 1;

	switch3->play();

	if (delta>0) { // Mouse wheel up
		CoordinateFrame zoomFrame = frame+frame.lookVector()*(zoom/3.5);
		zoom=(zoomFrame.translation-focusPosition).magnitude();
		if (zoom>CAM_ZOOM_MIN)
		{
			setFrame(zoomFrame);
			if(cameraType == Follow) tiltDown(45, 1);
		}
		else
		{
			zoom=CAM_ZOOM_MIN;
			refreshZoom(frame);
		}
	}
	else {
		CoordinateFrame zoomFrame = frame-frame.lookVector()*(zoom/3.5);
		zoom=(zoomFrame.translation-focusPosition).magnitude();
		if (zoom<CAM_ZOOM_MAX)
		{
			setFrame(zoomFrame);
			if (cameraType == Follow) tiltUp(45, 1);
		}
		else
		{
			zoom=CAM_ZOOM_MAX;
			refreshZoom(frame);
		}
	}
}
