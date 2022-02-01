#pragma once
#include "../il2cpp/il2cpp_loader.h"
#include "cheat.h"

namespace UnityEngine
{
	typedef void* Type;
	typedef struct Cursor;
	typedef struct Resolution;
	typedef struct Screen;
	typedef struct Color;
	typedef struct Quaternion;
	typedef struct Object;
	typedef struct GameObject;
	typedef struct Component;
	typedef struct Transform;
	typedef struct Behaviour;
	typedef struct MonoBehaviour;
	typedef struct Bounds;
	typedef struct Shader;
	typedef struct Material;
	typedef struct Renderer;
	typedef struct Ray;
	typedef struct Camera;
	typedef struct Collider;
	typedef struct Rigidbody;
	typedef struct RaycastHit;
	typedef struct Physics;
	
	// ---------- CORE ----------

	struct Cursor
	{
		Cursor() = delete;
		enum CursorLockMode : int
		{
			None,
			Locked,
			Confined
		};

		static bool get_visible()
		{
			static const auto func = (bool(*)())il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Boolean"),
				XorStr("UnityEngine"),
				XorStr("Cursor"),
				XorStr("get_visible"),
				XorStr(""));
			return func();
		}
		static void set_visible(const bool& visible)
		{
			static const auto func = (void(*)(bool))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Cursor"),
				XorStr("set_visible"),
				XorStr("System.Boolean"));
			return func(visible);
		}

		static bool get_lockState()
		{
			static const auto func = (int(*)())il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.CursorLockMode"),
				XorStr("UnityEngine"),
				XorStr("Cursor"),
				XorStr("get_lockState"),
				XorStr(""));
			return func();
		}
		static void set_locakState(const int& lockState)
		{
			static const auto func = (void(*)(int))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Cursor"),
				XorStr("set_lockState"),
				XorStr("UnityEngine.CursorLockMode"));
			return func(lockState);
		}
	};

	struct Resolution
	{
		int m_Width;
		int m_Heigth;
		int m_RefreshRate;

		std::string ToString()
		{
			static const auto func = (generic_string*(*)(Resolution))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.String"),
				XorStr("UnityEngine"),
				XorStr("Resolution"),
				XorStr("ToString"),
				XorStr(""));
			return il2cpp::generic_to_string(func(*this));
		}
	};

	struct Screen
	{
		Screen() = delete;

		enum FullScreenMode : int
		{
			ExlusiveFullScreen,
			FullScreenWindow,
			MaximizedWindow,
			Windowed
		};

		static int get_width()
		{
			static const auto func = (int(*)())il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Int32"),
				XorStr("UnityEngine"),
				XorStr("Screen"),
				XorStr("get_width"),
				XorStr(""));
			return func();
		}
		static int get_height()
		{
			static const auto func = (int(*)())il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Int32"),
				XorStr("UnityEngine"),
				XorStr("Screen"),
				XorStr("get_height"),
				XorStr(""));
			return func();
		}

		static float get_dpi()
		{
			static const auto func = (int(*)())il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Single"),
				XorStr("UnityEngine"),
				XorStr("Screen"),
				XorStr("get_dpi"),
				XorStr(""));
			return func();
		}

		static Resolution get_currentResolution()
		{
			static const auto func = (Resolution(*)())il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Resolution"),
				XorStr("UnityEngine"),
				XorStr("Screen"),
				XorStr("get_currentResolution"),
				XorStr(""));
			return func();
		}

		static bool get_fullScreen()
		{
			static const auto func = (bool(*)())il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Boolean"),
				XorStr("UnityEngine"),
				XorStr("Screen"),
				XorStr("get_fullScreen"),
				XorStr(""));
			return func();
		}

		static bool get_fullScreenMode()
		{
			static const auto func = (FullScreenMode(*)())il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.FullScreenMode"),
				XorStr("UnityEngine"),
				XorStr("Screen"),
				XorStr("get_fullScreenMode"),
				XorStr(""));
			return func();
		}

		static void SetResolution(const int& width, const int& height, const FullScreenMode& fullscreenMode, const int& preferredRefreshRate)
		{
			static const auto func = (void(*)(int, int, FullScreenMode, int))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Screen"),
				XorStr("SetResolution"),
				XorStr("System.Int32, System.Int32, UnityEngine.FullScreenMode, System.Int32"));
			return func(width, height, fullscreenMode, preferredRefreshRate);
		}
		static void SetResolution(const int& width, const int& height, const bool& fullscreen, const int& preferredRefreshRate)
		{
			static const auto func = (void(*)(int, int, bool, int))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Screen"),
				XorStr("SetResolution"),
				XorStr("System.Int32, System.Int32, System.Boolean, System.Int32"));
			return func(width, height, fullscreen, preferredRefreshRate);
		}

		static generic_array_t<Resolution>* get_resolutions()
		{
			static const auto func = (generic_array_t<Resolution>*(*)())il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Resolution[]"),
				XorStr("UnityEngine"),
				XorStr("Screen"),
				XorStr("get_resolutions"),
				XorStr(""));
			return func();
		}
	};

	struct Color
	{
		float x;
		float y;
		float z;
		float w;
		constexpr operator const Vec4() { return { x, y, z, w }; }
	};

	struct Quaternion
	{
		float x;
		float y;
		float z;
		float w;

		vec3 get_eulerAngles()
		{
			static const auto func = (vec3(*)(Quaternion))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Vector3"),
				XorStr("UnityEngine"),
				XorStr("Quaternion"),
				XorStr("get_eulerAngles"),
				XorStr(""));
			return func(*this);
		}

		static Quaternion FromRotation(const vec3& fromDirection, const vec3& toDirection)
		{
			static const auto func = (Quaternion(*)(vec3, vec3))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Quaternion"),
				XorStr("UnityEngine"),
				XorStr("Quaternion"),
				XorStr("FromRotation"),
				XorStr("UnityEngine.Vector3, UnityEngine.Vector3"));
			return func(fromDirection, toDirection);
		}

		static Quaternion Inverse(const Quaternion& rotation)
		{
			static const auto func = (Quaternion(*)(Quaternion))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Quaternion"),
				XorStr("UnityEngine"),
				XorStr("Quaternion"),
				XorStr("Inverse"),
				XorStr("UnityEngine.Quaternion"));
			return func(rotation);
		}

		static Quaternion Slerp(const Quaternion& a, const Quaternion& b, const float& t)
		{
			static const auto func = (Quaternion(*)(Quaternion, Quaternion, float))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Quaternion"),
				XorStr("UnityEngine"),
				XorStr("Quaternion"),
				XorStr("Slerp"),
				XorStr("UnityEngine.Quaternion, UnityEngine.Quaternion, System.Single"));
			return func(a, b, t);
		}
		static Quaternion Lerp(const Quaternion& a, const Quaternion& b, const float& t)
		{
			static const auto func = (Quaternion(*)(Quaternion, Quaternion, float))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Quaternion"),
				XorStr("UnityEngine"),
				XorStr("Quaternion"),
				XorStr("Lerp"),
				XorStr("UnityEngine.Quaternion, UnityEngine.Quaternion, System.Single"));
			return func(a, b, t);
		}

		static Quaternion AngleAxis(const float& angle, const vec3& axis)
		{
			static const auto func = (Quaternion(*)(float, vec3))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Quaternion"),
				XorStr("UnityEngine"),
				XorStr("Quaternion"),
				XorStr("AngleAxis"),
				XorStr("UnityEngine.Single, UnityEngine.Vector3"));
			return func(angle, axis);
		}
		static Quaternion LookRotation(const vec3& forward)
		{
			static const auto func = (Quaternion(*)(vec3))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Quaternion"),
				XorStr("UnityEngine"),
				XorStr("Quaternion"),
				XorStr("LookRotation"),
				XorStr("UnityEngine.Vector3"));
			return func(forward);
		}

		Quaternion() {}
		Quaternion(const float& x, const float& y, const float& z, const float& w) :
			x(x), y(y), z(z), w(w) {}

		Quaternion operator*(const Quaternion& rhs)
		{
			static const auto func = (Quaternion(*)(Quaternion, Quaternion))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Quaternion"),
				XorStr("UnityEngine"),
				XorStr("Quaternion"),
				XorStr("op_Multiply"),
				XorStr("UnityEngine.Quaternion, UnityEngine.Quaternion"));
			return func(*this, rhs);
		}
		Quaternion operator*(const vec3& point)
		{
			static const auto func = (Quaternion(*)(Quaternion, vec3))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Quaternion"),
				XorStr("UnityEngine"),
				XorStr("Quaternion"),
				XorStr("op_Multiply"),
				XorStr("UnityEngine.Quaternion, UnityEngine.Vector3"));
			return func(*this, point);
		}

		bool operator==(const Quaternion& rhs)
		{
			static const auto func = (bool(*)(Quaternion, Quaternion))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Boolean"),
				XorStr("UnityEngine"),
				XorStr("Quaternion"),
				XorStr("op_Equality"),
				XorStr("UnityEngine.Quaternion, UnityEngine.Quaternion"));
			return func(*this, rhs);
		}
		bool operator!=(const Quaternion& rhs)
		{
			static const auto func = (bool(*)(Quaternion, Quaternion))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Boolean"),
				XorStr("UnityEngine"),
				XorStr("Quaternion"),
				XorStr("op_Inequality"),
				XorStr("UnityEngine.Quaternion, UnityEngine.Quaternion"));
			return func(*this, rhs);
		}

		static float Dot(const Quaternion& a, const Quaternion& b)
		{
			static const auto func = (float(*)(Quaternion, Quaternion))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Single"),
				XorStr("UnityEngine"),
				XorStr("Quaternion"),
				XorStr("Dot"),
				XorStr("UnityEngine.Quaternion, UnityEngine.Quaternion"));
			return func(a, b);
		}
		static float Angle(const Quaternion& a, const Quaternion& b)
		{
			static const auto func = (float(*)(Quaternion, Quaternion))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Single"),
				XorStr("UnityEngine"),
				XorStr("Quaternion"),
				XorStr("Angle"),
				XorStr("UnityEngine.Quaternion, UnityEngine.Quaternion"));
			return func(a, b);
		}

		static Quaternion Euler(const vec3& euler)
		{
			static const auto func = (Quaternion(*)(vec3))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Quaternion"),
				XorStr("UnityEngine"),
				XorStr("Quaternion"),
				XorStr("Euler"),
				XorStr("UnityEngine.Vector3"));
			return func(euler);
		}
	};

	struct Object
	{
		static Object* FindObjectFromInstanceID(const int& instanceID)
		{
			static const auto func = (Object*(*)(int))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Object"),
				XorStr("UnityEngine"),
				XorStr("Object"),
				XorStr("FindObjectFromInstanceID"),
				XorStr("System.Int32"));
			return func(instanceID);
		}

		template<typename T = uintptr_t>
		static T FindObjectOfType(const std::string& type, const bool includeInactive = false)
		{
			static const auto func = (T(*)(Type, bool))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Object"),
				XorStr("UnityEngine"),
				XorStr("Object"),
				XorStr("FindObjectOfType"),
				XorStr("System.Type, System.Boolean"));
			return func(il2cpp::get_type(type), includeInactive);
		}
		template<typename T = uintptr_t>
		static T FindObjectOfType(const std::string& type, const std::string& assembly, const bool includeInactive = false)
		{
			return FindObjectOfType<T>(il2cpp::translateType(type, assembly));
		}

		template<typename T = uintptr_t>
		static generic_array_t<T>* FindObjectsOfType(const std::string& type, const bool includeInactive = false)
		{
			static const auto func = (generic_array_t<T>*(*)(Type, bool))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Object[]"),
				XorStr("UnityEngine"),
				XorStr("Object"),
				XorStr("FindObjectsOfType"),
				XorStr("System.Type, System.Boolean"));
			return func(il2cpp::get_type(type), includeInactive);
		} 

		int GetInstanceID()
		{
			static const auto func = (int(*)(Object*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Int32"),
				XorStr("UnityEngine"),
				XorStr("Object"),
				XorStr("GetInstanceID"),
				XorStr(""));
			return func(this);
		}

		int GetHashCode()
		{
			static const auto func = (int(*)(Object*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Int32"),
				XorStr("UnityEngine"),
				XorStr("Object"),
				XorStr("GetHashCode"),
				XorStr(""));
			return func(this);
		}

		std::string get_name()
		{
			static const auto func = (generic_string*(*)(Object*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.String"),
				XorStr("UnityEngine"),
				XorStr("Object"),
				XorStr("get_name"),
				XorStr(""));
			return il2cpp::generic_to_string(func(this));
		}
		void set_name(const std::string& name)
		{
			static const auto func = (void(*)(Object*, generic_string*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Object"),
				XorStr("set_name"),
				XorStr("System.String"));
			return func(this, il2cpp::string_to_generic(name));
		}

		void Destroy()
		{
			static const auto func = (void(*)(Object*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Object"),
				XorStr("Destroy"),
				XorStr("UnityEngine.Object"));
			return func(this);
		}
	};

	struct GameObject : Object
	{
		GameObject() = delete;
		static GameObject* _GameObject()
		{
			static const auto func = (GameObject*(*)())il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("GameObject"),
				XorStr(".ctor"),
				XorStr(""));
			return func();
		}
		static GameObject* _GameObject(const std::string& name)
		{
			static const auto func = (GameObject*(*)(generic_string*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("GameObject"),
				XorStr(".ctor"),
				XorStr("System.String"));
			return func(il2cpp::string_to_generic(name));
		}

		Component* GetComponent(const std::string& type)
		{
			static const auto func = (Component*(*)(GameObject*, Type))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Component"),
				XorStr("UnityEngine"),
				XorStr("GameObject"),
				XorStr("GetComponent"),
				XorStr("System.Type"));
			return func(this, il2cpp::get_type(type));
		}

		Component* GetComponentFastPath(const std::string& type, uintptr_t oneFurtherThanResultValue)
		{
			static const auto func = (Component * (*)(GameObject*, Type, uintptr_t))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Component"),
				XorStr("UnityEngine"),
				XorStr("GameObject"),
				XorStr("GetComponentFastPath"),
				XorStr("System.Type, System.IntPtr"));
			return func(this, il2cpp::get_type(type), oneFurtherThanResultValue);
		}

		Component* GetComponentInChildren(const std::string& type, bool includeInactive)
		{
			static const auto func = (Component * (*)(GameObject*, Type, bool))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Component"),
				XorStr("UnityEngine"),
				XorStr("GameObject"),
				XorStr("GetComponentInChildren"),
				XorStr("System.Type, System.Boolean"));
			return func(this, il2cpp::get_type(type), includeInactive);
		}

		Component* GetComponentInParent(const std::string& type, bool includeInactive)
		{
			static const auto func = (Component * (*)(GameObject*, Type, bool))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Component"),
				XorStr("UnityEngine"),
				XorStr("GameObject"),
				XorStr("GetComponentInParent"),
				XorStr("System.Type, System.Boolean"));
			return func(this, il2cpp::get_type(type), includeInactive);
		}
		Component* GetComponentInParent(const std::string& type)
		{
			static const auto func = (Component * (*)(GameObject*, Type))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Component"),
				XorStr("UnityEngine"),
				XorStr("GameObject"),
				XorStr("GetComponentInParent"),
				XorStr("System.Type"));
			return func(this, il2cpp::get_type(type));
		}

		Component* AddComponent(const std::string& componentType)
		{
			static const auto func = (Component * (*)(GameObject*, Type))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Component"),
				XorStr("UnityEngine"),
				XorStr("GameObject"),
				XorStr("AddComponent"),
				XorStr("System.Type"));
			return func(this, il2cpp::get_type(componentType));
		}

		Transform* get_transform()
		{
			static const auto func = (Transform * (*)(GameObject*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Transform"),
				XorStr("UnityEngine"),
				XorStr("GameObject"),
				XorStr("get_transform"),
				XorStr(""));
			return func(this);
		}

		int get_layer()
		{
			static const auto func = (int(*)(GameObject*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Int32"),
				XorStr("UnityEngine"),
				XorStr("GameObject"),
				XorStr("get_layer"),
				XorStr(""));
			return func(this);
		}
		void set_layer(const int& layer)
		{
			static const auto func = (void(*)(GameObject*, int))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("GameObject"),
				XorStr("set_layer"),
				XorStr("System.Int32"));
			return func(this, layer);
		}

		void SetActive(const bool& value)
		{
			static const auto func = (void(*)(GameObject*, bool))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("GameObject"),
				XorStr("SetActive"),
				XorStr("System.Boolean"));
			return func(this, value);
		}

		bool get_activeSelf()
		{
			static const auto func = (bool(*)(GameObject*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Boolean"),
				XorStr("UnityEngine"),
				XorStr("GameObject"),
				XorStr("get_activeSelf"),
				XorStr(""));
			return func(this);
		}
		std::string get_tag()
		{
			static const auto func = (generic_string*(*)(GameObject*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.String"),
				XorStr("UnityEngine"),
				XorStr("GameObject"),
				XorStr("get_tag"),
				XorStr(""));
			return il2cpp::generic_to_string(func(this));
		}

		template<typename T>
		generic_array_t<T*>* GetComponents(const std::string& type, const bool& includeInactive = true)
		{
			return GetComponentsInternal<T>(type, true, false, includeInactive, false, nullptr);
		}

		template<typename T>
		generic_array_t<T*>* GetComponentsInChildren(const std::string& type, const bool& includeInactive = false)
		{
			return GetComponentsInternal<T>(type, true, true, includeInactive, false, nullptr);
		}

		template<typename T>
		generic_array_t<T*>* GetComponentsInParents(const std::string& type, const bool& includeInactive = false)
		{
			return GetComponentsInternal<T>(type, true, true, includeInactive, true, nullptr);
		}

		bool CompareTag(const std::string& tag)
		{
			static const auto func = (bool(*)(GameObject*, generic_string*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Boolean"),
				XorStr("UnityEngine"),
				XorStr("GameObject"),
				XorStr("CompareTag"),
				XorStr("System.String"));
			return func(this, il2cpp::string_to_generic(tag));
		}

private:
	template<typename T>
	generic_array_t<T*>* GetComponentsInternal(const std::string& type, const bool& useSearchTypeAsArrayReturnType, const bool& recursive, const bool& includeInactive, const bool& reverse, void* resultList)
	{
		static const auto func = (generic_array_t<T*>*(*)(GameObject*, Type, bool, bool, bool, bool, void*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
			XorStr("System.Array"),
			XorStr("UnityEngine"),
			XorStr("GameObject"),
			XorStr("GetComponentsInternal"),
			XorStr("System.Type, System.Boolean, System.Boolean, System.Boolean, System.Boolean, System.Object"));
		return func(this, il2cpp::get_type(type), useSearchTypeAsArrayReturnType, recursive, includeInactive, includeInactive, resultList);
	}
	};

	struct Component : Object
	{
		static Component* _Component()
		{
			static const auto func = (Component * (*)())il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Component"),
				XorStr(".ctor"),
				XorStr(""));
			return func();
		}

		Transform* get_transform()
		{
			static const auto func = (Transform* (*)(Component*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Transform"),
				XorStr("UnityEngine"),
				XorStr("Component"),
				XorStr("get_transform"),
				XorStr(""));
			return func(this);
		}

		GameObject* get_gameObject()
		{
			static const auto func = (GameObject * (*)(Component*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.GameObject"),
				XorStr("UnityEngine"),
				XorStr("Component"),
				XorStr("get_gameObject"),
				XorStr(""));
			return func(this);
		}

		Component* GetComponent(Type type)
		{
			static const auto func = (Component * (*)(Component*, Type))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Component"),
				XorStr("UnityEngine"),
				XorStr("Component"),
				XorStr("GetComponent"),
				XorStr("System.Type"));
			return func(this, type);
		}
		Component* GetComponent(const std::string& type)
		{
			static const auto func = (Component * (*)(Component*, generic_string*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Component"),
				XorStr("UnityEngine"),
				XorStr("Component"),
				XorStr("GetComponent"),
				XorStr("System.String"));
			return func(this, il2cpp::string_to_generic(type));
		}

		Component* GetComponentInChildren(const std::string& type, bool includeInactive)
		{
			static const auto func = (Component * (*)(Component*, Type, bool))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Component"),
				XorStr("UnityEngine"),
				XorStr("Component"),
				XorStr("GetComponentInChildren"),
				XorStr("System.Type, System.Boolean"));
			return func(this, il2cpp::get_type(type), includeInactive);
		}

		Component* GetComponentInParent(const std::string& type)
		{
			static const auto func = (Component * (*)(Component*, Type))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Component"),
				XorStr("UnityEngine"),
				XorStr("Component"),
				XorStr("GetComponentInParent"),
				XorStr("System.Type"));
			return func(this, il2cpp::get_type(type));
		}

		template<typename T>
		generic_array_t<T*>* GetComponents(const std::string& type, const bool& includeInactive = false)
		{
			return get_gameObject()->GetComponents<T>(type, includeInactive);
		}

		template<typename T>
		generic_array_t<T*>* GetComponentsInChildren(const std::string& type, const bool& includeInactive = false)
		{
			return get_gameObject()->GetComponentsInChildren<T>(type, includeInactive);
		}

		template<typename T>
		generic_array_t<T*>* GetComponentsInParents(const std::string& type, const bool& includeInactive = false)
		{
			return get_gameObject()->GetComponenstInParent<T>(type, includeInactive);
		}

		std::string get_tag()
		{
			static const auto func = (generic_string*(*)(Component*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.String"),
				XorStr("UnityEngine"),
				XorStr("Component"),
				XorStr("get_tag"),
				XorStr(""));
			return il2cpp::generic_to_string(func(this));
		}
		bool CompareTag(const std::string& tag)
		{
			static const auto func = (bool(*)(Component*, generic_string*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Boolean"),
				XorStr("UnityEngine"),
				XorStr("Component"),
				XorStr("CompareTag"),
				XorStr("System.String"));
			return func(this, il2cpp::string_to_generic(tag));
		}
	};

	struct Transform : Component
	{
		Transform() = delete;
		static Transform* _Transform()
		{
			static const auto func = (Transform * (*)())il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr(".ctor"),
				XorStr(""));
			return func();
		}

		vec3 get_position()
		{
			static const auto func = (vec3(*)(Transform*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Vector3"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr("get_position"),
				XorStr(""));
			return func(this);
		}
		void set_position(const vec3& position)
		{
			static const auto func = (void(*)(Transform*, vec3))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr("set_position"),
				XorStr("UnityEngine.Vector3"));
			return func(this, position);
		}
		vec3 get_localPosition()
		{
			static const auto func = (vec3(*)(Transform*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Vector3"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr("get_localPosition"),
				XorStr(""));
			return func(this);
		}
		void set_localPosition(const vec3& localPosition)
		{
			static const auto func = (void(*)(Transform*, vec3))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr("set_localPosition"),
				XorStr("UnityEngine.Vector3"));
			return func(this, localPosition);
		}

		vec3 get_eulerAngles()
		{
			static const auto func = (vec3(*)(Transform*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Vector3"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr("get_eulerAngles"),
				XorStr(""));
			return func(this);
		}
		void set_localEulerAngles(const vec3& eulerAngles)
		{
			static const auto func = (void(*)(Transform*, vec3))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr("set_localEulerAngles"),
				XorStr("UnityEngine.Vector3"));
			return func(this, eulerAngles);
		}

		vec3 get_right()
		{
			static const auto func = (vec3(*)(Transform*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Vector3"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr("get_right"),
				XorStr(""));
			return func(this);
		}
		vec3 get_up()
		{
			static const auto func = (vec3(*)(Transform*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Vector3"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr("get_up"),
				XorStr(""));
			return func(this);
		}
		vec3 get_forward()
		{
			static const auto func = (vec3(*)(Transform*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Vector3"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr("get_forward"),
				XorStr(""));
			return func(this);
		}

		Quaternion get_rotation()
		{
			static const auto func = (Quaternion(*)(Transform*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Quaternion"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr("get_rotation"),
				XorStr(""));
			return func(this);
		}
		void set_rotation(const Quaternion& rotation)
		{
			static const auto func = (void(*)(Transform*, Quaternion))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr("set_rotation"),
				XorStr("UnityEngine.Quaternion"));
			return func(this, rotation);
		}
		Quaternion get_localRotation()
		{
			static const auto func = (Quaternion(*)(Transform*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Quaternion"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr("get_localRotation"),
				XorStr(""));
			return func(this);
		}
		void set_localRotation(const Quaternion& rotation)
		{
			static const auto func = (void(*)(Transform*, Quaternion))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr("set_localRotation"),
				XorStr("UnityEngine.Quaternion"));
			return func(this, rotation);
		}

		vec3 get_localScale()
		{
			static const auto func = (vec3(*)(Transform*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Vector3"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr("get_localScale"),
				XorStr(""));
			return func(this);
		}
		void set_localScale(const vec3& localScale)
		{
			static const auto func = (void(*)(Transform*, vec3))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr("set_localScale"),
				XorStr("UnityEngine.Vector3"));
			return func(this, localScale);
		}

		Transform* get_parent()
		{
			static const auto func = (Transform*(*)(Transform*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Transform"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr("get_parent"),
				XorStr(""));
			return func(this);
		}
		void set_parent(Transform* parent)
		{
			static const auto func = (void(*)(Transform*, Transform*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr("set_parent"),
				XorStr("UnityEngine.Transform"));
			return func(this, parent);
		}

		Transform* GetParent()
		{
			static const auto func = (Transform * (*)(Transform*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Transform"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr("GetParent"),
				XorStr(""));
			return func(this);
		}
		void SetParent(Transform* p)
		{
			static const auto func = (void(*)(Transform*, Transform*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr("SetParent"),
				XorStr("UnityEngine.Transform"));
			return func(this, p);
		}
		void SetParent(Transform* parent, bool worldPositionStays)
		{
			static const auto func = (void(*)(Transform*, Transform*, bool))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr("SetParent"),
				XorStr("UnityEngine.Transform, System.Boolean"));
			return func(this, parent, worldPositionStays);
		}

		void Rotate(const vec3& eulers)
		{
			static const auto func = (void(*)(Transform*, vec3))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr("Rotate"),
				XorStr("UnityEngine.Vector3"));
			return func(this, eulers);
		}
		void Rotate(const float& xAngle, const float& yAngle, const float& zAngle)
		{
			static const auto func = (void(*)(Transform*, float, float, float))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr("Rotate"),
				XorStr("System.Single, System.Single, System.Single"));
			return func(this, xAngle, yAngle, zAngle);
		}
		void RotateAround(const vec3& point, const vec3& axis, const float& angle)
		{
			static const auto func = (void(*)(Transform*, vec3, vec3, float))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr("RotateAround"),
				XorStr("UnityEngine.Vector3, UnityEngine.Vector3, System.Single"));
			return func(this, point, axis, angle);
		}

		void LookAt(Transform* target)
		{
			static const auto func = (void(*)(Transform*, Transform*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr("LookAt"),
				XorStr("UnityEngine.Transform"));
			return func(this, target);
		}
		void LookAt(const vec3& worldPosition)
		{
			static const auto func = (void(*)(Transform*, vec3))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr("LookAt"),
				XorStr("UnityEngine.Vector3"));
			return func(this, worldPosition);
		}

		vec3 TransformDirection(const vec3& direction)
		{
			static const auto func = (vec3(*)(Transform*, vec3))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Vector3"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr("TransformDirection"),
				XorStr("UnityEngine.Vector3"));
			return func(this, direction);
		}
		vec3 InverseTransformDirection(const vec3& direction)
		{
			static const auto func = (vec3(*)(Transform*, vec3))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Vector3"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr("InverseTransformDirection"),
				XorStr("UnityEngine.Vector3"));
			return func(this, direction);
		}

		vec3 TransformPoint(const vec3& position)
		{
			static const auto func = (vec3(*)(Transform*, vec3))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Vector3"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr("TransformPoint"),
				XorStr("UnityEngine.Vector3"));
			return func(this, position);
		}
		vec3 InverseTransformPoint(const vec3& position)
		{
			static const auto func = (vec3(*)(Transform*, vec3))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Vector3"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr("InverseTransformPoint"),
				XorStr("UnityEngine.Vector3"));
			return func(this, position);
		}

		Transform* GetRoot()
		{
			static const auto func = (Transform*(*)(Transform*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Transform"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr("GetRoot"),
				XorStr(""));
			return func(this);
		}

		int get_childCount()
		{
			static const auto func = (int(*)(Transform*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Int32"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr("GetRoot"),
				XorStr(""));
			return func(this);
		}

		vec3 get_lossyScale()
		{
			static const auto func = (vec3(*)(Transform*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Vector3"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr("get_lossyScale"),
				XorStr(""));
			return func(this);
		}

		bool IsChildOf(Transform* parent)
		{
			static const auto func = (bool(*)(Transform*, Transform*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Boolean"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr("IsChildOf"),
				XorStr("UnityEngine.Transform"));
			return func(this, parent);
		}

		Transform* GetChild(const int& index)
		{
			static const auto func = (Transform*(*)(Transform*, int))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Transform"),
				XorStr("UnityEngine"),
				XorStr("Transform"),
				XorStr("GetChild"),
				XorStr("System.Int32"));
			return func(this, index);
		}
	};

	struct Behaviour : Component
	{
		static Behaviour* _Behaviour()
		{
			static const auto func = (Behaviour * (*)())il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Behaviour"),
				XorStr(".ctor"),
				XorStr(""));
			return func();
		}

		bool get_enabled()
		{
			static const auto func = (bool(*)(Behaviour*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Boolean"),
				XorStr("UnityEngine"),
				XorStr("Behaviour"),
				XorStr("get_enabled"),
				XorStr(""));
			return func(this);
		}
		void set_enabled(const bool& enabled)
		{
			static const auto func = (void(*)(Behaviour*, bool))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Behaviour"),
				XorStr("set_enabled"),
				XorStr("System.Boolean"));
			return func(this, enabled);
		}

		bool get_isActiveAndEnabled()
		{
			static const auto func = (bool(*)(Behaviour*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Boolean"),
				XorStr("UnityEngine"),
				XorStr("Behaviour"),
				XorStr("get_isActiveAndEnabled"),
				XorStr(""));
			return func(this);
		}
	};

	struct MonoBehaviour : Behaviour
	{
		static MonoBehaviour* _MonoBehaviour()
		{
			static const auto func = (MonoBehaviour*(*)())il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("MonoBehaviour"),
				XorStr(".ctor"),
				XorStr(""));
			return func();
		}

		bool IsInvoking()
		{
			static const auto func = (bool(*)(MonoBehaviour*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Boolean"),
				XorStr("UnityEngine"),
				XorStr("MonoBehaviour"),
				XorStr("IsInvoking"),
				XorStr(""));
			return func(this);
		}

		void CancelInvoke()
		{
			static const auto func = (void(*)(MonoBehaviour*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("MonoBehaviour"),
				XorStr("CancelInvoke"),
				XorStr(""));
			return func(this);
		}

		void Invoke(const std::string& methodName, const float& time)
		{
			static const auto func = (void(*)(MonoBehaviour*, generic_string*, float))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("MonoBehaviour"),
				XorStr("Invoke"),
				XorStr("System.String, System.Single"));
			return func(this, il2cpp::string_to_generic(methodName), time);
		}

		void InvokeRepeating(const std::string& methodName, const float& time, const float& repeatRate)
		{
			static const auto func = (void(*)(MonoBehaviour*, generic_string*, float, float))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("MonoBehaviour"),
				XorStr("InvokeRepeating"),
				XorStr("System.String, System.Single, System.Single"));
			return func(this, il2cpp::string_to_generic(methodName), time, repeatRate);
		}
		
		void CancelInvoke(const std::string& methodName)
		{
			static const auto func = (void(*)(MonoBehaviour*, generic_string*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("MonoBehaviour"),
				XorStr("CancelInvoke"),
				XorStr("System.String"));
			return func(this, il2cpp::string_to_generic(methodName));
		}

		bool IsInvoking(const std::string& methodName)
		{
			static const auto func = (bool(*)(MonoBehaviour*, generic_string*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Boolean"),
				XorStr("UnityEngine"),
				XorStr("MonoBehaviour"),
				XorStr("IsInvoking"),
				XorStr("System.String"));
			return func(this, il2cpp::string_to_generic(methodName));
		}

	};

	struct Bounds
	{
		Bounds() = delete;
		static Bounds* _Bounds(const vec3& center, const vec3& size)
		{
			static const auto func = (Bounds*(*)(vec3, vec3))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Bounds"),
				XorStr(".ctor"),
				XorStr("UnityEngine.Vector3, UnityEngine.Vector3"));
			return func(center, size);
		}

		vec3 m_Center;
		vec3 m_Extents;

		int GetHashCode()
		{
			static const auto func = (int(*)(Bounds*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Int32"),
				XorStr("UnityEngine"),
				XorStr("Bounds"),
				XorStr("GetHashCode"),
				XorStr(""));
			return func(this);
		}

		bool Equals(Type other)
		{
			static const auto func = (bool(*)(Bounds*, Type))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Boolean"),
				XorStr("UnityEngine"),
				XorStr("Bounds"),
				XorStr("Equals"),
				XorStr("System.Type"));
			return func(this, other);
		}
		bool Equals(Bounds* other)
		{
			static const auto func = (bool(*)(Bounds*, Bounds*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Boolean"),
				XorStr("UnityEngine"),
				XorStr("Bounds"),
				XorStr("Equals"),
				XorStr("UnityEngine.Bounds"));
			return func(this, other);
		}

		vec3 get_center()
		{
			static const auto func = (vec3(*)(Bounds*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Vector3"),
				XorStr("UnityEngine"),
				XorStr("Bounds"),
				XorStr("get_center"),
				XorStr(""));
			return func(this);
		}
		void set_center(const vec3& center)
		{
			static const auto func = (void(*)(Bounds*, vec3))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Bounds"),
				XorStr("set_center"),
				XorStr("UnityEngine.Vector3"));
			return func(this, center);
		}

		vec3 get_size()
		{
			static const auto func = (vec3(*)(Bounds*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Vector3"),
				XorStr("UnityEngine"),
				XorStr("Bounds"),
				XorStr("get_size"),
				XorStr(""));
			return func(this);
		}
		void set_size(const vec3& size)
		{
			static const auto func = (void(*)(Bounds*, vec3))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Bounds"),
				XorStr("set_size"),
				XorStr("UnityEngine.Vector3"));
			return func(this, size);
		}

		vec3 get_extents()
		{
			static const auto func = (vec3(*)(Bounds*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Vector3"),
				XorStr("UnityEngine"),
				XorStr("Bounds"),
				XorStr("get_extents"),
				XorStr(""));
			return func(this);
		}
		void set_extents(const vec3& extents)
		{
			static const auto func = (void(*)(Bounds*, vec3))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Bounds"),
				XorStr("set_extents"),
				XorStr("UnityEngine.Vector3"));
			return func(this, extents);
		}

		vec3 get_min()
		{
			static const auto func = (vec3(*)(Bounds*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Vector3"),
				XorStr("UnityEngine"),
				XorStr("Bounds"),
				XorStr("get_min"),
				XorStr(""));
			return func(this);
		}
		vec3 get_max()
		{
			static const auto func = (vec3(*)(Bounds*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Vector3"),
				XorStr("UnityEngine"),
				XorStr("Bounds"),
				XorStr("get_max"),
				XorStr(""));
			return func(this);
		}

		bool operator==(Bounds* rhs)
		{
			static const auto func = (bool(*)(Bounds*, Bounds*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Boolean"),
				XorStr("UnityEngine"),
				XorStr("Bounds"),
				XorStr("op_Equality"),
				XorStr("UnityEngine.Bounds, UnityEngine.Bounds"));
			return func(this, rhs);
		}
		bool operator!=(Bounds* rhs)
		{
			static const auto func = (bool(*)(Bounds*, Bounds*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Boolean"),
				XorStr("UnityEngine"),
				XorStr("Bounds"),
				XorStr("op_Inequality"),
				XorStr("UnityEngine.Bounds, UnityEngine.Bounds"));
			return func(this, rhs);
		}

		void SetMinMax(const vec3& min, const vec3& max)
		{
			static const auto func = (void(*)(Bounds*, vec3, vec3))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Bounds"),
				XorStr("SetMinMax"),
				XorStr("UnityEngine.Vector3, UnityEngine.Vector3"));
			return func(this, min, max);
		}

		void Encapsulate(const vec3& point)
		{
			static const auto func = (void(*)(Bounds*, vec3))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Bounds"),
				XorStr("Encapsulate"),
				XorStr("UnityEngine.Vector3"));
			return func(this, point);
		}

		std::string ToString()
		{
			static const auto func = (generic_string*(*)(Bounds*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.String"),
				XorStr("UnityEngine"),
				XorStr("Bounds"),
				XorStr("ToString"),
				XorStr(""));
			return il2cpp::generic_to_string(func(this));
		}
	};

	struct Shader
	{
		Shader() = delete;
		static Shader* _Shader()
		{
			static const auto func = (Shader*(*)())il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Shader"),
				XorStr(".ctor"),
				XorStr(""));
			return func();
		}

		static Shader* Find(const std::string& name)
		{
			static const auto func = (Shader * (*)(generic_string*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Shader"),
				XorStr("UnityEngine"),
				XorStr("Shader"),
				XorStr("Find"),
				XorStr("System.String"));
			return func(il2cpp::string_to_generic(name));
		}
	};

	struct Material
	{
		Material() = delete;
		static Material* _Material()
		{
			static const auto func = (Material*(*)())il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Material"),
				XorStr(".ctor"),
				XorStr(""));
			return func();
		}
		static Material* _Material(Shader* shader)
		{
			static const auto func = (Material*(*)(Shader*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Material"),
				XorStr(".ctor"),
				XorStr("UnityEngine.Shader"));
			return func(shader);
		}
		static Material* _Material(const std::string& contents)
		{
			static const auto func = (Material*(*)(generic_string*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Material"),
				XorStr(".ctor"),
				XorStr("System.String"));
			return func(il2cpp::string_to_generic(contents));
		}

		Shader* get_shader()
		{
			static const auto func = (Shader*(*)(Material*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Shader"),
				XorStr("UnityEngine"),
				XorStr("Material"),
				XorStr("get_shader"),
				XorStr(""));
			return func(this);
		}
		void set_shader(Shader* shader)
		{
			static const auto func = (void(*)(Material*, Shader*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Material"),
				XorStr("set_shader"),
				XorStr("UnityEngine.Shader"));
			return func(this, shader);
		}

		void set_color(const Color& color)
		{
			static const auto func = (void(*)(Material*, Color))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Material"),
				XorStr("set_color"),
				XorStr("UnityEngine.Color"));
			return func(this, color);
		}

		Type get_mainTexture()
		{
			static const auto func = (Type(*)(Material*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Texture"),
				XorStr("UnityEngine"),
				XorStr("Material"),
				XorStr("get_mainTexture"),
				XorStr(""));
			return func(this);
		}
		void set_mainTexture(Type mainTexture)
		{
			static const auto func = (void(*)(Material*, Type))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Material"),
				XorStr("set_mainTexture"),
				XorStr("UnityEngine.Texture"));
			return func(this, mainTexture);
		}

		void set_renderQueue(const int& renderQueue)
		{
			static const auto func = (void(*)(Material*, int))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Material"),
				XorStr("set_renderQueue"),
				XorStr("System.Int32"));
			return func(this, renderQueue);
		}

		int get_passCount()
		{
			static const auto func = (int(*)(Material*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Int32"),
				XorStr("UnityEngine"),
				XorStr("Material"),
				XorStr("get_passCount"),
				XorStr(""));
			return func(this);
		}

		void SetColor(const std::string& name, const Color& value)
		{
			static const auto func = (void(*)(Material*, generic_string*, Color))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Material"),
				XorStr("SetColor"),
				XorStr("System.String, UnityEngine.Color"));
			return func(this, il2cpp::string_to_generic(name), value);
		}
		void SetColor(const int& nameID, const Color& value)
		{
			static const auto func = (void(*)(Material*, int, Color))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Material"),
				XorStr("SetColor"),
				XorStr("System.Int32, UnityEngine.Color"));
			return func(this, nameID, value);
		}

		void SetInt(const std::string& name, const int& value)
		{
			static const auto func = (void(*)(Material*, generic_string*, int))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Material"),
				XorStr("SetInt"),
				XorStr("System.String, System.Int32"));
			return func(this, il2cpp::string_to_generic(name), value);
		}
	};

	struct Renderer : Component
	{
		Renderer() = delete;
		static Renderer* _Renderer()
		{
			static const auto func = (Renderer*(*)())il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Renderer"),
				XorStr(".ctor"),
				XorStr(""));
			return func();
		}

		Material* GetMaterial()
		{
			static const auto func = (Material*(*)(Renderer*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Material"),
				XorStr("UnityEngine"),
				XorStr("Renderer"),
				XorStr("GetMaterial"),
				XorStr(""));
			return func(this);
		}
		Material* GetSharedMaterial()
		{
			static const auto func = (Material * (*)(Renderer*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Material"),
				XorStr("UnityEngine"),
				XorStr("Renderer"),
				XorStr("GetSharedMaterial"),
				XorStr(""));
			return func(this);
		}
		void SetMaterial(Material* material)
		{
			static const auto func = (void(*)(Renderer*, Material*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Renderer"),
				XorStr("SetMaterial"),
				XorStr("UnityEngine.Material"));
			return func(this, material);
		}

		bool get_enabled()
		{
			static const auto func = (bool(*)(Renderer*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Boolean"),
				XorStr("UnityEngine"),
				XorStr("Renderer"),
				XorStr("get_enabled"),
				XorStr(""));
			return func(this);
		}
		void set_enabled(const bool& enabled)
		{
			static const auto func = (void(*)(Renderer*, bool))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Renderer"),
				XorStr("set_enabled"),
				XorStr("System.Boolean"));
			return func(this, enabled);
		}

		void set_recieveShadows(const bool& recieveShadows)
		{
			static const auto func = (void(*)(Renderer*, bool))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Renderer"),
				XorStr("set_recieveShadows"),
				XorStr("System.Boolean"));
			return func(this, recieveShadows);
		}

		int get_sortingLayerID()
		{
			static const auto func = (int(*)(Renderer*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Int32"),
				XorStr("UnityEngine"),
				XorStr("Renderer"),
				XorStr("get_sortingLayerID"),
				XorStr(""));
			return func(this);
		}
		void set_sortingLayerID(const int& sortingLayer)
		{
			static const auto func = (void(*)(Renderer*, int))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Renderer"),
				XorStr("set_sortingLayerID"),
				XorStr("System.Int32"));
			return func(this, sortingLayer);
		}

		int GetMaterialCount()
		{
			static const auto func = (int(*)(Renderer*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Int32"),
				XorStr("UnityEngine"),
				XorStr("Renderer"),
				XorStr("get_sortingLayerID"),
				XorStr(""));
			return func(this);
		}

		void set_materials(generic_array_t<Material*>* materials)
		{
			static const auto func = (void(*)(Renderer*, generic_array_t<Material*>*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Renderer"),
				XorStr("set_materials"),
				XorStr("UnityEngine.Material[]"));
			return func(this, materials);
		}

		Material* get_material()
		{
			static const auto func = (Material*(*)(Renderer*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Material"),
				XorStr("UnityEngine"),
				XorStr("Renderer"),
				XorStr("get_material"),
				XorStr(""));
			return func(this);
		}
		void set_material(Material* material)
		{
			static const auto func = (void(*)(Renderer*, Material*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Renderer"),
				XorStr("set_material"),
				XorStr("Material"));
			return func(this, material);
		}
	};

	struct Ray
	{
		vec3 m_Origin;
		vec3 m_Direction;

		Ray() {}
		Ray(const vec3& origin, const vec3& direction) :
			m_Origin(origin), m_Direction(direction) {}

		vec3 GetPoint(const float& distance)
		{
			static const auto func = (vec3(*)(Ray, float))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Vector3"),
				XorStr("UnityEngine"),
				XorStr("Ray"),
				XorStr("GetPoint"),
				XorStr("System.Single"));
			return func(*this, distance);
		}

		std::string ToString()
		{
			static const auto func = (generic_string*(*)(Ray))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.String"),
				XorStr("UnityEngine"),
				XorStr("Ray"),
				XorStr("ToString"),
				XorStr(""));
			return il2cpp::generic_to_string(func(*this));
		}
	};

	struct Camera : Component
	{
		Camera() = delete;
		static Camera* _Camera()
		{
			static const auto func = (Camera*(*)())il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Camera"),
				XorStr(".ctor"),
				XorStr(""));
			return func();
		}

		static Camera* get_main()
		{
			static const auto func = (Camera * (*)())il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Camera"),
				XorStr("UnityEngine"),
				XorStr("Camera"),
				XorStr("get_main"),
				XorStr(""));
			return func();
		}
		static Camera* get_current()
		{
			static const auto func = (Camera * (*)())il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Camera"),
				XorStr("UnityEngine"),
				XorStr("Camera"),
				XorStr("get_current"),
				XorStr(""));
			return func();
		}

		enum class MonoOrStereoscopicEye
		{
			Left,
			Right,
			Mono
		};

		float get_fieldOfView()
		{
			static const auto func = (float(*)(Camera*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Single"),
				XorStr("UnityEngine"),
				XorStr("Camera"),
				XorStr("get_fieldOfView"),
				XorStr(""));
			return func(this);
		}
		void set_fieldOfView(const float& fieldOfView)
		{
			static const auto func = (void(*)(Camera*, float))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Camera"),
				XorStr("set_fieldOfView"),
				XorStr("System.Single"));
			return func(this, fieldOfView);
		}

		Ray ScreenPointToRay(const vec3& pos)
		{
			static const auto func = (Ray(*)(Camera*, vec3))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Ray"),
				XorStr("UnityEngine"),
				XorStr("Camera"),
				XorStr("ScreenPointToRay"),
				XorStr("UnityEngine.Vector3"));
			return func(this, pos);
		}
		Ray ScreenPointToRay(const vec2& pos, const MonoOrStereoscopicEye& eye)
		{
			static const auto func = (Ray(*)(Camera*, vec2, MonoOrStereoscopicEye))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Ray"),
				XorStr("UnityEngine"),
				XorStr("Camera"),
				XorStr("ScreenPointToRay"),
				XorStr("UnityEngine.Vector2, UnityEngine.Camera.MonoOrStereoscopicEye"));
			return func(this, pos, eye);
		}

		vec3 WorldToScreenPoint(const vec3& position)
		{
			static const auto func = (vec3(*)(Camera*, vec3))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Vector3"),
				XorStr("UnityEngine"),
				XorStr("Camera"),
				XorStr("WorldToScreenPoint"),
				XorStr("UnityEngine.Vector3"));
			return func(this, position);
		}
		vec3 WorldToScreenPoint(const vec3& position, const MonoOrStereoscopicEye& eye)
		{
			static const auto func = (vec3(*)(Camera*, vec3, MonoOrStereoscopicEye))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("UnityEngine.Vector3"),
				XorStr("UnityEngine"),
				XorStr("Camera"),
				XorStr("WorldToScreenPoint"),
				XorStr("UnityEngine.Vector3, UnityEngine.Camera.MonoOrStereoscopicEye"));
			return func(this, position, eye);
		}

		void Render()
		{
			static const auto func = (void(*)(Camera*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Camera"),
				XorStr("Render"),
				XorStr(""));
			return func(this);
		}

		void CopyFrom(Camera* other)
		{
			static const auto func = (void(*)(Camera*, Camera*))il2cpp::get_method(XorStr("UnityEngine.CoreModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Camera"),
				XorStr("CopyFrom"),
				XorStr("UnityEngine.Camera"));
			return func(this, other);
		}
	};

	// ---------- PHYSICS ----------

	struct Collider : Component
	{
		Collider() = delete;
		static Collider* _Collider()
		{
			static const auto func = (Collider*(*)())il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Collider"),
				XorStr(".ctor"),
				XorStr(""));
			return func();
		}

		bool get_enabled()
		{
			static const auto func = (bool(*)(Collider*))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Boolean"),
				XorStr("UnityEngine"),
				XorStr("Collider"),
				XorStr("get_enabled"),
				XorStr(""));
			return func(this);
		}
		void set_enabled(const bool& enabled)
		{
			static const auto func = (void(*)(Collider*, bool))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Collider"),
				XorStr("set_enabled"),
				XorStr("System.Boolean"));
			return func(this, enabled);
		}

		Rigidbody* get_attachedRigidbody()
		{
			static const auto func = (Rigidbody*(*)(Collider*))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("UnityEngine.Rigidbody"),
				XorStr("UnityEngine"),
				XorStr("Collider"),
				XorStr("get_attachedRigidbody"),
				XorStr(""));
			return func(this);
		}

		void set_isTrigger(const bool& isTrigger)
		{
			static const auto func = (void(*)(Collider*, bool))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Collider"),
				XorStr("set_isTrigger"),
				XorStr("System.Boolean"));
			return func(this, isTrigger);
		}

		vec3 ClosestPoint(const vec3& position)
		{
			static const auto func = (vec3(*)(Collider*, vec3))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("UnityEngine.Vector3"),
				XorStr("UnityEngine"),
				XorStr("Collider"),
				XorStr("ClosestPoint"),
				XorStr("UnityEngine.Vector3"));
			return func(this, position);
		}

		Bounds* get_bounds()
		{
			static const auto func = (Bounds*(*)(Collider*))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("UnityEngine.Bounds"),
				XorStr("UnityEngine"),
				XorStr("Collider"),
				XorStr("get_bounds"),
				XorStr(""));
			return func(this);
		}

		void set_material(Material* material)
		{
			static const auto func = (void(*)(Collider*, Material*))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Collider"),
				XorStr("set_material"),
				XorStr("UnityEngine.Material"));
			return func(this, material);
		}

		vec3 ClosestPointOnBounds(const vec3& position)
		{
			static const auto func = (vec3(*)(Collider*, vec3))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("UnityEngine.Vector3"),
				XorStr("UnityEngine"),
				XorStr("Collider"),
				XorStr("ClosestPointOnBounds"),
				XorStr("UnityEngine.Vector3"));
			return func(this, position);
		}
	};

	struct Rigidbody : Component
	{
		Rigidbody() = delete;
		static Rigidbody* _Rigidbody()
		{
			static const auto func = (Rigidbody*(*)())il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Rigidbody"),
				XorStr(".ctor"),
				XorStr(""));
			return func();
		}

		vec3 get_velocity()
		{
			static const auto func = (vec3(*)(Rigidbody*))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("UnityEngine.Vector3"),
				XorStr("UnityEngine"),
				XorStr("Rigidbody"),
				XorStr("get_velocity"),
				XorStr(""));
			return func(this);
		}
		void set_velocity(const vec3& velocity)
		{
			static const auto func = (void(*)(Rigidbody*, vec3))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Rigidbody"),
				XorStr("set_velocity"),
				XorStr("UnityEngine.Vector3"));
			return func(this, velocity);
		}
		vec3 get_angularVelocity()
		{
			static const auto func = (vec3(*)(Rigidbody*))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("UnityEngine.Vector3"),
				XorStr("UnityEngine"),
				XorStr("Rigidbody"),
				XorStr("get_angularVelocity"),
				XorStr(""));
			return func(this);
		}
		void set_angularVelocity(const vec3& angularVelocity)
		{
			static const auto func = (void(*)(Rigidbody*, vec3))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Rigidbody"),
				XorStr("set_angularVelocity"),
				XorStr("UnityEngine.Vector3"));
			return func(this, angularVelocity);
		}

		float get_drag()
		{
			static const auto func = (float(*)(Rigidbody*))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Single"),
				XorStr("UnityEngine"),
				XorStr("Rigidbody"),
				XorStr("get_drag"),
				XorStr(""));
			return func(this);
		}
		void set_drag(const float& drag)
		{
			static const auto func = (void(*)(Rigidbody*, float))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Rigidbody"),
				XorStr("set_drag"),
				XorStr("System.Single"));
			return func(this, drag);
		}
		void set_angularDrag(const float& angularDrag)
		{
			static const auto func = (void(*)(Rigidbody*, float))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Rigidbody"),
				XorStr("set_angularDrag"),
				XorStr("System.Single"));
			return func(this, angularDrag);
		}

		float get_mass()
		{
			static const auto func = (float(*)(Rigidbody*))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Single"),
				XorStr("UnityEngine"),
				XorStr("Rigidbody"),
				XorStr("get_mass"),
				XorStr(""));
			return func(this);
		}
		void set_mass(const float& mass)
		{
			static const auto func = (void(*)(Rigidbody*, float))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Rigidbody"),
				XorStr("set_mass"),
				XorStr("System.Single"));
			return func(this, mass);
		}

		void set_useGravity(const bool& useGravity)
		{
			static const auto func = (void(*)(Rigidbody*, bool))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Rigidbody"),
				XorStr("set_useGravity"),
				XorStr("System.Boolean"));
			return func(this, useGravity);
		}

		bool get_isKinematic()
		{
			static const auto func = (bool(*)(Rigidbody*))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Boolean"),
				XorStr("UnityEngine"),
				XorStr("Rigidbody"),
				XorStr("get_isKinematic"),
				XorStr(""));
			return func(this);
		}
		void set_isKinematic(const bool& isKinematic)
		{
			static const auto func = (void(*)(Rigidbody*, bool))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Rigidbody"),
				XorStr("set_isKinematic"),
				XorStr("System.Boolean"));
			return func(this, isKinematic);
		}

		vec3 get_position()
		{
			static const auto func = (vec3(*)(Rigidbody*))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("UnityEngine.Vector3"),
				XorStr("UnityEngine"),
				XorStr("Rigidbody"),
				XorStr("get_position"),
				XorStr(""));
			return func(this);
		}
		void set_position(const vec3& position)
		{
			static const auto func = (void(*)(Rigidbody*, vec3))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Rigidbody"),
				XorStr("set_position"),
				XorStr("UnityEngine.Vector3"));
			return func(this, position);
		}

		Quaternion get_rotation()
		{
			static const auto func = (Quaternion(*)(Rigidbody*))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("UnityEngine.Quaternion"),
				XorStr("UnityEngine"),
				XorStr("Rigidbody"),
				XorStr("get_rotation"),
				XorStr(""));
			return func(this);
		}
		void set_rotation(const Quaternion& rotation)
		{
			static const auto func = (void(*)(Rigidbody*, Quaternion))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Rigidbody"),
				XorStr("set_rotation"),
				XorStr("UnityEngine.Quaternion"));
			return func(this, rotation);
		}

		void MovePosition(const vec3& position)
		{
			static const auto func = (void(*)(Rigidbody*, vec3))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Rigidbody"),
				XorStr("MovePosition"),
				XorStr("UnityEngine.Vector3"));
			return func(this, position);
		}

		void MoveRotation(const Quaternion& rot)
		{
			static const auto func = (void(*)(Rigidbody*, Quaternion))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Rigidbody"),
				XorStr("MoveRotation"),
				XorStr("UnityEngine.Quaternion"));
			return func(this, rot);
		}

		void AddForce(const vec3& force)
		{
			static const auto func = (void(*)(Rigidbody*, vec3))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Void"),
				XorStr("UnityEngine"),
				XorStr("Rigidbody"),
				XorStr("AddForce"),
				XorStr("UnityEngine.Vector3"));
			return func(this, force);
		}
	};

	struct RaycastHit
	{
		vec3 m_Point;
		vec3 m_Normal;
		unsigned int m_FaceID;
		float m_Distance;
		vec2 m_UV;
		int m_Collider;

		Collider* get_collider()
		{
			return (Collider*)Object::FindObjectFromInstanceID(m_Collider);
		}

		Rigidbody* get_rigidbody()
		{
			return get_collider()->get_attachedRigidbody();
		}

		Transform* get_transform()
		{
			if (get_rigidbody())
				return get_rigidbody()->get_transform();

			return get_collider()->get_transform();
		}
	};

	struct Physics
	{
		Physics() = delete;

		enum class QueryTriggerInteraction
		{
			UseGlobal,
			Ignore,
			Collide
		};

		static vec3 get_gravity()
		{
			static const auto func = (vec3(*)())il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("UnityEngine.Vector3"),
				XorStr("UnityEngine"),
				XorStr("Physics"),
				XorStr("get_gravity"),
				XorStr(""));
			return func();
		}

		static bool Raycast(const Ray& ray)
		{
			static const auto func = (bool(*)(Ray))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Boolean"),
				XorStr("UnityEngine"),
				XorStr("Physics"),
				XorStr("Raycast"),
				XorStr("UnityEngine.Ray"));
			return func(ray);
		}
		static bool Raycast(const vec3& origin, const vec3& direction)
		{
			static const auto func = (bool(*)(vec3, vec3))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Boolean"),
				XorStr("UnityEngine"),
				XorStr("Physics"),
				XorStr("Raycast"),
				XorStr("UnityEngine.Vector3, UnityEngine.Vector3"));
			return func(origin, direction);
		}
		static bool Raycast(const Ray& ray, const float& maxDistance)
		{
			static const auto func = (bool(*)(Ray, float))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Boolean"),
				XorStr("UnityEngine"),
				XorStr("Physics"),
				XorStr("Raycast"),
				XorStr("UnityEngine.Ray, System.Single"));
			return func(ray, maxDistance);
		}
		static bool Raycast(const Ray& ray, RaycastHit& hitInfo)
		{
			static const auto func = (bool(*)(Ray, RaycastHit*))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Boolean"),
				XorStr("UnityEngine"),
				XorStr("Physics"),
				XorStr("Raycast"),
				XorStr("UnityEngine.Ray, UnityEngine.RaycastHit&"));
			return func(ray, &hitInfo);
		}
		static bool Raycast(const vec3& origin, const vec3& direction, const float& maxDistance)
		{
			static const auto func = (bool(*)(vec3, vec3, float))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Boolean"),
				XorStr("UnityEngine"),
				XorStr("Physics"),
				XorStr("Raycast"),
				XorStr("UnityEngine.Vector3, UnityEngine.Vector3, System.Single"));
			return func(origin, direction, maxDistance);
		}
		static bool Raycast(const vec3& origin, const vec3& direction, RaycastHit& hitInfo)
		{
			static const auto func = (bool(*)(vec3, vec3, RaycastHit*))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Boolean"),
				XorStr("UnityEngine"),
				XorStr("Physics"),
				XorStr("Raycast"),
				XorStr("UnityEngine.Vector3, UnityEngine.Vector3, UnityEngine.RaycastHit&"));
			return func(origin, direction, &hitInfo);
		}
		static bool Raycast(const Ray& ray, const float& maxDistance, const int& layerMask)
		{
			static const auto func = (bool(*)(Ray, float, int))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Boolean"),
				XorStr("UnityEngine"),
				XorStr("Physics"),
				XorStr("Raycast"),
				XorStr("UnityEngine.Ray, System.Single, System.Int32"));
			return func(ray, maxDistance, layerMask);
		}
		static bool Raycast(const Ray& ray, RaycastHit& hitInfo, const float& maxDistance)
		{
			static const auto func = (bool(*)(Ray, RaycastHit*, float))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Boolean"),
				XorStr("UnityEngine"),
				XorStr("Physics"),
				XorStr("Raycast"),
				XorStr("UnityEngine.Ray, UnityEngine.RaycastHit&, System.Single"));
			return func(ray, &hitInfo, maxDistance);
		}
		static bool Raycast(const vec3& origin, const vec3& direction, const float& maxDistance, const int& layerMask)
		{
			static const auto func = (bool(*)(vec3, vec3, float, int))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Boolean"),
				XorStr("UnityEngine"),
				XorStr("Physics"),
				XorStr("Raycast"),
				XorStr("UnityEngine.Vector3, UnityEngine.Vector3, System.Single, System.Int32"));
			return func(origin, direction, maxDistance, layerMask);
		}
		static bool Raycast(const vec3& origin, const vec3& direction, RaycastHit& hitInfo, const float& maxDistance)
		{
			static const auto func = (bool(*)(vec3, vec3, RaycastHit*, float))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Boolean"),
				XorStr("UnityEngine"),
				XorStr("Physics"),
				XorStr("Raycast"),
				XorStr("UnityEngine.Vector3, UnityEngine.Vector3, UnityEngine.RaycastHit&, System.Single"));
			return func(origin, direction, &hitInfo, maxDistance);
		}
		static bool Raycast(const Ray& ray, RaycastHit& hitInfo, const float& maxDistance, const int& layerMask)
		{
			static const auto func = (bool(*)(Ray, RaycastHit*, float, int))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Boolean"),
				XorStr("UnityEngine"),
				XorStr("Physics"),
				XorStr("Raycast"),
				XorStr("UnityEngine.Ray, UnityEngine.RaycastHit&, System.Single, System.Int32"));
			return func(ray, &hitInfo, maxDistance, layerMask);
		}
		static bool Raycast(const vec3& origin, const vec3& direction, RaycastHit& hitInfo, const float& maxDistance, const int& layerMask)
		{
			static const auto func = (bool(*)(vec3, vec3, RaycastHit*, float, int))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Boolean"),
				XorStr("UnityEngine"),
				XorStr("Physics"),
				XorStr("Raycast"),
				XorStr("UnityEngine.Vector3, UnityEngine.Vector3, UnityEngine.RaycastHit&, System.Single, System.Int32"));
			return func(origin, direction, &hitInfo, maxDistance, layerMask);
		}
		static bool Raycast(const vec3& origin, const vec3& direction, RaycastHit& hitInfo, const float& maxDistance, const int& layerMask, const QueryTriggerInteraction& queryTriggerInteraction)
		{
			static const auto func = (bool(*)(vec3, vec3, RaycastHit*, float, int, QueryTriggerInteraction))il2cpp::get_method(XorStr("UnityEngine.PhysicsModule"),
				XorStr("System.Boolean"),
				XorStr("UnityEngine"),
				XorStr("Physics"),
				XorStr("Raycast"),
				XorStr("UnityEngine.Vector3, UnityEngine.Vector3, UnityEngine.RaycastHit&, System.Single, System.Int32, UnityEngine.QueryTriggerInteraction"));
			return func(origin, direction, &hitInfo, maxDistance, layerMask, queryTriggerInteraction);
		}
	};
};
