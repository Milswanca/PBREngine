#pragma once
#include "SystemBase.h"
#include "Delegate.h"


template <typename Ret, typename... Params>
class RenderDelegate : public Delegate<Ret, Params...>
{
private:
	std::map<Callback<Ret, Params...>*, unsigned int> renderCallbacksMap;

public:
	template<typename T, typename Method>
	void Add(T* object, Method method, unsigned int flags)
	{
		MethodCallback<T, Method, void, Params...>* newCallback = new MethodCallback<T, Method, void, Params...>(object, method);
		callbacks.push_back(newCallback);
		renderCallbacksMap[newCallback] = flags;
	}

	void Invoke(unsigned int flags, Params... param)
	{
		for (Callback<Ret, Params...>* i : callbacks)
		{
			if (renderCallbacksMap[i] & flags)
			{
				i->Invoke(param...);
			}
		}
	}
};

//Struct which holds light data to pass to glsl
struct LightData
{
public:
	LightData(glm::vec3 _position, glm::vec3 _colour, glm::vec3 _direction, float _intensity, float _radius, unsigned int _lightType)
	{
		position = _position;
		colour = _colour;
		direction = _direction;
		intensity = _intensity;
		radius = _radius;
		lightType = _lightType;
	}


	glm::vec3 position;
	glm::vec3 colour;
	glm::vec3 direction;
	float intensity;
	float radius;
	unsigned int lightType;
};

class RenderManager : public SystemBase
{
public:
	RenderManager(ObjectInitData OI);
	~RenderManager();

	void Init();
	void Update(float deltaTime);
	void Deinit();

	//void GetAffectingLightsToPoint(std::vector<Light*>& _outLights, glm::vec3 _point);

	class Light* GetLight(unsigned int _id) const;
	void SetMaxAffectingLights(unsigned int _numLights);
	void RegisterLight(class Light* _light);

	void RegisterUniformBuffer(class UniformBufferObject* _buffer);

	virtual void RenderScene(class CameraComponent* camera, ERenderConditions condition = ERenderConditions::RC_All);
	virtual void RenderQuad(class Material* material);
	virtual void RenderCube(class CameraComponent* camera, class Material* material);

	template<typename T>
	void RegisterRenderTask(T* _object, void(T::*_func)(class CameraComponent*), unsigned int _flags)
	{
		if (!m_renderDelegate.IsAlreadyBound(_object, _func))
		{
			m_renderDelegate.Add(_object, _func, RenderFlagToConditions(_flags));
		}
	}

	//Get any of the main rendering assets
	class RenderTarget* GetMainRenderTarget()				const	{ return m_activeRenderTarget; }
	class Material* GetMainMaterial()						const	{ return m_activeMaterial; }
	class Texture* GetMainActiveTexture()					const	{ return m_activeTextures[m_activeTextureIndex]; }
	class Texture* GetMainTexture(unsigned int _index)	const	{ return m_activeTextures[_index]; }
	class Texture* GetMainDepthBuffer()					const	{ return m_activeDepthBuffer; }

	//Handle render target bindings
	void SetMainRenderTarget(class RenderTarget* _renderTarget);
	void ClearMainRenderTarget();
	void BindRenderTarget(class RenderTarget* _renderTarget);
	void UnbindBoundRenderTarget();

	//Handle texture bindings
	void SetActiveTextureIndex(unsigned int _activeIndex);
	void SetMainTexture(class Texture* _texture, unsigned int _boundIndex);
	void ClearMainTexture(unsigned int _boundIndex);
	void BindTexture(class Texture* _texture);
	void UnbindBoundTexture();

	//Handle depth textures (RenderBuffers)
	void SetMainDepthBuffer(class Texture* _tex);
	void ClearMainDepthBuffer();
	void BindDepthBuffer(class Texture* _tex);
	void UnbindBoundDepthBuffer();

	//Handle material bindings
	void SetMainMaterial(class Material* _mat);
	void ClearMainMaterial();
	void BindMaterial(class Material* _mat);
	void UnbindBoundMaterial();

	//Handle uniform buffers
	void BindUniformBuffer(class UniformBufferObject* _buffer, unsigned int _index);
	void UnbindUniformBuffer(unsigned int _index);
	class UniformBufferObject* GetUniformBuffer(unsigned int _boundIndex) const;
	class UniformBufferObject* GetUniformBuffer(const char* _bufferName);
	unsigned int GetUniformBufferBoundIndex(class UniformBufferObject* _buffer) const;

private:
	unsigned int RenderFlagToConditions(unsigned int _renderFlags);
	void RebufferLightData();
	void RebufferCameraUniformBuffer(CameraComponent* _newCamera);

	RenderDelegate<void, class CameraComponent*> m_renderDelegate;

	class RenderTarget* m_activeRenderTarget = nullptr;
	class RenderTarget* m_boundRenderTarget = nullptr;
	class Material* m_activeMaterial = nullptr;
	class Material* m_boundMaterial = nullptr;

	class Texture* m_boundDepthBuffer = nullptr;
	class Texture* m_activeDepthBuffer = nullptr;

	int m_activeTextureIndex = 0;
	class Texture* m_activeTextures[16];
	class Texture* m_boundTexture = nullptr; //Only a single bound texture can occur, it will be removed when changing texture index

	class UniformBufferObject* m_boundUniformBuffers[6]; //Only gonna allow for 6 of these (for now)
	std::map<const char*, class UniformBufferObject*> m_uniformBuffers;

	//Lights
	std::vector<class Light*> m_lights;
	unsigned int m_maxAffectingLights = 4;

	//Regular Meshes
	class StaticMesh* m_cubeMesh = nullptr;
	class StaticMesh* m_quadMesh = nullptr;

	//Uniform buffer for camera render data
	class UniformBufferObject* m_cameraUniformBuffer = nullptr;
	class UniformBufferObject* m_lightUniformBuffer = nullptr;
};