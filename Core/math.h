#include <stdint.h>
#include <algorithm>
#include <bit>
#include <chrono>

#ifdef __CUDA_ARCH__
#   define bitswap(X) __brevll(X)
#   define countr_zero(X) __ffsll(X) - 1
#   define countl_zero(X) __clzll(X)
#   define popcount(X) __popcll(X)
#   define HOST_DEVICE __inline__ __device__ __host__
#   define sync_threads __syncthreads
#	define HOST_DEVICE_LOOKUP static __constant__
#else
#include <immintrin.h>
#   define bitswap(X) bit_reverse(X)
#   define countr_zero(X) _tzcnt_u64(X)
#   define countl_zero(X) _lzcnt_u64(X)
#   define popcount(X) __popcnt64(X)
#   define HOST_DEVICE __inline__ __device__ __host__
#   define sync_threads void
#	define HOST_DEVICE_LOOKUP static const
#endif

constexpr uint64_t ncr(int n, int r)
{
	uint64_t ans = 1;
	r = std::min(r, n - r);

	for (int j = 1; j <= r; j++, n--) {
		if (n % j == 0) {
			ans *= n / j;
		}
		else if (ans % j == 0) {
			ans = (ans / j) * n;
		}
		else {
			ans = (ans * n) / j;
		}
	}
	return ans;
}
#pragma warning( push )
#pragma warning ( disable : 4146 )

static void bit_twiddle_permute(uint64_t& v) {
	uint64_t t = v | (v - 1);
	v = (t + 1) | (((~t & -~t) - 1) >> (countr_zero(v) + 1));
}


class Timer {
public:
	Timer() {
		beginTimer = std::chrono::high_resolution_clock::now();
	}
	uint64_t nanos() const {
		return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - beginTimer).count();
	}

	double ms() const {
		return nanos() / 1000000.0;
	}
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> beginTimer;
};
