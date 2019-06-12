#pragma once
#include "hzpch.h"

#define DEFAULT_POOL_ELEMENT_SIZE 128

namespace Hazel {

	template <size_t SIZE>//No use of T here since we want all objects of the same size to be grouped togther
	class MemoryPool {
	public:
		static void Init() {
			if (m_Pool == nullptr) {
				m_Capacity = DEFAULT_POOL_ELEMENT_SIZE * SIZE;
				m_Pool = new char[m_Capacity];
				char* temp = m_Pool;
				m_Offset = 0;
			}
		}

		inline static void Clear() {
			m_Offset = 0;
		}

		inline static void* Allocate() {
			if (m_Offset >= m_Capacity) {
				HZ_CORE_CRITICAL("Event Pool Overflow! offset {}, capacity {}, alloc size {}", m_Offset, m_Capacity, SIZE);
				return nullptr;
			}
			char* result = m_Pool + m_Offset;
			m_Offset += SIZE;
			return result;
		}
	private:
		static void DoubleSize() {

			char* oldPool = m_Pool;
			size_t oldCapacity = m_Capacity, newCapacity = m_Capacity * 2;
			char* newPool = new char[newCapacity];
			memcpy(newPool, oldPool, oldCapacity);

			m_Capacity = newCapacity;
			m_Pool = newPool;
			delete[] oldPool;
		}

	private:
		static char* m_Pool;
		static size_t m_Capacity;//Size of bytes of the pool
		static size_t m_Offset;//How many bytes the next object is into the pool
	};

	template<size_t SIZE>
	char* MemoryPool<SIZE>::m_Pool = nullptr;

	template<size_t SIZE>
	size_t MemoryPool<SIZE>::m_Capacity;

	template<size_t SIZE>
	size_t MemoryPool<SIZE>::m_Offset;

}