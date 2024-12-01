#pragma once

#include <d3d11.h>


#define CHECK_D3D11_ERRORS(FuncCall) 	{\
											HRESULT error = FuncCall;\
											if (FAILED(error))\
											{\
												SPX_CORE_LOG_ERROR("Call: {}", #FuncCall);\
												SPX_CORE_LOG_ERROR("Error: {}", error);\
												__debugbreak();\
											}\
										}	

#define CHECK_D3D11_FAILED(FuncCall) 	(FAILED(FuncCall))
