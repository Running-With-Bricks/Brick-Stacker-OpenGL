#pragma once
#include "BrickStacker/Core/Graphics/Window.hpp"

namespace BrickStacker
{
	using Keycode = uint16_t;
	namespace Keys
	{
		enum : Keycode
		{
			Unknown = 0,

			Spacebar = 32,
			Apostrophe = 39,
			Comma = 44,
			Minus = 45,
			Period = 46,
			Slash = 47,
			Zero = 48,
			One = 49,
			Two = 50,
			Three = 51,
			Four = 52,
			Five = 53,
			Six = 54,
			Seven = 55,
			Eight = 56,
			Nine = 57,
			SEMICOLON = 59,
			EQUAL = 61,
			A = 65,
			B = 66,
			C = 67,
			D = 68,
			E = 69,
			F = 70,
			G = 71,
			H = 72,
			I = 73,
			J = 74,
			K = 75,
			L = 76,
			M = 77,
			N = 78,
			O = 79,
			P = 80,
			Q = 81,
			R = 82,
			S = 83,
			T = 84,
			U = 85,
			V = 86,
			W = 87,
			X = 88,
			Y = 89,
			Z = 90,
			LeftBracket = 91,
			Backslash = 92,
			RightBracket = 93,
			GraveAccent = 96,

			Esc = 256,
			Enter = 257,
			Tab = 258,
			Backspace = 259,
			Insert = 260,
			Delete = 261,
			Right = 262,
			Left = 263,
			Down = 264,
			Up = 265,
			PageUp = 266,
			PageDown = 267,
			Home = 268,
			End = 269,
			CapsLock = 280,
			ScrollLock = 281,
			NumLock = 282,
			PrintScreen = 283,
			Pause = 284,
			F1 = 290,
			F2 = 291,
			F3 = 292,
			F4 = 293,
			F5 = 294,
			F6 = 295,
			F7 = 296,
			F8 = 297,
			F9 = 298,
			F10 = 299,
			F11 = 300,
			F12 = 301,
			F13 = 302,
			F14 = 303,
			F15 = 304,
			F16 = 305,
			F17 = 306,
			F18 = 307,
			F19 = 308,
			F20 = 309,
			F21 = 310,
			F22 = 311,
			F23 = 312,
			F24 = 313,
			F25 = 314,
			KeyPadZero = 320,
			KeyPadOne = 321,
			KeyPadTwo = 322,
			KeyPadThree = 323,
			KeyPadFour = 324,
			KeyPadFive = 325,
			KeyPadSix = 326,
			KeyPadSeven = 327,
			KeyPadEight = 328,
			KeyPadNine = 329,
			KeyPadDecimal = 330,
			KeyPadDivide = 331,
			KeyPadMultiply = 332,
			KeyPadSubtract = 333,
			KeyPadAdd = 334,
			KeyPadEenter = 335,
			KeyPadEqual = 336,
			LeftShift = 340,
			LeftControl = 341,
			LeftAlt = 342,
			LeftSuper = 343,
			RightShift = 344,
			RightControl = 345,
			RightAalt = 346,
			RightSuper = 347,
			Menu = 348
		};
	}

	enum class KeyAction
	{
		Up = 0,
		Down,
		Repeat
	};

	template<typename T>
	class DisconnectableCallback
	{
	public:
		DisconnectableCallback(std::shared_ptr<T> callback, std::vector<std::shared_ptr<T>>& owner)
			: m_Owner{ owner }
		{
			m_Callback = callback;
		};
		~DisconnectableCallback() {};

		void Disconnect()
		{
			auto it = std::find(m_Owner.begin(), m_Owner.end(), m_Callback);
			if (it != m_Owner.end())
				m_Owner.erase(it);

			m_Callback = nullptr;
		}
	private:
		std::vector<std::shared_ptr<T>>& m_Owner;
		std::shared_ptr<T> m_Callback;
	};

	using KeyFunction = std::function<void(Keycode, KeyAction)>;
	using KeyCallback = DisconnectableCallback<KeyFunction>;

	class Keyboard
	{
	public:
		static Keyboard& Get()
		{
			static Keyboard keyboard{};
			return keyboard;
		}

		static bool IsKeyDown(const Keycode key);
		static KeyCallback AddCallback(const KeyFunction& callback);
		static void Setup();

		bool UpdateImGui = true;
	private:
		Keyboard() = default;
		~Keyboard() = default;
		static void KeyCallbacks(GLFWwindow* window, int key, int scancode, int action, int mods);

		std::vector<std::shared_ptr<KeyFunction>> m_Callbacks;
	};
}