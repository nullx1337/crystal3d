#include "Script.h"

namespace Scripting
{

	CrScript::CrScript(const IO::CrPath& a_File)
	{
		auto path = Resources::ResolvePath(a_File);
		m_Script = m_State.load_file(path.string());
		CrFramework::RegisterTypes(m_State);
	}

	CrScript::~CrScript()
	{
	}

	void CrScript::Run()
	{
		try
		{
			m_Script();
		}
		catch (sol::error err)
		{
			CrAssert(0, err.what());
		}
	}
}
