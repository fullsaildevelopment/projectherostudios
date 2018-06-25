#pragma once
#include <type_traits>
#include <array>
#include <cassert>

namespace end
{
	template<typename T, size_t N>
	class free_list_pool
	{
		// Todo: Implement the function bodies

		using storage_type = std::aligned_storage_t<sizeof(T), alignof(T)>;

		size_t next_free = 0;

		union entry_t
		{
			storage_type storage;
			size_t next;
		};

		std::array<entry_t, N> data;

	public:
		free_list_pool()
		{
			// Initialize the free list
			for (int i = 0; i < N; ++i)
			{
				data[i].next = i + 1;
			}
		}

		template<typename...Args>	// Variadic template parameters
		T* allocate(Args&&...args)	// Variadic function parameters
		{
			// If no space, return nullptr
			if (next_free >= N)
			{
				return nullptr;
			}
			//Pop an element from the free list to allocate space

			void* space = &data[next_free];
			next_free = data[next_free].next;

			// This is an example of using placement new and forwarding the parameters
			// to the constructor of T. If no matching constructor exists, it will be a compile-time error
			return new (space) T{ std::forward<Args>(args)... };
		}

		void free(T* ptr)
		{
			// If ptr maps to a valid index in the pool, 
			// destruct it then push it on to the free list
			size_t offset = (size_t)ptr - (size_t)&data[0];
			if (index < N/*ptr maps to valid index*/)
			{
				//destruct ptr;
				((T*)&data[index].storage)->~T();
				
				//push it on to the free list;
				data[index].next = next_free;
				next_free = index;
			}
		}
	};

	template<typename T, uint16_t N>
	class sorted_pool
	{
		uint16_t active_count = 0;
		T * pool_mem = nullptr;
	public:

		// Todo : Implement the sorted_pool class
		/*
		Sorted Pool
		Pre-allocated arrays of certain sizes and/or types
		Ex: Particle particle_pool[1024];
		No active/inactive flags
		Active objects are contiguous
		Maintains a count of active elements
		“Spawn” a new element
		particle_pool[active_count++]
		“Despawn” an element (index)
		swap( particle_pool[index], particle_pool[--active_count] )
		*/


		sorted_pool()
		{
			pool_mem = new T[N];
		};
		~sorted_pool()
		{
			if (pool_mem)
			{
				delete[] pool_mem;
			}
		};

		template<typename...Args>	// Variadic template parameters
		T* allocate(Args&&...args)	// Variadic function parameters
		{
			if (active_count >= N) 
				return;
			pool_mem[active_count++] = T{ std::forward<Args>(args)... };
		}

		void dalloc_(int index)
		{
			std::swap(pool_mem[index], pool_mem[--active_count]);
		}

		T &operator[](uint16_t index)
		{
			return pool_mem[index];
		}

		uint16_t count()
		{
			return active_count;
		}

	};
}