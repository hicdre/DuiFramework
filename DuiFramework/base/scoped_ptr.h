#pragma once
#include "base/basictypes.h"

template<class T>
class scoped_ptr
{
public:
	explicit scoped_ptr(T* ptr = NULL) : ptr_(ptr){}
	~scoped_ptr() {
		if (ptr_) {
			delete ptr_;
			ptr_ = NULL;
		}
	}

	void reset(T* ptr = NULL) {
		if (ptr != NULL && ptr == ptr_)
			return;

		T* old_ptr = ptr_;
		ptr_ = ptr;
		if (old_ptr) {
			delete old_ptr;
		}
	}

	T* get() const { return ptr_; }

	T* release() {
		T* old_ptr = ptr_;
		ptr_ = NULL;
		return old_ptr;
	}

	T& operator*() const {
		return *get();
	}
	T* operator->() const  {
		return get();
	}

private:
	T* ptr_;
	DISALLOW_COPY_AND_ASSIGN(scoped_ptr);
};

template <class T>
class scoped_ptr_malloc
{
public:
	explicit scoped_ptr_malloc(T* ptr = NULL) : ptr_(ptr){}
	~scoped_ptr_malloc() {
		if (ptr_) {
			free(ptr_);
			ptr_ = NULL;
		}
	}

	void reset(T* ptr = NULL) {
		if (ptr != NULL && ptr == ptr_)
			return;

		T* old_ptr = ptr_;
		ptr_ = ptr;
		if (old_ptr) {
			free(old_ptr);
		}
	}

	T* get() const { return ptr_; }

	T* release() {
		T* old_ptr = ptr_;
		ptr_ = NULL;
		return old_ptr;
	}

	T& operator*() const {
		return *get();
	}
	T* operator->() const  {
		return get();
	}

private:
	T* ptr_;
	DISALLOW_COPY_AND_ASSIGN(scoped_ptr_malloc);
};