#ifndef SOUND_BUFFER_H
#define SOUND_BUFFER_H

#include <AL/al.h>
#include <AL/alc.h>

class SoundBuffer {
public:
	const int max_ampl = 32767;

	int freq = 44100;
	int time = 0;

	std::shared_ptr<short int> data;

	unsigned int buffer;

	virtual void abstract() = 0;

	void createBuffer() {
		alGenBuffers(1, &buffer);

		if (alGetError() != AL_NO_ERROR) {
			DEBUG("Sound [gen buffer] error");
		}

		alBufferData(buffer, AL_FORMAT_MONO16, (void *)data.get(), freq * time * sizeof(short int), freq);
	}

	void draw (ShaderProgram& prog) {
		prog.setMatrix("worldMatrix", GameMesh::topMatrixStack());

		glBegin(GL_POINTS);
			for (int i = 1; i < freq * time; i++) {
				glVertex3f(float(i) / freq, data.get()[i] / float(max_ampl), 0);
				glVertex3f(float(i - 1) / freq, data.get()[i - 1] / float(max_ampl), 0);
			}
		glEnd();
	}

	unsigned int get() {
		return buffer;
	}

	~SoundBuffer() {
		alDeleteBuffers(1, &buffer);
	}
};

#endif