#ifndef SOUND_SOURCE_H
#define SOUND_SOURCE_H

#include "SoundBuffer.h"

class SoundSource {
public:
	unsigned int source;

	float volume = 1.0f;
	float pitch = 1.0f;
	Point3f position;
	Point3f velocity;

	void createSource(SoundBuffer& buffer, float* listPosition, float* listVelocity
			, float* listOrientation)
	{
		alGenSources(1, &source);

		if (alGetError() != AL_NO_ERROR) {
			std::cout << "Sound [gen source] error" << std::endl;
		}

		alSourcei(source, AL_BUFFER, buffer.get());

		if (alGetError() != AL_NO_ERROR) {
			std::cout << "Sound [sourcei source] error" << std::endl;
		}

		alListenerfv(AL_POSITION, listPosition);
		alListenerfv(AL_VELOCITY, listVelocity);
		alListenerfv(AL_ORIENTATION, listOrientation);

		if (alGetError() != AL_NO_ERROR) {
			std::cout << "Sound [listener] error" << std::endl;
		}

		alSourcef(source, AL_PITCH, pitch);
		alSourcef(source, AL_GAIN, volume);
		alSourcefv(source, AL_POSITION, position.array);
		alSourcefv(source, AL_VELOCITY, velocity.array);
		alSourcei(source, AL_LOOPING, AL_FALSE);

		if (alGetError() != AL_NO_ERROR) {
			std::cout << "Sound [sourcef] error" << std::endl;
		}
	}

	float getStatef(unsigned int state) {
		float retVal = 0;
		alGetSourcef(source, state, &retVal);
		return retVal;
	}

	void play() {
		alSourcePlay(source);

		if (alGetError() != AL_NO_ERROR) {
			std::cout << "Sound [play] error" << std::endl;
		}
	}
};

#endif