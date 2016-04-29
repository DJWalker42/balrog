#ifndef DJW_SOUND_H
#define DJW_SOUND_H

#include <Framework.h>
#include <vector>
#include <string>
#include <iostream>


class Sound {
public:
	Sound();
	Sound(const std::string& filename);
	Sound(const std::vector< std::string >& filenames);

	~Sound()
	{
		alDeleteSources(1, &m_source);
		alDeleteBuffers(m_buffer.size(), m_buffer.data());
	}

	void play()
	{ 
		if(!m_initialised) return;
		if(this->isPlaying()) return; 		
		alSourcePlay(m_source); 
	}

	void pause() 
	{ 
		if (!m_initialised) return;
		if (this->isPaused()) return;
		alSourcePause(m_source); 
	} 

	void stop() 
	{ 
		if (!m_initialised) return;
		if (this->isStopped()) return;
		alSourceStop(m_source); 
	}

	void rewind() { alSourceRewind(m_source); }

	void loop() { alSourcei(m_source, AL_LOOPING, AL_TRUE); }
	void stopLoop() { alSourcei(m_source, AL_LOOPING, AL_FALSE); }

	bool isPlaying()
	{
		return (this->getSourceState() == AL_PLAYING); 
	}

	bool isPaused()
	{
		return (this->getSourceState() == AL_PAUSED);
	}

	bool isStopped()
	{
		return (this->getSourceState() == AL_STOPPED);
	}
	
private:
	ALint getSourceState()
	{
		ALint sourceState;
		alGetSourcei(m_source, AL_SOURCE_STATE, &sourceState);
		return sourceState;
	}

private:
	ALuint m_source;
	std::vector< ALuint > m_buffer;
	bool m_initialised; 
};


#endif 