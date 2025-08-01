#include "rendering/framebuffer.hpp"


FrameBuffer::FrameBuffer()
    : m_texture(0, 0), m_depthTexture(0, 0)
{
    this->Init(IVec2(0));
}


FrameBuffer::FrameBuffer(IVec2 size)
    : m_texture(size.x, size.y), m_depthTexture(size.x, size.y)
{
    this->Init(size);
}


FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &this->m_id);
    this->m_texture.Delete();
    this->m_depthTexture.Delete();
}


void FrameBuffer::Init(IVec2 size)
{
    this->m_size = size;
    this->m_texture = BasicTexture(size.x, size.y);
    this->m_depthTexture = BasicTexture(
        size.x, 
        size.y, 
        GL_DEPTH_COMPONENT24, 
        GL_DEPTH_COMPONENT
    );

    glGenFramebuffers(1, &this->m_id);
    glBindFramebuffer(GL_FRAMEBUFFER, this->m_id);
    
    // The depth texture is required for depth testing
    glFramebufferTexture2D(
        GL_FRAMEBUFFER, 
        GL_DEPTH_ATTACHMENT, 
        GL_TEXTURE_2D, 
        this->m_depthTexture.GetID(), 
        0
    );

    // Colored texture containing the render data
    glFramebufferTexture2D(
        GL_FRAMEBUFFER, 
        GL_COLOR_ATTACHMENT0, 
        GL_TEXTURE_2D, 
        this->m_texture.GetID(), 
        0
    );
    
    // Check for errors in the creation of the frambuffer
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
    {
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
            LOG_ERR("Framebuffer incomplete: {0:#x}", (int)status);
    }

    // Unbind the frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void FrameBuffer::Delete()
{
    glDeleteFramebuffers(1, &this->m_id);

    this->m_texture.Delete();
    this->m_depthTexture.Delete();
}


void FrameBuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->m_id);
}


void FrameBuffer::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void FrameBuffer::Resize(IVec2 size)
{
    if (size == this->m_size)
        return;

    this->Delete();
    this->Init(size);
}


GLuint FrameBuffer::GetTextureID() const
{
    return this->m_texture.GetID();
}
