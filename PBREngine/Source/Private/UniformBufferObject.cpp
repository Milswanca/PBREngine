#include "EnginePCH.h"
#include "Engine.h"

UniformBufferObject::UniformBufferObject(ObjectInitData _OI) : ObjectBase(_OI)
{
	m_renderManager = GetEngine()->GetRenderSystem();
}

UniformBufferObject::~UniformBufferObject()
{

}

UniformBufferObject* UniformBufferObject::Create(ObjectBase* _outer, const char* _bufferName, unsigned int _bufferSize)
{
	if (!_outer) { return nullptr; }

	UniformBufferObject* newBuffer = _outer->GetEngine()->CreateObject<UniformBufferObject>(_outer);

	newBuffer->Init(_bufferName, _bufferSize);

	return newBuffer;
}

void UniformBufferObject::Destroyed()
{
	ObjectBase::Destroyed();

	glDeleteBuffers(1, &m_id);
}

void UniformBufferObject::Init(const char* _bufferName, unsigned int _bufferSize)
{
	m_bufferName = _bufferName;
	m_size = _bufferSize;

	glGenBuffers(1, &m_id);
	glBindBuffer(GL_UNIFORM_BUFFER, m_id);
	glBufferData(GL_UNIFORM_BUFFER, _bufferSize, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	m_renderManager->RegisterUniformBuffer(this);
}

void UniformBufferObject::SetBufferData(void* _data, unsigned int _dataSize, unsigned int _dataOffset)
{
	unsigned int outIndex = 0;
	if (ContainsData(outIndex, _data))
	{
		m_data[outIndex].SetBufferData(_data, _dataSize, _dataOffset);
	}
	else
	{
		m_data.push_back(UniformBufferData(_data, _dataSize, _dataOffset));
	}

	glBindBuffer(GL_UNIFORM_BUFFER, m_id);
	glBufferSubData(GL_UNIFORM_BUFFER, _dataOffset, _dataSize, _data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBufferObject::Resize(unsigned int _size)
{
	m_size = _size;

	glBindBuffer(GL_UNIFORM_BUFFER, m_id);
	glBufferData(GL_UNIFORM_BUFFER, _size, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	RepushAllData();
}

void UniformBufferObject::RepushAllData()
{
	for (int i = 0; i < m_data.size(); ++i)
	{
		SetBufferData(m_data[i].GetData(), m_data[i].GetSize(), m_data[i].GetOffset());
	}
}

void UniformBufferObject::Bind(unsigned int _index)
{
	m_boundIndex = _index;
	m_renderManager->BindUniformBuffer(this, m_boundIndex);
}

void UniformBufferObject::Unbind()
{
	if (m_renderManager->GetUniformBuffer(m_boundIndex) == this)
	{
		m_renderManager->UnbindUniformBuffer(m_boundIndex);
	}
}

unsigned int UniformBufferObject::GetBufferID() const
{
	return m_id;
}

unsigned int UniformBufferObject::GetBufferSize() const
{
	return m_size;
}

const char* UniformBufferObject::GetBufferName() const
{
	return m_bufferName;
}

bool UniformBufferObject::ContainsData(unsigned int& _outIndex, void* _data)
{
	_outIndex = 0;

	for (int i = 0; i < m_data.size(); ++i)
	{
		if (m_data[i].GetData() == _data)
		{
			_outIndex = i;
			return true;
		}
	}

	return false;
}