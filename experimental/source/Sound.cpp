#include <Sound.h>

Sound::Sound() :	m_source(0), 
					m_buffer(1, 0), 
					m_initialised(false)
{}


Sound::Sound(	const std::string& fn) :
				m_source(0),
				m_buffer(1, 0),
				m_initialised(true)
{
	alGenBuffers(1, &m_buffer[0]);

	if( !ALFWLoadWaveToBuffer(fn.c_str(), m_buffer[0]) ){
		alDeleteBuffers(m_buffer.size(), m_buffer.data());
		throw std::runtime_error("Error loading " + fn);
	}

	alGenSources(1, &m_source);
	alSourcei(m_source, AL_BUFFER, m_buffer[0]);
}

Sound::Sound(	const std::vector< std::string >& fns) :
				m_source(0),
				m_buffer(fns.size(), 0),
				m_initialised(true)
{
	alGenBuffers(fns.size(), m_buffer.data());

	for(size_t i = 0; i < m_buffer.size(); ++i){
		if (!ALFWLoadWaveToBuffer(fns[i].c_str(), m_buffer[i])){
			alDeleteBuffers(m_buffer.size(), m_buffer.data());
			throw std::runtime_error("Error loading " + fns[i]);
		}
	}

	alGenSources(1, &m_source); //always one source

	alSourcei(m_source, AL_BUFFER, m_buffer[0]); //for now attach first buffer only
}

