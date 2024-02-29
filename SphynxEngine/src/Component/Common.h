#pragma once

#define COMPONENT_COMMON_BODY(Component) \
public:\
	Component() = default; \
	Component(Component& other) = default; \
	Component(Component&& other) = default; \
	Component& operator=(Component&& other) = default
