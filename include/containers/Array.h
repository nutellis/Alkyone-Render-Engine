#pragma once

#include <core/PODTypes.h>

#include <assert.h>
#include <iostream>
#include <cstring>

template <typename T> //, typename Allocator = DefaultAllocator>
class TArray
{
public:
	//The type of the container's elements.
	using ValueType = T;

	// A reference to the element type.
	using Reference = T&;
	//consts reference
	using ConstReference = T const&;

	// A pointer to the element type.
	using Pointer = T*;
	//const pointer
	using ConstPointer = T const*;

	// The type used to index into the container.
	using SizeType = size_t;

	// The type of the result of subtracting two container iterators.
	using DifferenceType = ptrdiff_t;

public:

	explicit TArray() = default;

	//TODO: Should this arraySize be zero ????
	TArray(const SizeType Count) :
		First(static_cast<T*>(::operator new(Count * sizeof(T)))),
		Last(First),
		Storage(First + Count),
		ArraySize(0),
		StorageSize(Count) {

		if constexpr (std::is_trivially_constructible_v<T>) {
			memset(First, 0, Count * sizeof(T));
		} else {
			for (SizeType i = 0; i < Count; ++i) {
				new (First + i) T();
			}
		}
	}

	//initialized array with Value
	TArray(const SizeType Count, ValueType Value) :
		First(static_cast<T*>(::operator new(Count * sizeof(T)))),
		Last(First + Count),
		Storage(Last),
		ArraySize(Count),
		StorageSize(Count) {
		if (BuySpace(Count)) {
			this->Last = Fill(this->First, Count, Value);
			// Tidy(); 
		}
	}

	// Copy Constructor
	TArray(const TArray& Other) {
		CopyToEmpty(Other.First, Other.Size());
	}

	// TODO: might not be the safest approach
	// Copy Assignment operator
	TArray& operator =(const TArray& Other) {
		if (this != &Other) {
			CopyToEmpty(Other.First, Other.Size());
		}
		return *this;
	}

	TArray(TArray&& Other) noexcept :
	First(Other.First),
	Last(Other.Last),
	Storage(Other.Storage),
	ArraySize(Other.ArraySize),
	StorageSize(Other.StorageSize)
	{
		Other.First = nullptr;
		Other.Last = nullptr;
		Other.Storage = nullptr;
		Other.ArraySize = 0;
		Other.StorageSize = 0;
	}

	//TODO: maybe can be safer
	void CopyToEmpty(const Pointer Other, int64 OtherSize) {
		if (OtherSize || StorageSize)
		{
			ResizeForCopy(OtherSize, StorageSize);
			ConstructItems(First, Other, OtherSize);
		}
		else
		{
			StorageSize = 0;
		}
	}


	void ConstructItems(void* Dest, const ValueType* Source, int64 Count)
	{
		ArraySize = Count;

		while (Count) {
			new (Dest) ValueType(*Source);
			++(ValueType*&)Dest;
			++Source;
			--Count;
		}
		Last = First + ArraySize;
		Storage = First + StorageSize;

	}

	/*void ConstructItems(void* Dest, const Pointer Source, int64 Count)
	{
		Utilities::Memcpy(Dest, Source, Count * sizeof(ValueType) * Count);
		Last = First + ArraySize;
		EndOfArray = First + ArrayMax;
	}*/

	constexpr bool operator ==(TArray& Other) const {
		if (this->Size() != Other.Size()) {
			return false;
		}
		for (SizeType i = 0; i < this->Size(); ++i) {
			if (this->First[i] != Other.First[i]) {
				return false;
			}
		}
		return true;
	}

	TArray& operator=(TArray&& Other) noexcept {
		if (this != &Other) {
			Free();

			First = Other.First;
			Last = Other.Last;
			Storage = Other.Storage;
			ArraySize = Other.ArraySize;
			StorageSize = Other.StorageSize;

			Other.First = nullptr;
			Other.Last = nullptr;
			Other.Storage = nullptr;
			Other.ArraySize = 0;
			Other.StorageSize = 0;
		}
		return *this;
	}

	TArray& operator=(std::initializer_list<T> InitList) {
			SizeType ListSize = InitList.size();

			Clear();

			// If newsize == 0, destroy everything
			if (ListSize == 0) {
				return *this;
			} 
			
			if (ListSize > StorageSize) {
				// Not enough capacity, free and allocate new memory
				Free();
				BuySpace(ListSize);
			}
			Pointer Dest = First;
			for (const auto& Element : InitList) {
				new (Dest++) T(Element);
			}
			ArraySize = ListSize;
			Last = First + ArraySize;
			Storage = First + StorageSize;
			return *this;
	}

	constexpr bool operator!=(const TArray& Other) const {
		return !(*this == Other);
	}

	~TArray() noexcept{
		if constexpr (!std::is_trivially_destructible_v<T>)
		{
			Pointer Delete = First;
			while (Delete != Last) {
				Delete++->~ValueType();
			}
		}
		Free();
	}

	constexpr Reference operator[](SizeType Index) {
		return *(First + Index);
	}

	constexpr Reference operator[](SizeType Index) const {
		return *(First + Index);
	}


	Reference At(SizeType Index) {
		assert(Index < ArraySize);
		return this->First[Index];
	}

	ConstReference At(SizeType Index) const {
		assert(Index < ArraySize);
		return this->First[Index];
	}

	SizeType Size() const
	{
		return Last - First; //ArraySize;//
	}

	SizeType Capacity() const
	{
		return Storage - First; //ArrayMax;//
	}

	SizeType UnusedCapacity() const
	{
		return Storage - Last;
	}

	bool IsEmpty() const
	{
		return (First == Last);
	}

	bool IsNotEmpty() const
	{
		return !IsEmpty();
	}

	SizeType SizeOf() const
	{
		return Size() * sizeof(T);
	}

	Pointer Begin() const {
		return First;
	}

	Pointer End() const {
		return Last;
	}

	constexpr ConstReference Front() const {
		return *First;
	}

	constexpr Reference Front() {
		return *First;
	}

	constexpr ConstReference Back() const {
		return *(Last - 1);
	}

	constexpr Reference Back() {
		return *(Last - 1);
	}


	void PushBack(const ValueType & Value)
	{
		EmplaceBack(Value);
	}

	void PushBack(ValueType && Value)
	{
		EmplaceBack(std::move(Value));
	}

	template<class... Types>
	void EmplaceBack(Types&&... Values)
	{

		if (HasUnusedCapacity())
		{

			//Create a new value @ Last
			::new (static_cast<void *>(Last)) ValueType(std::forward<Types>(Values)...);

			++Last;
			++ArraySize;

		}
		else {
			EmplaceReallocate(std::forward<Types>(Values)...);
		}
	}

	template<class... Types>
	void EmplaceReallocate(Types&&... Values)
	{
		const SizeType OldSize = Size();
		const SizeType NewSize = OldSize + 1;
		const SizeType NewCapacity = Growth(NewSize);

		Pointer NewVector = static_cast<ValueType*>(::operator new(NewCapacity * sizeof(ValueType)));

		if constexpr (std::is_trivially_copyable_v<ValueType>) {
			memmove(NewVector, First, OldSize * sizeof(T));
		} 
		else {
			for (SizeType i = 0; i < OldSize; ++i) {
				::new (static_cast<void*>(NewVector + i)) ValueType(std::move(First[i]));
				First[i].~ValueType();  // Destroy old element
			}
		}

		::new (static_cast<void*>(NewVector + OldSize)) ValueType(std::forward<Types>(Values)...);

		ChangeArray(NewVector, NewSize, NewCapacity);

	}

	SizeType Growth(const SizeType NewSize)
	{
		const SizeType OldCapacity = Capacity();
		SizeType NewCapacity;
		const SizeType GeometricGrowth = OldCapacity + OldCapacity / 2;

		if (GeometricGrowth < NewSize)
		{
			NewCapacity = NewSize;
		}
		else {
			NewCapacity = GeometricGrowth;
		}

		return NewCapacity;
	}

	void ResizeForCopy(int64 NewMax, int64 PrevMax)
	{
		Free();
		if (NewMax)
		{
			First = static_cast<ValueType*>(::operator new(NewMax * sizeof(ValueType)));
			Last = First;
			Storage = First + NewMax;
		}
		else {
			First = Last = Storage = nullptr;
		}
		StorageSize = NewMax;
		ArraySize = 0;
	}

	void PopBack()
	{
		if (ArraySize > 0)
		{
			--Last;
			--ArraySize;

			if constexpr (!std::is_trivially_destructible_v<ValueType>)
			{
				Last->~ValueType();
			}
		}
	}

	void Clear() noexcept{

		while (Last != First)
		{
			Last--;
			Last->~T();
		}

		ArraySize = 0;
	}

	//returns Value, or nullptr
	template <typename Predicate>
	Pointer FindFirst(Predicate predicate) {
		Pointer Result = First;

		while (Result != Last)
		{
			if (predicate(*Result)) {
				return *Result;
			}
			++Result;
		}
		return nullptr;
	}

	template <typename Predicate>
	TArray<T> FindAll(Predicate predicate) {
		TArray<T> Matches;
		Pointer Result = First;

		while (Result != Last) {
			if (predicate(*Result)) {
				Matches.PushBack(*Result);
			}
			++Result;
		}
		return Matches;
	}

	void Move(TArray<T>&& OtherData) {
		Clear();
		*this = std::move(OtherData);
	}


	//void Reserve(SizeType NewCapacity) {
	//	if (NewCapacity > StorageSize) {
	//		// Allocate new storage
	//		Pointer NewStorage = static_cast<ValueType*>(::operator new(NewCapacity * sizeof(ValueType)));
	//		if (std::is_trivially_copyable<ValueType>::value == true) {
	//			memmove(NewStorage, First, ArraySize * sizeof(ValueType));
	//		}
	//		else {
	//			for (SizeType i = 0; i < ArraySize; ++i) {
	//				::new (static_cast<void*>(NewStorage + i)) ValueType(std::move(First[i]));
	//			}
	//		}
	//		Free();
	//		ChangeArray(NewStorage, ArraySize, NewCapacity);
	//	}
	//}


private:

	bool HasUnusedCapacity() const
	{
		return Last != Storage;
	}


	/*void Swap(Reference Source) {
		ValueType Temp = Utilities::Move(First);

		First = Utilities::Move(Source);

		Source = Utilities::Move(Temp);
	}*/

	bool BuySpace(SizeType NewCapacity) {
		// allocate array with _Newcapacity number of elements
		//	this->_Myfirst() = pointer();
		//	this->_Mylast() = pointer();
		//	this->_Myend() = pointer();

		//Too Small
		if (NewCapacity == 0) {
			return false;
		}

		//Too big
		/*if (_Newcapacity > max_size())
		{
			_Xlength();
		}*/

		Free();

		// just right
		First = static_cast <ValueType *> (::operator new(NewCapacity * sizeof(ValueType))); // TODO:
		Last = First; // TODO: don't really like this here. Possible bug!
		Storage = First + NewCapacity;
		StorageSize = NewCapacity;
		ArraySize = 0;

		return true;
	}

	Pointer Fill(Pointer Dest, SizeType Count, const ValueType & Value) {
		// Pointer First = Dest;
		Pointer Last = Dest;

		for (; 0 < Count; --Count) {

			//Create a new value @ Last
			::new(static_cast <void *> (Last)) ValueType(Value);

			++Last;
		}
		// First = Last;

		return Last;
	}

	void ChangeArray(Pointer NewArray, const SizeType NewSize, const SizeType NewCapacity)
	{
		Clear();
		Free();

		First = NewArray;

		Last = First + NewSize;

		Storage = First + NewCapacity;

		ArraySize = NewSize;

		StorageSize = NewCapacity;
	}

	void Free() {
		if (First != nullptr) {
			::operator delete(First);
			First = nullptr;
			Last = nullptr;
		}

	}

	
private:

	SizeType ArraySize {};

	SizeType StorageSize {};

	//i need a pointer to the first item
	ValueType *First {};

	//last
	ValueType *Last {};

	//and end of array
	ValueType *Storage {};



public:
		class Iterator {
		public:
			using iterator_category = std::forward_iterator_tag;


			Iterator(T* ptr) : m_ptr(ptr) {}

			T& operator*() const { return *m_ptr; }
			T* operator->() { return m_ptr; }

			// Prefix increment
			Iterator& operator++() { m_ptr++; return *this; }

			// Postfix increment
			Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

			friend bool operator==(const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; }
			friend bool operator!=(const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; }

		private:
			T* m_ptr;
		};

		class ConstIterator {
		public:
			using iterator_category = std::forward_iterator_tag;

			ConstIterator(const T* ptr) : m_ptr(ptr) {}

			const T& operator*() const { return *m_ptr; }
			const T* operator->() const { return m_ptr; }


			// Prefix increment
			ConstIterator& operator++() { m_ptr++; return *this; }

			// Postfix increment
			ConstIterator operator++(int) { ConstIterator tmp = *this; ++(*this); return tmp; }


			friend bool operator==(const ConstIterator& a, const ConstIterator& b) { return a.m_ptr == b.m_ptr; }
			friend bool operator!=(const ConstIterator& a, const ConstIterator& b) { return a.m_ptr != b.m_ptr; }

		private:
			const T* m_ptr;
		};

		Iterator begin() { return Iterator(First); }
		Iterator end() { return Iterator(Last); }
		ConstIterator begin() const { return ConstIterator(First); }
		ConstIterator end() const { return ConstIterator(Last); }
};