#pragma once

namespace sle
{
	namespace audio
	{
		SLE_EXPORTS void Initialize();
		SLE_EXPORTS void PlayAudioFile(std::wstring filePath, bool loop = false);
	}
}