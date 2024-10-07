#include "Input.h"

Input::Input(){}
Input::Input(const Input& src){}
Input::~Input(){}

void Input::KeyDown(unsigned int input) {
	m_Keys[input] = true;
}

void Input::KeyUp(unsigned int input) {
	m_Keys[input] = false;
}

bool Input::IsKeyDown(unsigned int key) {
	return m_Keys[key];
}