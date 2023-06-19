#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <iostream>
#include <dwmapi.h>
#include <d3d9.h>
#include <string>

#include "offset.h"
#include "cfg.h"
#include "driver.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dwmapi.lib")

template<class T>
class TArray
{
public:
	int Length() const
	{
		return m_nCount;
	}

	bool IsValid() const
	{
		if (m_nCount > m_nMax)
			return false;
		if (!m_Data)
			return false;
		return true;
	}

	uint64_t GetAddress() const
	{
		return m_Data;
	}

	T GetById(int i)
	{
		return read<T>(m_Data + i * 8);
	}

protected:
	uint64_t m_Data;
	uint32_t m_nCount;
	uint32_t m_nMax;
};

struct FString : private TArray<wchar_t>
{
	std::wstring ToWString() const
	{
		wchar_t* buffer = new wchar_t[m_nCount];
		read_array(m_Data, buffer, m_nCount);
		std::wstring ws(buffer);
		delete[] buffer;

		return ws;
	}

	std::string ToString() const
	{
		std::wstring ws = ToWString();
		std::string str(ws.begin(), ws.end());

		return str;
	}
};

std::string RandomString(int len)
{
	srand(time(NULL));
	std::string str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::string newstr;
	int pos;
	while (newstr.size() != len) {
		pos = ((rand() % (str.size() - 1)));
		newstr += str.substr(pos, 1);
	}
	return newstr;
}
DWORD GetVisibleColor(bool bVisible)
{

	ImColor Color;
	if (bVisible)
		Color = CFG.VisibleColor;
	else
		Color = CFG.InvisibleColor;

	return Color;
}
void move_to(float x, float y)
{
	float center_x = GameVars.ScreenWidth / 2;
	float center_y = GameVars.ScreenHeight / 2;

	float smooth = CFG.Smoothing;

	float target_x = 0.f;
	float target_y = 0.f;

	if (x != 0.f)
	{
		if (x > center_x)
		{
			target_x = -(center_x - x);
			target_x /= smooth;
			if (target_x + center_x > center_x * 2.f) target_x = 0.f;
		}

		if (x < center_x)
		{
			target_x = x - center_x;
			target_x /= smooth;
			if (target_x + center_x < 0.f) target_x = 0.f;
		}
	}

	if (y != 0.f)
	{
		if (y > center_y)
		{
			target_y = -(center_y - y);
			target_y /= smooth;
			if (target_y + center_y > center_y * 2.f) target_y = 0.f;
		}

		if (y < center_y)
		{
			target_y = y - center_y;
			target_y /= smooth;
			if (target_y + center_y < 0.f) target_y = 0.f;
		}
	}

	mouse_event(MOUSEEVENTF_MOVE, static_cast<DWORD>(target_x), static_cast<DWORD>(target_y), 0, 0);
}

D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2)
{
	D3DMATRIX pOut;
	pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
	pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
	pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
	pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
	pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
	pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
	pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
	pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
	pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
	pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
	pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
	pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
	pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
	pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
	pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
	pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

	return pOut;
}

FTransform GetBoneIndex(DWORD_PTR mesh, int index)
{
	DWORD_PTR bonearray = read<DWORD_PTR>(mesh + GameOffset.offset_bone_array);
	if(!bonearray)
		bonearray = read<DWORD_PTR>(mesh + GameOffset.offset_bone_array + 0x10);
	return read<FTransform>(bonearray + (index * 0x30));
}
bool isVisible(DWORD_PTR mesh)
{
	float fLastSubmitTime = read<float>(mesh + GameOffset.offset_last_submit_time);
	float fLastRenderTimeOnScreen = read<float>(mesh + GameOffset.offset_last_render_time);
	const float fVisionTick = 0.06f;
	bool bVisible = fLastRenderTimeOnScreen + fVisionTick >= fLastSubmitTime;
	return bVisible;
}
Vector3 GetBoneWithRotation(DWORD_PTR mesh, int id)
{
	FTransform bone = GetBoneIndex(mesh, id);
	FTransform ComponentToWorld = read<FTransform>(mesh + GameOffset.offset_component_to_world);
	D3DMATRIX Matrix;
	Matrix = MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());
	return Vector3(Matrix._41, Matrix._42, Matrix._43);
}
std::string GetNameFromFName(int key)
{
	auto chunkOffset = (UINT)((int)(key) >> 16);
	auto nameOffset = (USHORT)key;

	auto namePoolChunk = read<UINT64>(GameVars.dwProcess_Base + GameOffset.offset_g_names + ((chunkOffset + 2) * 8));
	auto entryOffset = namePoolChunk + (ULONG)(2 * nameOffset);
	auto nameEntry = read<INT16>(entryOffset);

	auto nameLength = nameEntry >> 6;
	char buff[1028];
	if ((DWORD)nameLength && nameLength > 0)
	{
		read_array(entryOffset + 2, buff, nameLength);
		buff[nameLength] = '\0';
		return std::string(buff);
	}
	else return "";
}
D3DXMATRIX Matrix(Vector3 rot, Vector3 origin)
{
	float radPitch = (rot.x * float(M_PI) / 180.f);
	float radYaw = (rot.y * float(M_PI) / 180.f);
	float radRoll = (rot.z * float(M_PI) / 180.f);

	float SP = sinf(radPitch);
	float CP = cosf(radPitch);
	float SY = sinf(radYaw);
	float CY = cosf(radYaw);
	float SR = sinf(radRoll);
	float CR = cosf(radRoll);

	D3DMATRIX matrix;
	matrix.m[0][0] = CP * CY;
	matrix.m[0][1] = CP * SY;
	matrix.m[0][2] = SP;
	matrix.m[0][3] = 0.f;

	matrix.m[1][0] = SR * SP * CY - CR * SY;
	matrix.m[1][1] = SR * SP * SY + CR * CY;
	matrix.m[1][2] = -SR * CP;
	matrix.m[1][3] = 0.f;

	matrix.m[2][0] = -(CR * SP * CY + SR * SY);
	matrix.m[2][1] = CY * SR - CR * SP * SY;
	matrix.m[2][2] = CR * CP;
	matrix.m[2][3] = 0.f;

	matrix.m[3][0] = origin.x;
	matrix.m[3][1] = origin.y;
	matrix.m[3][2] = origin.z;
	matrix.m[3][3] = 1.f;

	return matrix;
}
struct FMinimalViewInfo
{
	Vector3 Location; // 0x00(0x0c)
	Vector3 Rotation; // 0x0c(0x0c)
	float FOV; // 0x18(0x04)
	float DesiredFOV; // 0x1c(0x04)
	float OrthoWidth; // 0x20(0x04)
	float OrthoNearClipPlane; // 0x24(0x04)
	float OrthoFarClipPlane; // 0x28(0x04)
	float AspectRatio; // 0x2c(0x04)
};

struct FCameraCacheEntry
{
	float Timestamp; // 0x00(0x04)
	char pad_4[0xc]; // 0x04(0x0c)
	FMinimalViewInfo POV; // 0x10(0x5e0)
};

D3DMATRIX to_matrix(Vector3 rot, Vector3 origin = Vector3(0, 0, 0))
{
	float radpitch = (rot.x * M_PI / 180);
	float radyaw = (rot.y * M_PI / 180);
	float radroll = (rot.z * M_PI / 180);
	float sp = sinf(radpitch);
	float cp = cosf(radpitch);
	float sy = sinf(radyaw);
	float cy = cosf(radyaw);
	float sr = sinf(radroll);
	float cr = cosf(radroll);
	D3DMATRIX matrix{};
	matrix.m[0][0] = cp * cy;
	matrix.m[0][1] = cp * sy;
	matrix.m[0][2] = sp;
	matrix.m[0][3] = 0.f;
	matrix.m[1][0] = sr * sp * cy - cr * sy;
	matrix.m[1][1] = sr * sp * sy + cr * cy;
	matrix.m[1][2] = -sr * cp;
	matrix.m[1][3] = 0.f;
	matrix.m[2][0] = -(cr * sp * cy + sr * sy);
	matrix.m[2][1] = cy * sr - cr * sp * sy;
	matrix.m[2][2] = cr * cp;
	matrix.m[2][3] = 0.f;
	matrix.m[3][0] = origin.x;
	matrix.m[3][1] = origin.y;
	matrix.m[3][2] = origin.z;
	matrix.m[3][3] = 1.f;
	return matrix;
}


Vector3 ProjectWorldToScreen(Vector3 world_location)
{
	Vector3 Screenlocation = Vector3(0, 0, 0);

	auto camera = read<uintptr_t>(GameVars.local_player_controller + GameOffset.offset_camera_manager);
	auto CameraCache = read<FCameraCacheEntry>(camera + GameOffset.offset_camera_cache);

	D3DMATRIX temp_matrix = to_matrix(CameraCache.POV.Rotation);
	Vector3 vaxisx = Vector3(temp_matrix.m[0][0], temp_matrix.m[0][1], temp_matrix.m[0][2]);
	Vector3 vaxisy = Vector3(temp_matrix.m[1][0], temp_matrix.m[1][1], temp_matrix.m[1][2]);
	Vector3 vaxisz = Vector3(temp_matrix.m[2][0], temp_matrix.m[2][1], temp_matrix.m[2][2]);

	Vector3 vdelta = world_location - CameraCache.POV.Location;
	Vector3 vtransformed = Vector3(vdelta.Dot(vaxisy), vdelta.Dot(vaxisz), vdelta.Dot(vaxisx));

	if (vtransformed.z < 1.f)
		vtransformed.z = 1.f;

	auto screen_center_x = GameVars.ScreenWidth / 2.0f;
	auto screen_center_y = GameVars.ScreenHeight / 2.0f;

	Screenlocation.x = screen_center_x + vtransformed.x * ((screen_center_x / tanf(CameraCache.POV.FOV * M_PI / 360))) / vtransformed.z;
	Screenlocation.y = screen_center_y - vtransformed.y * ((screen_center_x / tanf(CameraCache.POV.FOV * M_PI / 360))) / vtransformed.z;

	return Screenlocation;
}

