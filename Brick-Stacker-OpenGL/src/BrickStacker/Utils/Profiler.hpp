#pragma once
#include "pch.hpp"

namespace BrickStacker
{
	template<typename T, std::size_t Size>
	class CircularBuffer
	{
	public:
		CircularBuffer() { static_assert(std::is_arithmetic<T>::value, "CircularBuffer only supports numeric types"); };

		void PushBack(const T& value)
		{
			m_Buffer[m_BufferIndex] = value;
			m_BufferIndex = (m_BufferIndex + 1) % Size;
		}

		const T& operator[](std::size_t index) const
		{
			return m_Buffer[(m_BufferIndex + index) % Size];
		}

		const T& Current() const { return m_Buffer[m_BufferIndex]; };
		const T& Back() const { return m_Buffer.back(); };

		template<typename Ty = T>
		const Ty Average() const
		{
			static_assert(std::is_arithmetic<Ty>::value, "CircularBuffer only supports numeric types");
			Ty sum = std::accumulate(m_Buffer.begin(), m_Buffer.end(), Ty{});
			return sum / (Ty)m_Buffer.size();
		}

	private:
		std::array<T, Size> m_Buffer{};
		size_t m_BufferIndex{ 0 };
	};

	class Profiler
	{
	public:
		static constexpr size_t s_FPSBufferSize = 100;

		static Profiler& Get()
		{
			static Profiler profiler{};
			return profiler;
		}

		void AddFrame(float FPS)
		{
			m_FrameCount++;
			m_HighestFPS = FPS > m_HighestFPS ? FPS : m_HighestFPS;
			m_FPSBuffer.PushBack(FPS);			
		};

		const size_t FrameCount() const { return m_FrameCount; };
		const float HighestFPS() const { return m_HighestFPS; };

		const CircularBuffer<float, s_FPSBufferSize>& GetFPSBuffer() const { return m_FPSBuffer; };
	private:
		Profiler() = default;
		~Profiler() = default;

		size_t m_FrameCount = 0;
		float m_HighestFPS = 0;
		CircularBuffer<float, s_FPSBufferSize> m_FPSBuffer;
	};
}
