
#include <mutex>
#include <condition_variable>
#include <cstddef>


#ifndef BARRIER_INC
#define BARRIER_INC

namespace ari {


class barrier {
	size_t mThreads;
	size_t mWaiting;
	bool mCycle = false; // Boost makes this a size_t, however a bool will suffice and
	// potentially uses less memory. 
	
	bool mComplete = false;
	thread_local bool mIsPart = false;
	
	std::mutex mMtx;
	std::condition_variable mCondVar;
	
public:
	explicit barrier(ptrdiff_t num_threads) : mThreads{ num_threads }, mWaiting{ num_threads } {};
	barrier(const barrier&) = delete;
	
	barrier& operator=(const barrier&) = delete;
	~barrier() {
		std::scoped_lock lk{ mMtx };
		mCycle = !mCycle;
		mCondVar.notify_all();
	};
	
	void arrive_and_wait() {
		std::unique_lock lk{ mMtx };
		
		if (!mComplete)	{ // if we are not past the completion phase,
			//then mark this thread as part of the barrier
			mIsPart = true; 
		} 
		
		if (mComplete and !mIsPart) { // Standard does not state what happens if
			// if a non-participationg member calls this function.
			return; 
		} 
		
		// executes if complete and is a member
		--mWaiting;
	
		if (mWaiting == 0) { // we are the last one waiting
			if (!mComplete) mComplete = true;
			
			mWaiting = mThreads;
			mCycle = !mCycle;
			lk.unlock();
			mCondVar.notify_all();
		} else {
			bool cycle_local = mCycle;
			mCondVar.wait(lk, [this]{ return mCycle == cycle_local; });	
		}
	}
	
	void arrive_and_drop() {
		std::scoped_lock lk{ mMtx };
		if (mIsPart) mIsPart = false;
		--mThreads;
	}
	
};

} // end namespace ari

#endif