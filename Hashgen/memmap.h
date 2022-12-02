#include <string>
class memmap
{
public:
	memmap();
	memmap(std::string path, bool overwriteExisting = false);
	memmap(const memmap& other);
	memmap& operator=(memmap&& other) noexcept;
	~memmap();

	void* ptr() const
	{
		return basePtr;
	}

	template<typename T>
	T* ptr() const
	{
		return static_cast<T*>(basePtr);
	}

	size_t size() const {
		return fileSize;
	}

	template<typename T>
	size_t count() const {
		return size() / sizeof(T);
	}

private:
	void* basePtr;
	void* hFile;
	void* hMap;
	size_t fileSize;
};