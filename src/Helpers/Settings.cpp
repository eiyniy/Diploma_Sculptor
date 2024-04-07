#include <Settings.hpp>

// IWYU pragma: begin_keep
#include <glm/glm.hpp>
#include <glm/ext.hpp>
// IWYU pragma: end_keep

Settings *Settings::instance{nullptr};
std::mutex Settings::mutex{};

Settings::Settings()
	: startupResolution{1280, 720},
	  cameraSpeed{3.f},
	  cameraFoV(glm::radians(45.0f)),
	  vSyncEnabled(true),
	  depthBufferEnabled(true),
	  resizeEnabled(false),
	  windowName("Sculptor")
{
}

Settings *Settings::get()
{
	std::lock_guard<std::mutex> lock(mutex);
	if (instance == nullptr)
		instance = new Settings();

	return instance;
}
