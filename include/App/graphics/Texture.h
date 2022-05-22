class Texture {
public:
    Texture(const char* filepath, int slot);
    ~Texture();

    void bind() const;

private:
    unsigned int m_TextureID {};
    int m_Slot {};
};