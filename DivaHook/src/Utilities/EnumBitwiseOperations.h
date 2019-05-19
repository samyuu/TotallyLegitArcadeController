#pragma once

namespace DivaHook::Utilities
{
	template<class T> inline T operator~ (T a) { return (T)~(int)a; }
	template<class T> inline T operator| (T a, T b) { return (T)((int)a | (int)b); }
	template<class T> inline T operator& (T a, T b) { return (T)((int)a & (int)b); }
	template<class T> inline T operator^ (T a, T b) { return (T)((int)a ^ (int)b); }
	template<class T> inline T& operator|= (T& a, T b) { return (T&)((int&)a |= (int)b); }
	template<class T> inline T& operator&= (T& a, T b) { return (T&)((int&)a &= (int)b); }
	template<class T> inline T& operator^= (T& a, T b) { return (T&)((int&)a ^= (int)b); }
}