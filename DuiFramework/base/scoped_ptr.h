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


template<class T>
class scoped_ptr_release
{
public:
	explicit scoped_ptr_release(T* ptr = NULL) : ptr_(ptr){}
	~scoped_ptr_release() {
		if (ptr_) {
			ptr_->Release();
			ptr_ = NULL;
		}
	}

	void reset(T* ptr = NULL) {
		if (ptr != NULL && ptr == ptr_)
			return;

		T* old_ptr = ptr_;
		ptr_ = ptr;
		if (old_ptr) {
			old_ptr->Release();
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
	DISALLOW_COPY_AND_ASSIGN(scoped_ptr_release);
};

template <class T>
class scoped_refptr
{
public:
	scoped_refptr(T* t)
	{
		p_ = t;
		if (p_)
			p_->AddRef();
	}
	~scoped_refptr()
	{
		Clear();
	}
	scoped_refptr(const scoped_refptr<T>& r) : p_(r.p_) {
		if (p_)
			p_->AddRef();
	}
	void Clear()
	{
		if (p_)
			p_->Release();
	}
	T* operator->() const
	{
		return p_;
	}
	T* get() const
	{
		return p_;
	}
	void reset(T* p)
	{
		Clear();
		p_ = p;
		if (p_)
			p_->AddRef();
	}


private:
	T* p_;
};