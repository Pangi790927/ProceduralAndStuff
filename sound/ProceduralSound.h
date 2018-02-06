#ifndef PROCEDURAL_SOUND_H
#define PROCEDURAL_SOUND_H

#include <Util.h>
#include "SoundBuffer.h"
#include "RandGenerator.h"

class ProceduralSound : public SoundBuffer {
public:
	virtual void abstract() {} // implementation of abstract

	ProceduralSound (int time = 40) {
		SoundBuffer::time = time;
		data = std::shared_ptr<short int> (new short int[freq * time], Util::array_deleter<short int>());

		for (int i = 0; i < freq * time; i++)
			data.get()[i] = 0;
	}

	void addNote (float timeLoc, float note_freq, float duration) {
		namespace RG = RandGenerator;

		float ampl = max_ampl * 0.50;
		float timeConst = -std::log(1 / 100.) / duration;

		data.get()[0] = max_ampl;
		for (int i = 0; i < freq * time; i++) {
			if ((i / float(freq)) > timeLoc) {
				RG::srand(note_freq);
				float x = (i / float(freq) - timeLoc);
				data.get()[i] += (sin(x * 2 * 3.141592653589 * (float)note_freq))  
						* ampl * (std::exp(-x * timeConst) - 
								std::exp(-x * timeConst * (RG::randNorm() * 10 + 5)));
			}
		}
	}

	void addSong (unsigned char *song, int lenght) {
		auto notes = [](int note) {
			return pow(2.0, (note - 49) / 12.0) * 440;
		};

		int DO = 37;
		int RE = 39;
		int MI = 40;
		int FA = 42;
		int SO = 44;
		int LA = 45;
		int SI = 47;

		int note[] = {0, DO, RE, MI, FA, SO, LA, SI};

		float nextNote = 0;
		for (int i = 0; i < lenght; i++) {
			if (song[i] != 255)
				addNote(nextNote, notes(note[song[i]]), 1.5);
			
			nextNote += 200 / 1000.;
		}
	}
};

#endif