
/**
 *  _ShowType.h
 *
 *  Shows the type of the expression due to compilation errors
 *  For debugging purposes
 *
 */

#pragma once

template<typename T>
class _ShowType;

#define __showType(x) _ShowType<decltype(x)>();
