#ifndef INCLUDE_BOX_COLLIDER
#define INCLUDE_BOX_COLLIDER

#include "Engine/Component/Physics/Collider.h"

namespace GameEngine
{
	class BoxCollider : public Collider
	{
	public:
		BoxCollider ();
		virtual ~BoxCollider ();

		virtual void OnInit () override;

		void SetSize (Vector3 size);
		Vector3 GetSize () const;

		virtual void OnRenderEditor (Editor& editor) override;

	private:
		Vector3 m_size;
	};
}

#endif