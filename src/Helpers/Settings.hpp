#pragma once

#include <GL/glew.h>

#include <utility>
#include <string>
#include <mutex>

class Settings
{
private:
	static Settings *instance;
	static std::mutex mutex;

	std::pair<int, int> startupResolution;

	GLfloat cameraSpeed;
	GLfloat cameraFoV;

	bool vSyncEnabled;
	bool depthBufferEnabled;
	bool resizeEnabled;

	std::string windowName;

protected:
	Settings();

public:
	Settings(Settings &other) = delete;
	void operator=(const Settings &) = delete;

	static Settings *get();

	std::pair<int, int> getStartupResolution();

	GLfloat getCameraSpeed();
	GLfloat getCameraFoV();

	bool isVSyncEnabled();
	bool isDepthBufferEnabled();
	bool isResizeEnabled();

	const std::string &getWindowName();
};

inline std::pair<int, int> Settings::getStartupResolution()
{
	return startupResolution;
}

inline GLfloat Settings::getCameraSpeed()
{
	return cameraSpeed;
}

inline GLfloat Settings::getCameraFoV()
{
	return cameraFoV;
}

inline bool Settings::isVSyncEnabled()
{
	return vSyncEnabled;
}

inline bool Settings::isDepthBufferEnabled()
{
	return depthBufferEnabled;
}

inline bool Settings::isResizeEnabled()
{
	return resizeEnabled;
}

inline const std::string &Settings::getWindowName()
{
	return windowName;
}
