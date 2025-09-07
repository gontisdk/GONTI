#pragma once

#include <GONTI/GONTI-ENGINE/GONTI.RUNTIME/Source/Events/Events.h>
#include <GONTI/GONTI-ENGINE/GONTI.RUNTIME/Source/Events/InputSystem/Inputs.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Logging/Logger.h>
#include <GONTI/GONTI-ENGINE/GONTI.RUNTIME/Source/Application/Vulkan/VulkanApplication.h>
#include <GONTI/GONTI-ENGINE/GONTI.RENDER/Source/Renderer/Frontend/RenderFrontend.h>

b8 applicationOnEvent(u16 code, void* sender, void* listenerInst, GontiEventContext context);
b8 applicationOnKey(u16 code, void* sender, void* listenerInst, GontiEventContext context);
b8 applicationOnButton(u16 code, void* sender, void* listenerInst, GontiEventContext context);
b8 applicationOnMouseWheel(u16 code, void* sender, void* listenerInst, GontiEventContext context);
b8 applicationOnMouseMove(u16 code, void* sender, void* listenerInst, GontiEventContext context);
b8 applicationOnResized(u16 code, void* sender, void* listenerInst, GontiEventContext context);