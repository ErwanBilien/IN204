#ifndef PICTURE
#define PICTURE

class Picture {
protected:
	int width;
	int height;

public:
	Picture() : width(500), height(500) {}
	Picture(int w, int h) : width(w), height(h) {}

	void setWidth(int w)
	{
		width = w;
	}
	void setHeight(int h)
	{
		height = h;
	}
	int getWidth()
	{
		return width;
	}
	int getHeight()
	{
		return height;
	}
};
#endif