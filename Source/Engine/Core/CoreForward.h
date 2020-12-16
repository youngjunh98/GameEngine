#ifndef INCLUDE_CORE_FORWARD
#define INCLUDE_CORE_FORWARD

namespace GameEngine
{
	class Engine;
	class Application;
	class Input;
	class Timer;

	// Math
	class Math;
	class Vector2;
	class Vector3;
	class Vector4;
	class Quaternion;
	class Matrix4x4;
	class AABB;
	class Plane;

	// RI
	class RenderingInterface;
	class RI_Resource;
	class RI_Buffer;
	class RI_VertexBuffer;
	class RI_IndexBuffer;
	class RI_ShaderConstantBuffer;
	class RI_Shader;
	class RI_VertexShader;
	class RI_PixelShader;
	class RI_HullShader;
	class RI_DomainShader;
	class RI_GeometryShader;
	class RI_ShaderResourceView;
	class RI_Sampler;
	class RI_Texture;
	class RI_Texture1D;
	class RI_Texture2D;
	class RI_Texture3D;
	class RI_RasterizerState;
	class RI_DepthStencilState;
	class RI_RenderTargetView;
	class RI_DepthStencilView;
	class RI_InputLayout;

	// Rendering
	class GlobalRenderer;
	class Shader;
	class Material;
	class Mesh;
	class Texture;
	class Texture2D;
	class TextureCube;
	class RenderPipeline;
	class RenderPass;

	// Physics
	class Physics;
	class Rigidbody;
	class Collider;
	class BoxCollider;
	class SphereCollider;

	// Audio
	class AudioClip;

	// Scene
	class Scene;
	class SceneManager;

	// Object
	class Object;
	//class GameObject;
	class Component;
	class Transform;
	class Camera;
	class Renderer;
	class MeshRenderer;
	class Light;
}

#endif