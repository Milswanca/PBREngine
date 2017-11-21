#pragma once
#include "ObjectBase.h"

struct UniformBufferData
{
public:
	UniformBufferData(void* _data, unsigned int _size, unsigned int _offset)
	{
		SetBufferData(_data, _size, _offset);
	}

	void SetBufferData(void* _data, unsigned int _size, unsigned int _offset)
	{
		data = _data;
		size = _size;
		offset = _offset;
	}

	void* GetData() const { return data; }
	unsigned int GetSize() const { return size; }
	unsigned int GetOffset() const { return offset; }

private:
	void* data = nullptr;
	unsigned int size = 0;
	unsigned int offset = 0;
};

class UniformBufferObject : public ObjectBase
{
public:
	UniformBufferObject(ObjectInitData _OI);
	~UniformBufferObject();

	static UniformBufferObject* Create(ObjectBase* _outer, const char* _bufferName, unsigned int _bufferSize);

	virtual void Destroyed() override;

	void Init(const char* _bufferName, unsigned int _bufferSize);
	void SetBufferData(void* _data, unsigned int _dataSize, unsigned int _dataOffset);

	void Resize(unsigned int _size);

	void Bind(unsigned int _index);
	void Unbind();

	unsigned int GetBufferID() const;
	unsigned int GetBufferSize() const;
	const char* GetBufferName() const;

private:
	bool ContainsData(unsigned int& _outIndex, void* _data);
	void RepushAllData();

	unsigned int m_id = 0;
	unsigned int m_size = 0;
	std::vector<UniformBufferData> m_data;

	const char* m_bufferName = nullptr;
	unsigned int m_boundIndex = 0;

	class RenderManager* m_renderManager = nullptr;
};

