#include "GLRenderEntity.h"
#include "GLRenderer.h"
#include "core\Engine.h"

namespace Graphics
{
	namespace OpenGL
	{
		GLRenderEntity::GLRenderEntity(GLVertexArray* a_VAO, GLShaderProgram* a_Program, GLenum a_Mode, Scene::CrTransform* a_Transform, Math::AABB* a_BoundingBox)
			: m_Mode(a_Mode)
		{
			this->SetVertexArray(a_VAO);
			this->SetProgram(a_Program);
			this->SetMode(a_Mode);
			this->SetTransform(a_Transform);
			this->SetBoundingBox(a_BoundingBox);
		}

		GLRenderEntity::~GLRenderEntity()
		{
			delete m_ShaderProgram;
			delete m_VertexArray;
		}

		void GLRenderEntity::SetTransform(Scene::CrTransform * a_Transform)
		{
			m_Transform = a_Transform;
		}

		void GLRenderEntity::SetProgram(GLShaderProgram* a_Program)
		{
			if (m_ShaderProgram != nullptr)
				delete m_ShaderProgram;

			m_ShaderProgram = a_Program;
			GLRenderer::m_UniformMVPBuffer->Bind(m_ShaderProgram->GetHandle());
		}

		void GLRenderEntity::SetVertexArray(GLVertexArray* a_VertexArray)
		{
			if (m_VertexArray != nullptr)
				delete m_VertexArray;

			m_VertexArray = a_VertexArray;
		}

		void GLRenderEntity::SetBoundingBox(Math::AABB * a_BoundingBox)
		{
			m_BoundingBox = a_BoundingBox;
		}

		void GLRenderEntity::SetMode(GLenum a_Mode)
		{
			m_Mode = a_Mode;
		}

		void GLRenderEntity::Render()
		{
			m_ShaderProgram->Bind();
			this->BufferTransform();

			glPushAttrib(GL_POLYGON_BIT);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				
			m_VertexArray->Bind();
			glDrawElements(m_Mode, m_VertexArray->m_ElementCount, GL_UNSIGNED_INT, 0);
			m_VertexArray->Unbind();

			glPopAttrib();
			m_ShaderProgram->Unbind();
		}

		void GLRenderEntity::BufferTransform()
		{
			if (m_Transform != nullptr)
			{
				glm::mat4 worldMatrix = m_Transform->WorldMatrix();
				GLRenderer::m_UniformMVPBuffer->Subdata(&worldMatrix, 0);
			}
		}

	}
}
