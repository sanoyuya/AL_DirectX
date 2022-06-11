#pragma once

#include "Vector2.h"
#include <Windows.h>
#include <array>
#include <vector>
#include <wrl.h>

#include <XInput.h>
#define DIRECTINPUT_VERSION 0x0800 // DirectInput�̃o�[�W�����w��
#include <dinput.h>

/// <summary>
/// ����
/// </summary>
class Input {

public: // �C���i�[�N���X
	struct MouseMove {
		LONG lX;
		LONG lY;
		LONG lZ;
	};

public:
	enum class PadType {
		DirectInput,
		XInput,
	};

	// variant��C++17����
	union State {
		XINPUT_STATE xInput_;
		DIJOYSTATE2 directInput_;
	};

	struct Joystick {
		Microsoft::WRL::ComPtr<IDirectInputDevice8> device_;
		int32_t deadZoneL_;
		int32_t deadZoneR_;
		PadType type_;
		State state_;
		State statePre_;
	};

public: // �����o�֐�

	static Input* GetInstance();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �L�[�̉������`�F�b�N
	/// </summary>
	/// <param name="keyNumber">�L�[�ԍ�( DIK_0 ��)</param>
	/// <returns>������Ă��邩</returns>
	bool PushKey(BYTE keyNumber) const;

	/// <summary>
	/// �L�[�̃g���K�[���`�F�b�N
	/// </summary>
	/// <param name="keyNumber">�L�[�ԍ�( DIK_0 ��)</param>
	/// <returns>�g���K�[��</returns>
	bool TriggerKey(BYTE keyNumber) const;

	/// <summary>
	/// �S�L�[���擾
	/// </summary>
	/// <param name="keyStateBuf">�S�L�[���</param>
	const std::array<BYTE, 256>& GetAllKey() const { return key_; }

	/// <summary>
	/// �S�}�E�X���擾
	/// </summary>
	/// <returns>�}�E�X���</returns>
	const DIMOUSESTATE2& GetAllMouse() const;

	/// <summary>
	/// �}�E�X�̉������`�F�b�N
	/// </summary>
	/// <param name="buttonNumber">�}�E�X�{�^���ԍ�(0:��,1:�E,2:��,3~7:�g���}�E�X�{�^��)</param>
	/// <returns>������Ă��邩</returns>
	bool IsPressMouse(int32_t mouseNumber) const;

	/// <summary>
	/// �}�E�X�̃g���K�[���`�F�b�N�B�������u�Ԃ���true�ɂȂ�
	/// </summary>
	/// <param name="buttonNumber">�}�E�X�{�^���ԍ�(0:��,1:�E,2:��,3~7:�g���}�E�X�{�^��)</param>
	/// <returns>�g���K�[��</returns>
	bool IsTriggerMouse(int32_t buttonNumber) const;

	/// <summary>
	/// �}�E�X�ړ��ʂ��擾
	/// </summary>
	/// <returns>�}�E�X�ړ���</returns>
	MouseMove GetMouseMove();

	/// <summary>
	/// �z�C�[���X�N���[���ʂ��擾����
	/// </summary>
	/// <returns>�z�C�[���X�N���[���ʁB�����ɉ񂵂���+�BWindows�̐ݒ�ŋt�ɂ��Ă���t</returns>
	int32_t GetWheel() const;

	/// <summary>
	/// �}�E�X�̈ʒu���擾����i�E�B���h�E���W�n�j
	/// </summary>
	/// <returns>�}�E�X�̈ʒu</returns>
	const Vector2& GetMousePosition() const;

	/// <summary>
	/// ���݂̃W���C�X�e�B�b�N��Ԃ��擾����
	/// </summary>
	/// <param name="stickNo">�W���C�X�e�B�b�N�ԍ�</param>
	/// <param name="out">���݂̃W���C�X�e�B�b�N���</param>
	/// <returns>�������擾�ł�����</returns>
	bool GetJoystickState(int32_t stickNo, DIJOYSTATE2& out) const;

	/// <summary>
	/// �O��̃W���C�X�e�B�b�N��Ԃ��擾����
	/// </summary>
	/// <param name="stickNo">�W���C�X�e�B�b�N�ԍ�</param>
	/// <param name="out">�O��̃W���C�X�e�B�b�N���</param>
	/// <returns>�������擾�ł�����</returns>
	bool GetJoystickStatePrevious(int32_t stickNo, DIJOYSTATE2& out) const;

	/// <summary>
	/// ���݂̃W���C�X�e�B�b�N��Ԃ��擾����
	/// </summary>
	/// <param name="stickNo">�W���C�X�e�B�b�N�ԍ�</param>
	/// <param name="out">���݂̃W���C�X�e�B�b�N���</param>
	/// <returns>�������擾�ł�����</returns>
	bool GetJoystickState(int32_t stickNo, XINPUT_STATE& out) const;

	/// <summary>
	/// �O��̃W���C�X�e�B�b�N��Ԃ��擾����
	/// </summary>
	/// <param name="stickNo">�W���C�X�e�B�b�N�ԍ�</param>
	/// <param name="out">�O��̃W���C�X�e�B�b�N���</param>
	/// <returns>�������擾�ł�����</returns>
	bool GetJoystickStatePrevious(int32_t stickNo, XINPUT_STATE& out) const;

	/// <summary>
	/// �f�b�h�]�[����ݒ肷��
	/// </summary>
	/// <param name="stickNo">�W���C�X�e�B�b�N�ԍ�</param>
	/// <param name="deadZoneL">�f�b�h�]�[�����X�e�B�b�N 0~32768</param>
	/// <param name="deadZoneR">�f�b�h�]�[���E�X�e�B�b�N 0~32768</param>
	/// <returns>�������擾�ł�����</returns>
	void SetJoystickDeadZone(int32_t stickNo, int32_t deadZoneL, int32_t deadZoneR);

	/// <summary>
	/// �ڑ�����Ă���W���C�X�e�B�b�N�����擾����
	/// </summary>
	/// <returns>�ڑ�����Ă���W���C�X�e�B�b�N��</returns>
	size_t GetNumberOfJoysticks();

private:
	static BOOL CALLBACK
		EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext) noexcept;
	Input() = default;
	~Input();
	Input(const Input&) = delete;
	const Input& operator=(const Input&) = delete;
	void SetupJoysticks();

private: // �����o�ϐ�
	Microsoft::WRL::ComPtr<IDirectInput8> dInput_;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> devKeyboard_;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> devMouse_;
	std::vector<Joystick> devJoysticks_;
	std::array<BYTE, 256> key_;
	std::array<BYTE, 256> keyPre_;
	DIMOUSESTATE2 mouse_;
	DIMOUSESTATE2 mousePre_;
	HWND hwnd_;
	Vector2 mousePosition_;
};