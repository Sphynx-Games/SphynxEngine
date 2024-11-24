#pragma once

#define COMPONENT_COMMON_BODY(Component) \
public:\
	Component() = default; \
	Component(const Component& other) = default; \
	Component(Component&& other) = default; \
	Component& operator=(Component&& other) = default

#define COMPONENT_COMMON_BODY_NO_DEFAULT_CONSTRUCTOR(Component) \
public:\
	Component(const Component& other) = default; \
	Component(Component&& other) = default; \
	Component& operator=(Component&& other) = default