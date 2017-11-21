#pragma once

struct SerializedData
{
public:
	SerializedData()
	{
		data = nullptr;
		dataLength = 0;
	}

	SerializedData(void* _data, int _dataLength)
	{
		data = _data;
		dataLength = _dataLength;
	}

	void* data;
	int dataLength;
};

class Serializer
{
public:
	template<typename T>
	static SerializedData Serialize(T _toSerialize)
	{
		SerializedType<T> serializer;
		return serializer.Serialize(_toSerialize);
	}

	template<typename T>
	static SerializedData SerializeArray(std::vector<T> _toSerialize)
	{
		SerializedType<T> serializer;
		return serializer.SerializeArray(_toSerialize);
	}

	template<typename T>
	static T Deserialize(SerializedData _data)
	{
		SerializedType<T> serializer;
		return serializer.Deserialize(_data);
	}

	template<typename T>
	static std::vector<T> DeserializeArray(SerializedData _data)
	{
		SerializedType<T> serializer;
		return serializer.DeserializeArray(_data);
	}

	template<typename T>
	static T Deserialize(void* _data, int _len)
	{
		SerializedType<T> serializer;
		return serializer.Deserialize(_data, _len);
	}

	static void SendSerializedToFile(std::vector<SerializedData>& _data, std::string file)
	{
		ofstream os;
		os.open(file, ios::binary);
		os.seekp(0, ios::beg);

		for (int i = 0; i < _data.size(); ++i)
		{
			os.write(reinterpret_cast<char*>(&_data[i].dataLength), sizeof(_data[i].dataLength));
			os.write((char*)(_data[i].data), _data[i].dataLength);
		}

		os.close();
	}

	static void LoadSerializedFromFile(std::vector<SerializedData>& _data, std::string file)
	{
		ifstream is;
		is.open(file, ios::binary);
		is.seekg(0, ios::beg);

		while (!is.eof())
		{
			SerializedData newData;

			//Read the length
			int lengthBuffer;
			is.read((char*)(&newData.dataLength), sizeof(int));

			//Read the data
			newData.data = malloc(newData.dataLength);
			is.read((char*)(newData.data), newData.dataLength);

			//delete[] dataBuffer;

			_data.push_back(newData);
		}

		is.close();
	}
};

template<class T>
class SerializedType
{
public:
	virtual SerializedData Serialize(T _data)
	{
		SerializedData dat;

		int len = sizeof(_data);
		dat.data = malloc(len);
		std::memcpy(dat.data, &_data, len);
		dat.dataLength = len;

		return dat;
	}

	virtual SerializedData SerializeArray(std::vector<T> _data)
	{
		SerializedData dat;

		int len = sizeof(T) * _data.size();
		dat.data = malloc(len);
		std::memcpy(dat.data, _data.data(), len);
		dat.dataLength = len;

		return dat;
	}

	virtual T Deserialize(SerializedData _data)
	{
		T ret;
		std::memcpy(&ret, _data.data, _data.dataLength);
		return ret;
	}

	virtual std::vector<T> DeserializeArray(SerializedData _data)
	{
		T *charBuf = (T*)_data.data;
		/* create a vector by copying out the contents of charBuf */
		std::vector<T> ret(charBuf, charBuf + (int)((float)_data.dataLength / (float)sizeof(T)));

		return ret;
	}

	virtual T Deserialize(void* _data, int _len)
	{
		T ret;
		std::memcpy(&ret, _data, _len);
		return ret;
	}
};

template<>
class SerializedType<float>
{
public:
	virtual SerializedData Serialize(float _data)
	{
		//Convert to string and serialize as string
		std::string serialized = std::to_string(_data);
		return Serializer::Serialize<std::string>(serialized);
	}

	virtual float Deserialize(SerializedData _data)
	{
		//Convert to string and deserialize as string
		std::string deserialized = Serializer::Deserialize<std::string>(_data);
		return std::stof(deserialized);
	}

	virtual float Deserialize(void* _data, int _len)
	{
		//Convert to string and deserialize as string
		std::string deserialized = Serializer::Deserialize<std::string>(_data, _len);
		return std::stof(deserialized);
	}
};

template<>
class SerializedType<std::string>
{
public:
	virtual SerializedData Serialize(std::string _data)
	{
		SerializedData ret;

		int strlen = std::strlen(_data.c_str()) * sizeof(char);
		ret.data = malloc(strlen);
		memcpy(ret.data, _data.c_str(), strlen);
		ret.dataLength = strlen;

		return ret;
	}

	virtual std::string Deserialize(SerializedData _data)
	{
		std::string ret = "";
		ret.resize(_data.dataLength);
		std::memcpy(&ret[0], _data.data, _data.dataLength);
		return ret;
	}

	virtual std::string Deserialize(void* _data, int _len)
	{
		std::string ret = "";
		ret.reserve(_len);
		std::memcpy(&ret[0], _data, _len);
		return ret;
	}
};