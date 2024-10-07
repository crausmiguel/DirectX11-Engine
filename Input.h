#pragma once

class Input {
public:
	Input();
	Input(const Input& src);
	~Input();


	void KeyDown(unsigned int);
	void KeyUp(unsigned int);
	bool IsKeyDown(unsigned int);

private:
	bool m_Keys[256] = { false };
};