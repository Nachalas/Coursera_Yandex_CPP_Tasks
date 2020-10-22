#include "Common.h"
#include <algorithm>

using namespace std;

class Ellipse : public IShape {
	unique_ptr<IShape> Clone() const override {
		unique_ptr<IShape> ptr = make_unique<Ellipse>();
		ptr->SetPosition(position);
		ptr->SetSize(size);
		ptr->SetTexture(texture);
		return ptr;
	}

	void SetPosition(Point p) override {
		position = move(p);
	}

	Point GetPosition() const override {
		return position;
	}

	void SetSize(Size s) override {
		size = move(s);
	}

	Size GetSize() const override {
		return size;
	}

	void SetTexture(std::shared_ptr<ITexture> t) override {
		texture = move(t);
	}

	ITexture* GetTexture() const override {
		if (texture) {
			return texture.get();
		}
		else {
			return nullptr;
		}
	}

	void Draw(Image& img) const override {
		Image tempTxtrImg;
		if (texture) {
			tempTxtrImg = texture->GetImage();
		}
		tempTxtrImg.resize(size.height);
		for (auto& i : tempTxtrImg) {
			i.resize(size.width, '\z');
		}
		//for (int y = 0; y < size.height; y++) {
		//	for (int x = 0; x < size.width; x++) {
		//		if (!IsPointInEllipse({ x , y }, size)) {
		//			tempTxtrImg[y][x] = ' ';
		//		}
		//	}
		//}
		for (int y = position.y; y < min(position.y + size.height, (int)img.size()); y++) {
			for (int x = position.x; x < min(position.x + size.width, (int)img[0].size()); x++) {
				if (IsPointInEllipse({ x - position.x, y - position.y }, size)) {
					img[y][x] = tempTxtrImg[y - position.y][x - position.x] == '\z' ? '.' : tempTxtrImg[y - position.y][x - position.x];
				}
			}
		}
	}

private:
	Point position;
	Size size;
	shared_ptr<ITexture> texture;
};

class Rectangle : public IShape {
	unique_ptr<IShape> Clone() const override {
		unique_ptr<IShape> ptr = make_unique<Rectangle>();
		ptr->SetPosition(position);
		ptr->SetSize(size);
		ptr->SetTexture(texture);
		return ptr;
	}

	void SetPosition(Point p) override {
		position = move(p);
	}

	Point GetPosition() const override {
		return position;
	}

	void SetSize(Size s) override {
		size = move(s);
	}

	Size GetSize() const override {
		return size;
	}

	void SetTexture(std::shared_ptr<ITexture> t) override {
		texture = move(t);
	}

	ITexture* GetTexture() const override {
		if (texture) {
			return texture.get();
		}
		else {
			return nullptr;
		}
	}

	//bool belongsToTxtr(int x, int y, Image& txtr) const {
	//	if (((y - position.y) < txtr.size()) && ((x - position.x) < txtr[0].size())) {
	//		return true;
	//	}
	//	else {
	//		return false;
	//	}
	//}

	void Draw(Image& img) const override {
		//Point upleft = position;
		//Point upright = {position.x + size.width, position.y};
		//Point lowleft = {position.x, position.y + size.height};
		//Point lowright = { position.x + size.width, position.y + size.height };
		Image tempTxtrImg;
		if (texture) {
			tempTxtrImg = texture->GetImage();
		}
		tempTxtrImg.resize(size.height);
		for (auto& i : tempTxtrImg) {
			i.resize(size.width, '.');
		}
		for (int y = position.y; y < min(position.y + size.height, (int)img.size()); y++) {
			for (int x = position.x; x < min(position.x + size.width, (int)img[0].size()); x++) {
				img[y][x] = tempTxtrImg[y - position.y][x - position.x];
			}
		}
		/*if (texture) {
			Image tempTxtrImg = texture->GetImage();
			for (int y = position.y; y <= position.y + size.height; y++) {
				for (int x = position.x; x <= position.x + size.width; x++) {
					if (belongsToTxtr(x, y, tempTxtrImg)) {
						img[y][x] = tempTxtrImg[y - position.y][x - position.x];
					}
					else {
						img[y][x] = '.';
					}
				}
			}
		}*/
	}

private:
	Point position;
	Size size;
	shared_ptr<ITexture> texture;
};

unique_ptr<IShape> MakeShape(ShapeType shape_type) {
	switch (shape_type) {
	case ShapeType::Ellipse:
		return make_unique<Ellipse>();
	case ShapeType::Rectangle:
		return make_unique<Rectangle>();
	}
}