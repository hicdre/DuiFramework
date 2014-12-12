#pragma once

#define CAIRO_WIN32_STATIC_BUILD 1
#include "third_party/cairo/src/cairo.h"
#include "third_party/cairo/src/cairo-win32.h"


void InitEngineCairo();
void UnintEngineCairo();

class CairoEngine
{
public:
	static void Init() {
		cairo_startup();
	}

	static void Unint() {
		cairo_shutdown();
	}
};