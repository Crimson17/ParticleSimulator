#include "KrimzLib/renderer/skybox.h"


// Skybox box vertices
const std::vector<kl::vertex> boxVertices {
	kl::vertex(kl::float3( 1, -1,  1)), kl::vertex(kl::float3( 1, -1, -1)), kl::vertex(kl::float3( 1,  1, -1)),
	kl::vertex(kl::float3( 1,  1, -1)), kl::vertex(kl::float3( 1,  1,  1)), kl::vertex(kl::float3( 1, -1,  1)),
	kl::vertex(kl::float3(-1, -1,  1)), kl::vertex(kl::float3(-1,  1,  1)), kl::vertex(kl::float3(-1,  1, -1)),
	kl::vertex(kl::float3(-1,  1, -1)), kl::vertex(kl::float3(-1, -1, -1)), kl::vertex(kl::float3(-1, -1,  1)),
	kl::vertex(kl::float3(-1,  1,  1)), kl::vertex(kl::float3( 1,  1,  1)), kl::vertex(kl::float3( 1,  1, -1)),
	kl::vertex(kl::float3( 1,  1, -1)), kl::vertex(kl::float3(-1,  1, -1)), kl::vertex(kl::float3(-1,  1,  1)),
	kl::vertex(kl::float3(-1, -1,  1)), kl::vertex(kl::float3(-1, -1, -1)), kl::vertex(kl::float3( 1, -1, -1)),
	kl::vertex(kl::float3( 1, -1, -1)), kl::vertex(kl::float3( 1, -1,  1)), kl::vertex(kl::float3(-1, -1,  1)),
	kl::vertex(kl::float3(-1, -1,  1)), kl::vertex(kl::float3( 1, -1,  1)), kl::vertex(kl::float3( 1,  1,  1)),
	kl::vertex(kl::float3( 1,  1,  1)), kl::vertex(kl::float3(-1,  1,  1)), kl::vertex(kl::float3(-1, -1,  1)),
	kl::vertex(kl::float3(-1, -1, -1)), kl::vertex(kl::float3(-1,  1, -1)), kl::vertex(kl::float3( 1,  1, -1)),
	kl::vertex(kl::float3( 1,  1, -1)), kl::vertex(kl::float3( 1, -1, -1)), kl::vertex(kl::float3(-1, -1, -1))
};

// Constructors/destructor
kl::skybox::skybox(ID3D11Device* dev, ID3D11DeviceContext* devcon, const std::string& name, const kl::image& fullbox) {
	// Checking the aspect ratio
	if (fullbox.width() % 4 == 0 && fullbox.height() % 3 == 0) {
		// Getting the part size
		const int partWidth = fullbox.width() / 4;
		const int partHeight = fullbox.height() / 3;

		// Checking the part size
		if (partWidth == partHeight) {
			// Extracting the sides
			const kl::int2 partSize(partWidth, partHeight);
			const kl::image front  = fullbox.rect(partSize * kl::int2(1, 1), partSize * kl::int2(2, 2));
			const kl::image back   = fullbox.rect(partSize * kl::int2(3, 1), partSize * kl::int2(4, 2));
			const kl::image left   = fullbox.rect(partSize * kl::int2(0, 1), partSize * kl::int2(1, 2));
			const kl::image right  = fullbox.rect(partSize * kl::int2(2, 1), partSize * kl::int2(3, 2));
			const kl::image top    = fullbox.rect(partSize * kl::int2(1, 0), partSize * kl::int2(2, 1));
			const kl::image bottom = fullbox.rect(partSize * kl::int2(1, 2), partSize * kl::int2(2, 3));

			// Calling the other constructor
			this->kl::skybox::skybox(dev, devcon, name, front, back, left, right, top, bottom);
		}
		else {
			printf("Skybox image width and height do not match!");
			std::cin.get();
			exit(69);
		}
	}
	else {
		printf("Skybox image does not have the correct aspect ratio!");
		std::cin.get();
		exit(69);
	}
}
kl::skybox::skybox(ID3D11Device* dev, ID3D11DeviceContext* devcon, const std::string& name, const kl::image& front, const kl::image& back, const kl::image& left, const kl::image& right, const kl::image& top, const kl::image& bottom) {
	// Saving the name
	this->name = name;

	// Compiling skybox shaders
	sky_sh = new kl::shaders(dev, devcon, "res/shaders/skybox.hlsl", sizeof(kl::mat4), 0);

	// Generating the box mesh
	box_mes = new kl::mesh(dev, devcon, boxVertices);

	// Generating the box texture
	box_tex = new kl::texture(dev, devcon, front, back, left, right, top, bottom);
}
kl::skybox::~skybox() {
	delete sky_sh;
	delete box_tex;
	delete box_mes;
}

// Renders the cubemap
void kl::skybox::render(const kl::mat4& vpMat) const {
	// Setting skybox vp data
	sky_sh->setVertData(vpMat.pointer());

	// Binding the texture
	box_tex->bind(0);

	// Drawing the cubemap
	box_mes->draw();
}
