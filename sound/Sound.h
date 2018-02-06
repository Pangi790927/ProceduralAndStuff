#ifndef SOUND_H
#define SOUND_H

#include <AL/al.h>
#include <AL/alc.h>

#include "SoundSource.h"

/// try instaling libalut-dev if you can't find openal

namespace Sound
{
	ALCdevice *device;
	ALCcontext *context;

	void init() {
		device = alcOpenDevice(NULL);
		
		if (device) {
			context = alcCreateContext(device, NULL);
			alcMakeContextCurrent(context);
		}

		auto suportedEAX = alIsExtensionPresent("EAX2.0"); // ?? 
		alGetError(); // clear errors
	}

	void playNote (float wave_freq, float duration) {
		unsigned int buffer;

		alGenBuffers(1, &buffer);

		if (alGetError() != AL_NO_ERROR) {
			std::cout << "Sound [gen buffer] error" << std::endl;
		}

		int secs = 10;
		int freq = 44100;
		int size = freq * secs;
		int ampl = 0.50 * 32767.0;

		short int data[size];

		auto ease = [](float x) {
			// return x;
			return (x * x * x * (x * (x * 6 - 15) + 10));
		};

		for (int i = 0; i < size; i++) {
			// data[i] = sin(i * 2 * 3.141592653589 / float(freq) * (float)wave_freq) 
			// 		* ampl * std::max(0.0f, ease((duration - (i / float(freq))) / duration));
			data[i] = sin(i * 2 * 3.141592653589 / float(freq) * (float)wave_freq) 
					* ampl * std::max(0.0f, ease((duration - (i / float(freq))) / duration)) * 
					std::exp(-i / float(freq) / 2.0 / 3.14159);
		}

		alBufferData(buffer, AL_FORMAT_MONO16, (void *)data, sizeof(data), freq);

		unsigned int source;
		alGenSources(1, &source);
		
		if (alGetError() != AL_NO_ERROR) {
			std::cout << "Sound [gen source] error" << std::endl;
		}

		alSourcei(source, AL_BUFFER, buffer);

		if (alGetError() != AL_NO_ERROR) {
			std::cout << "Sound [sourcei source] error" << std::endl;
		}

		alListenerfv(AL_POSITION, Point3f(0, 0, 0).array);
		alListenerfv(AL_VELOCITY, Point3f(0, 0, 0).array);

		float orientation[] = {0.0, 0.0, -1.0, 0.0, 1.0, 0.0};
		alListenerfv(AL_ORIENTATION, orientation);

		if (alGetError() != AL_NO_ERROR) {
			std::cout << "Sound [listener] error" << std::endl;
		}

		alSourcef(source, AL_PITCH, 1.0f);
		alSourcef(source, AL_GAIN, 1.0f);
		alSourcefv(source, AL_POSITION, Point3f(1, 1, 1).array);
		alSourcefv(source, AL_VELOCITY, Point3f(0, 0, 0).array);
		alSourcei(source, AL_LOOPING, AL_FALSE);

		if (alGetError() != AL_NO_ERROR) {
			std::cout << "Sound [sourcef] error" << std::endl;
		}

		alSourcePlay(source);

		if (alGetError() != AL_NO_ERROR) {
			std::cout << "Sound [play] error" << std::endl;
		}

		// alDeleteSources(1, &source);
		// alDeleteBuffers(1, &buffer);
	}

	void free() {
		context = alcGetCurrentContext(); 
		device = alcGetContextsDevice(context); 
		alcMakeContextCurrent(NULL); 
		alcDestroyContext(context); 
		alcCloseDevice(device); 
	}
}

#endif