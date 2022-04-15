#pragma once

#include "LACoreLinkStd.h"
#include "CoreStandardLink.h"

const int MAX_ANIMATION_COUNT = 3;

struct SceneData
{
	int iFirstFrame;
	int iLastFrame;
	int iFrameSpeed;
	int iTickSecPerFrame;

	SceneData()
	{
		iFirstFrame = 0;
		iLastFrame = 0;
		iFrameSpeed = 0;
		iTickSecPerFrame = 0;
	}
};

enum class E_AnimationType
{
	AnimatinScale = 0,
	AnimatinRotation,
	AnimatinTransformation,
};

struct AnimationTrack
{
	int Tick;
	D3DXVECTOR3 Pos;
	D3DXQUATERNION Axis;

	AnimationTrack()
	{
		Tick = 0;
		ZeroMemory(&Pos, sizeof(D3DXVECTOR3));
		ZeroMemory(&Axis, sizeof(D3DXQUATERNION));
	}
};

class LAAnimaionData
{
public:
	std::vector<AnimationTrack> BaseAniData[MAX_ANIMATION_COUNT];
	std::vector<AnimationTrack> AnimationTrack[MAX_ANIMATION_COUNT];
public:
	LAAnimaionData();
	~LAAnimaionData();
};

